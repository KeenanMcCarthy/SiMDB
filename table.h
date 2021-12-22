#ifndef TABLE_H
#define TABLE_H

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

using namespace std;

class Table{
public:
  class SchemaObj{
  public:
    string name;
    unsigned int size;
    unsigned int offset;
    SchemaObj(string name, unsigned int size, unsigned int offset);
  };

  class WriteJob{
  public:
    string operation;
    string data;
    WriteJob(string data, string operation);
  };

  string name;
  Table(string name);
  Table(string name, string path);

  unsigned int schema_size;
  unsigned int row_size;
  unsigned int num_rows;
  bool disk_thread_running;

  vector<vector<string>> table;
  vector<SchemaObj> schema;
  unordered_map<string, int> rows;
  unordered_map<string, int> columns;
  //queue<string> request;
  queue<WriteJob> to_write;

  void add_column(string name, int size);
  void submit_to_job_queue(string data, string operation);
  void run_job(WriteJob job);

private:
  void load_table(string path);
  void run_thread();
  void write_schema_to_disk(string data);
  void write_value_to_disk(string data, int schema_index);
  void update_disk(int row, int col, string data);
};

#endif
