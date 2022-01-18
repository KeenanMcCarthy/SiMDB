#include"commands.h"
#include"database.h"

Command::Command(Database* db){
  this->db = db;
}

Table* Command::get_current_table(){
  if (db->current_table.empty()){
    return nullptr;
  }
  return db->current_table.top().table;
}

vector<string> Command::get_current_fields(){
  return db->current_table.top().fields;
}

void Command::to_uppercase(string& str){
  for (int i=0; i<str.length(); i++){
    str[i] = toupper(str[i]);
  }
}

void Command::add_command(string name, Command* command){
  commands[name] = command;
}

void Command::trim_whitespace_left(string& str){
  str.erase(str.begin(), str.begin()+str.find_first_not_of(" \n\r\t"));
}

void Command::trim_whitespace_right(string& str){
  str.erase(str.find_last_not_of(" \n\r\t")+1);
}

void Command::trim_whitespace(string& str){
  trim_whitespace_left(str);
  trim_whitespace_right(str);
}

void Command::rollback(string params){}

Root_Command::Root_Command(Database* db) : Command(db){}

string Root_Command::command(string command, int ind){
  trim_whitespace_left(command);
  int delimeter_ind = command.find_first_of(" ", ind);
  string element = command.substr(ind, delimeter_ind-ind);
  to_uppercase(element);
  if (commands.find(element) != commands.end()){
    return commands[element]->command(command, delimeter_ind+1);
  } else {
    db->rollback();
    return "NOT VALID INPUT\n";
  }
}
