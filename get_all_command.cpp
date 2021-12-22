#include"commands.h"

Get_All_Command::Get_All_Command(Database* db): Command(db){}

string Get_All_Command::command(string command, int ind){
  int end_ind = command.find_first_of(";", ind);
  string condition = command.substr(ind, end_ind-ind);
  trim_whitespace(condition);
  string field = "";
  string value = "";
  if (condition.size() != 0){
    int if_end = condition.find_first_of(" ");
    if (condition.substr(0, if_end) != "IF"){
      return "Invalid query\n";
    }
    int equals_ind = condition.find_first_of("=", if_end);
    field = condition.substr(if_end, equals_ind-if_end);
    trim_whitespace(field);
    value = condition.substr(equals_ind+1, end_ind-(equals_ind+1));
    trim_whitespace(value);
  }
  string response = "";
  for (int i=0; i<get_current_table()->table.size(); i++){
    if (condition.size() == 0 ||
    get_current_table()->table[i][get_current_table()->columns[field]] == value){
      for (int j=0; j<get_current_table()->table[0].size(); j++){
        response += (get_current_table()->table[i][j] + " ");
      }
      response += '\n';
    }
  }
  return response;
}
