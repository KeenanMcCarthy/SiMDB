#include"database.h"
#include"commands.h"

Set_Table_Command::Set_Table_Command(Database* db): Command(db){}

string Set_Table_Command::command(string command, int ind){
  int end_ind = command.find_first_of(':', ind);
  string table_name = command.substr(ind, end_ind-ind);
  trim_whitespace(table_name);
  if (db->tables.find(table_name) == db->tables.end()){
    return "Table does not exist\n";
  }
  if (get_current_table() != nullptr){
    db->add_to_rollback(this, get_current_table()->name);
  }
  db->set_cur_table(table_name);
  return commands["addValue"]->command(command, end_ind+1);
}

void Set_Table_Command::rollback(string params){
  db->set_cur_table(params);
}
