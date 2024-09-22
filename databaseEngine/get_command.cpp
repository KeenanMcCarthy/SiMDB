#include"commands.h"

Get_Command::Get_Command(Database* db): Command(db){}

string Get_Command::command(string command, int ind){
  int op_start = command.find_first_not_of(" ", ind);
  int end_ind = command.find_first_of(" ;", op_start);
  string pk_name = command.substr(op_start, end_ind-op_start);
  trim_whitespace(pk_name);
  if (pk_name == "*"){
    return commands["*"]->command(command, end_ind);
  }
  if (get_current_table()->rows.count(pk_name) == 0){
    db->rollback();
    return "Entry not found\n";
  }
  vector<string> row = get_current_table()->table[get_current_table()->rows[pk_name]];
  string response = "";
  if (get_current_fields().size() == 0){
    for (string entry: row){
      response += (entry + " ");
    }
  } else {
    for (string field: get_current_fields()){
      response += (row[get_current_table()->columns[field]]);
    }
  }
  response += '\n';
  return response;
}
