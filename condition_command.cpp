#include"commands.h"

Condition_Command::Condition_Command(Database* db): Command(db){}

bool Condition_Command::evaluate(int row_ind, string field, string value, string op){
  string element = get_current_table()->table[row_ind][get_current_table()->columns[field]];
  int comp_val;
  try {
    int int_element = stoi(element);
    int int_val;
    try {
      int_val = stoi(value);
    } catch (exception e){
      cout << "types incompatible " << endl;
    }
    comp_val = int_element-int_val;
  } catch (exception e){
    to_uppercase(element);
    comp_val = element.compare(value);
  }
  if (op == "=" && comp_val == 0){
    return true;
  }
  if (op == "<" && comp_val < 0){
    return true;
  }
  if (op == "<=" && comp_val <= 0){
    return true;
  }
  if (op == ">=" && comp_val >= 0){
    return true;
  }
  if (op == ">" && comp_val > 0){
    return true;
  }
  if (op == "!=" && comp_val != 0){
    return true;
  }
  return false;
}

string Condition_Command::command(string command, int ind){
  string field = "";
  string value = "";
  string op = "";
  int end_ind = command.find_first_of(",;)", ind);
  int op_start = command.find_first_of("=!<>", ind);
  int op_end = op_start+1;
  bool has_condition = true;
  if (command[command.find_first_not_of(' ', ind)] == ';'){
    has_condition = false;
  } else if (op_end == string::npos || op_end > end_ind){
    return "NOT VALID INPUT\n";
  }
  if (has_condition){
    if ((command[op_start] == '>' || command[op_start] == '<' || command[op_start] == '!')
    && (command[op_end] == '=')){
      op_end ++;
    }
    op = command.substr(op_start, op_end-op_start);
    field = command.substr(ind, op_start-ind);
    value = command.substr(op_end, end_ind-op_end);
    trim_whitespace(field);
    trim_whitespace(value);
    to_uppercase(value);
  }
  string response = "";
  for (int i=0; i<get_current_table()->table.size(); i++){
    if (!has_condition || evaluate(i, field, value, op)){
      for (int j=0; j<get_current_table()->table[0].size(); j++){
        response += (get_current_table()->table[i][j] + ",");
      }
      response.pop_back();
      response += '\n';
    }
  }
  return response;
}
