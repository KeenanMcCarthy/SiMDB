#include"commands.h"
#include<unordered_set>

Get_All_Command::Get_All_Command(Database* db): Command(db){}

string Get_All_Command::get_conditional_response(string command, int ind){
  string cond_response = "";
  int field_start_ind = command.find_first_not_of(' ', ind);
  int end_ind = command.find_first_of(";,()", ind);
  int in_ind = command.find(" IN", field_start_ind);
  int in_ind_lower = command.find(" in", field_start_ind);
  if (in_ind != string::npos && in_ind+2 < end_ind){
    return commands["IN"]->command(command, in_ind);
  }
  if (in_ind_lower != string::npos && in_ind_lower+2 < end_ind){
    return commands["IN"]->command(command, in_ind_lower);
  }
  if (command[field_start_ind] == '('){
    return commands["PARENTHESES"]->command(command, field_start_ind);
  }
  return commands["COND"]->command(command, ind);
}

int Get_All_Command::skip_parenthese(string command, int ind){
    int num_parenthese = 0;
    for (int i=ind; i<command.length(); i++){
      if (command[i] == '('){
        num_parenthese ++;
      }
      if (command[i] == ')'){
        num_parenthese --;
      }
      if (num_parenthese == 0){
        return i;
      }
    }
    return command.length();
}

string Get_All_Command::command(string command, int ind){
  if (command[ind] == ';'){
    return commands["COND"]->command(command,ind);
  }
  string response = "";
  unordered_set<string> response_set;
  while (command[ind] != ';'){
    ind ++;
    ind = command.find_first_not_of(' ', ind);
    if (command[ind] == ')'){
      return response;
    }
    int op_end = command.find_first_of(' ', ind);
    string op = command.substr(ind, op_end-ind);
    trim_whitespace(op);
    to_uppercase(op);
    if (op == "IF" || op == "OR"){
      string cond_response = get_conditional_response(command, op_end);
      int row_start = 0;
      int row_end = 0;
      while ((row_end=cond_response.find_first_of('\n', row_start)) != string::npos){
        string entry = cond_response.substr(row_start, row_end-row_start);
        string primary_key = entry.substr(0, entry.find_first_of(",\n"));
        if (response_set.find(primary_key) == response_set.end()){
          response += (entry+'\n');
          response_set.insert(primary_key);
        }
        row_start = row_end+1;
      }
    } else if (op == "AND"){
      string cond_response = get_conditional_response(command, op_end);
      string new_response = "";
      unordered_set<string> new_set;
      int row_start = 0;
      int row_end = 0;
      while ((row_end=cond_response.find_first_of('\n', row_start)) != string::npos){
        string entry = cond_response.substr(row_start, row_end-row_start);
        string primary_key = entry.substr(0, entry.find_first_of(",\n"));
        if (response_set.find(primary_key) != response_set.end()){
          new_response += (entry+'\n');
          new_set.insert(primary_key);
        }
        row_start = row_end+1;
      }
      response_set = new_set;
      response = new_response;
    } else {
      return "INVALID QUERY\n";
    }
    int end_ind = command.find_first_of(";,()", ind);
    if (command[end_ind] == ')'){
      return response;
    }
    if (command[end_ind] == '('){
      ind = skip_parenthese(command, end_ind);
      ind = command.find_first_of(",;", ind);
    } else {
      ind = end_ind;
    }
  }
  return response;
}
