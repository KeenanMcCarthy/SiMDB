#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<unordered_map>
#include<queue>
#include"database.h"
#include"command_facade.h"

using namespace std;

Table::Table(string name){
  this->name = name;
  disk_thread_running = false;
  schema_size = 0;
  row_size = 0;
  num_rows = 0;
  ofstream outfile("tables/"+name+".txt");
  outfile.close();
}

Table::Table(string name, string path){
  this->name = name;
  disk_thread_running = false;
  schema_size = 0;
  row_size = 0;
  num_rows = 0;
  load_table(path);
}

Table::SchemaObj::SchemaObj(string name, unsigned int size, unsigned int offset){
  this->name = name;
  this->size = size;
  this->offset = offset;
}

Table::WriteJob::WriteJob(string data, string operation){
  this->data = data;
  this->operation = operation;
}

void Table::add_column(string name, int size){
  columns[name] = schema.size();
  schema.push_back(SchemaObj(name, size, row_size));
  row_size += size;
}

void Table::submit_to_job_queue(string data, string operation){
  to_write.push(WriteJob(data, operation));
  if (!disk_thread_running){
    thread disk_thread ([this](){
      run_thread();
    });
    disk_thread.detach();
    disk_thread_running = true;
  }
}

void Table::run_job(WriteJob job){
  if (job.operation == "SCHEMA"){
    write_schema_to_disk(job.data);
  } else if (job.operation.substr(0, 5) == "VALUE"){
    int size_start_ind = job.operation.find_first_of(" ")+1;
    int size_length = job.operation.size()-size_start_ind;
    write_value_to_disk(job.data, stoi(job.operation.substr(size_start_ind, size_length)));
  } else if (job.operation.substr(0, 6) == "UPDATE"){
    int space_ind = job.operation.find_first_of(' ', 7);
    int row = stoi(job.operation.substr(7, space_ind-7));
    int col = stoi(job.operation.substr(space_ind+1, job.operation.size()-(space_ind+1)));
    update_disk(row, col, job.data);
  }
}

void Table::run_thread(){
  while (!to_write.empty()){
    WriteJob job = to_write.front();
    to_write.pop();
    run_job(job);
  }
  disk_thread_running = false;
}

void Table::load_table(string path){
  ifstream input_file(path);
  string schema_line;
  getline(input_file, schema_line);
  int start_ind = 0;
  int end_ind = 0;
  schema_size = 1;
  while (start_ind < schema_line.size()){
    end_ind = schema_line.find_first_of(",;", start_ind);
    string element = schema_line.substr(start_ind, end_ind-start_ind);
    int space_ind = element.find_first_of(" ");
    string name = element.substr(0, space_ind);
    string size = element.substr(space_ind+1, element.length()-(space_ind+1));
    add_column(name, stoi(size));
    start_ind = end_ind+1;
    schema_size += (name.length() + size.length() + 2);
  }
  string line;
  while (getline(input_file, line)){
    table.push_back(vector<string>());
    int line_ind = 0;
    for (int schema_ind=0; schema_ind<schema.size(); schema_ind++){
      string data = line.substr(line_ind, schema[schema_ind].size);
      data.erase(data.find_last_not_of(" \n\r\t")+1);
      if (schema_ind == 0){
        rows[data] = table.size()-1;
      }
      table.back().push_back(data);
      line_ind += schema[schema_ind].size;
    }
  }
}

void Table::write_schema_to_disk(string data){
  schema_size += data.size();
  ofstream dest_file;
  dest_file.open("tables/"+name+".txt", ofstream::app);
  dest_file << data;
}

void Table::write_value_to_disk(string data, int schema_index){
  bool new_line = false;
  if (data.back() == '\n'){
    new_line = true;
    data.pop_back();
  }
  int element_size = schema[schema_index].size;
  int buffer_size = element_size-data.length();
  for (int i=0; i<buffer_size; i++){
      data += ' ';
  }
  if (new_line){
    data += '\n';
  }
  ofstream dest_file;
  dest_file.open("tables/"+name+".txt", ofstream::app);
  dest_file << data;
  dest_file.close();
}

void Table::update_disk(int row, int col, string data){
  int element_size = schema[col].size;
  int buffer_size = element_size-data.length();
  for (int i=0; i<buffer_size; i++){
    data += ' ';
  }
  int total_offset = schema_size + (row*row_size) + row + schema[col].offset;

  fstream dest_file;
  dest_file.open("tables/"+name+".txt", ios::binary | ios::out | ios::in);
  dest_file.seekp(total_offset, ios::beg);
  dest_file.write(data.c_str(), schema[col].size);
  dest_file.close();
}
