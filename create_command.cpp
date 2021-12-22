#include"database.h"
#include"commands.h"

Create_Command::Create_Command(Database* db): Command(db){}

string Create_Command::command(string command, int ind){
  int delimeter = command.find_first_of(",;", ind);
  if (delimeter == string::npos){
    db->clear_queue();
    db->roll_back();
    return "ERROR: NOT VALID INPUT\n";
  }
  string element = command.substr(ind, delimeter-ind);
  trim_whitespace(element);
  int space_ind = element.find_first_of(" ");
  if (space_ind == string::npos){
    db->clear_queue();
    db->roll_back();
    return "ERROR: NOT VALID INPUT\n";
  }
  string name = element.substr(0, space_ind);
  string size = element.substr(space_ind+1, element.length()-(space_ind+1));
  get_current_table()->add_column(name, stoi(size));
  if (command[delimeter] == ','){
    db->submit_job_to_queue(name + " " + size + ",", "SCHEMA");
    db->add_to_rollback(this);
    return commands["addValue"]->command(command, delimeter+1);
  } else if (command[delimeter] == ';'){
    db->submit_job_to_queue(name + " " + size + ";\n", "SCHEMA");
    db->commit_queue_to_disk();
    db->clear_stack();
    return "Table Created\n";
  }
  return "Error";
}

void Create_Command::rollback(string params){
  Table::SchemaObj so = get_current_table()->schema.back();
  get_current_table()->schema.pop_back();
  get_current_table()->columns.erase(so.name);
  get_current_table()->row_size -= so.size;
}
