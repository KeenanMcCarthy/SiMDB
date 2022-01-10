#include"commands.h"
#include<unordered_set>

In_Command::In_Command(Database* db): Command(db){}

string In_Command::command(string command, int ind){
  unordered_set<string> response_set;
  int field_end = command.find_last_not_of(' ', ind);
  int field_start = command.find_last_of(' ', field_end);
  string field = command.substr(field_start+1, field_end-field_start);
  int par_start = command.find_first_of('(', ind);
  db->current_table.push(get_current_table());
  string inner_response = commands["ROOT"]->command(command, par_start+1);
  db->current_table.pop();
  int row_start = 0;
  int row_end = 0;
  while ((row_end=inner_response.find_first_of('\n', row_start)) != string::npos){
    string entry = inner_response.substr(row_start, row_end-row_start);
    string primary_key = entry.substr(0, entry.find_first_of(",\n"));
    response_set.insert(primary_key);
    row_start = row_end+1;
  }
  string response = "";
  for (int i=0; i<get_current_table()->table.size(); i++){
    string element = get_current_table()->table[i][get_current_table()->columns[field]];
    if (response_set.find(element) != response_set.end()){
      for (int j=0; j<get_current_table()->table[0].size(); j++){
        response += (get_current_table()->table[i][j] + ",");
      }
      response.pop_back();
      response += '\n';
    }
  }
  return response;
}
