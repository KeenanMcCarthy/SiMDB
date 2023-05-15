#include"database.h"
#include"commands.h"

Set_Table_Command::Set_Table_Command(Database* db): Command(db){}

vector<string> Set_Table_Command::split_str(string fields, string table_name){
  vector<string> split_fields = vector<string>();
  int start = 0;
  int end = 0;
  while (start < fields.length()){
    end = fields.find_first_of(',', start);
    if (end == string::npos){
      end = fields.length();
    }
    string field = fields.substr(start, end-start);
    trim_whitespace(field);
    if (db->tables[table_name]->columns.count(field) > 0){
        split_fields.push_back(field);
    } else {
      throw invalid_argument("not a valid field \n");
    }
    start = end+1;
  }
  return split_fields;
}

string Set_Table_Command::command(string command, int ind){
  int end_ind = command.find_first_of(':', ind);
  vector<string> fields = vector<string>();
  int period_ind = command.find_first_of('.', ind);
  string table_name;
  if (period_ind != string::npos && period_ind < end_ind){
    table_name = command.substr(ind, period_ind-ind);
    string str_fields = command.substr(period_ind+1, end_ind-(period_ind+1));
    try {
      fields = split_str(str_fields, table_name);
    } catch (invalid_argument& e){
      return e.what();
    }
  } else {
    table_name = command.substr(ind, end_ind-ind);
  }
  trim_whitespace(table_name);
  if (db->tables.find(table_name) == db->tables.end()){
    return "ERROR: Table does not exist\n";
  }
  if (get_current_table() != nullptr){
    db->add_to_rollback(this, get_current_table()->name);
  }
  db->set_cur_table(table_name, fields);
  string ret = commands["addValue"]->command(command, end_ind+1);
  db->pop_current_table();
  return ret;
}

void Set_Table_Command::rollback(string params){
  db->pop_current_table();
}
