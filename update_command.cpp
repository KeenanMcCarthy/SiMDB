#include"commands.h"

Update_Command::Update_Command(Database* db): Command(db){}

string Update_Command::command(string command, int ind){
  int first_comma = command.find_first_of(',', ind);
  string row = command.substr(ind, first_comma-ind);
  trim_whitespace(row);
  int second_comma = command.find_first_of(',', first_comma+1);
  string col = command.substr(first_comma+1, second_comma-(first_comma+1));
  trim_whitespace(col);
  int semi_colon = command.find_first_of(';', second_comma);
  string value = command.substr(second_comma+1, semi_colon-(second_comma+1));
  trim_whitespace(value);
  int row_ind = get_current_table()->rows[row];
  int col_ind = get_current_table()->columns[col];
  string old_val = get_current_table()->table[row_ind][col_ind];
  get_current_table()->table[row_ind][col_ind] = value;
  db->add_to_rollback(this, old_val+",UPDATE"+to_string(row_ind)+" "+to_string(col_ind));
  db->submit_job_to_queue(value, "UPDATE "+to_string(row_ind)+" "+to_string(col_ind));
  db->commit_queue_to_disk();
  db->clear_stack();
  return "Value updated successfully\n";
}

void Update_Command::rollback(string params){
  int first_comma = params.find_first_of(',');
  string old_val = params.substr(0, first_comma);
  params.erase(0, first_comma+1);
  db->submit_job_to_queue(old_val, params);
}
