#include"commands.h"
#include"database.h"

Insert_Command::Insert_Command(Database* db): Command(db){}

string Insert_Command::command(string command, int ind){
  int delimeter = command.find_first_of(",;", ind);
  string name = command.substr(ind, delimeter-ind);
  trim_whitespace(name);
  Table::SchemaObj so = get_current_table()->schema[get_current_table()->table.back().size()];
  if (name.size() > so.size){
    db->rollback();
    return "Value too large, schema size: " + to_string(so.size) + " value size: " + to_string(name.size()) + '\n';
  }
  int schema_index = get_current_table()->table.back().size();
  get_current_table()->table.back().push_back(name);
  if (command[delimeter] == ','){
    db->submit_job_to_queue(name, "VALUE "+to_string(schema_index));
    return commands["addValue"]->command(command, delimeter+1);
  } else if (command[delimeter] == ';' &&
get_current_table()->table.back().size() == get_current_table()->schema.size()){
    db->submit_job_to_queue(name + '\n', "VALUE " + to_string(schema_index));
    db->commit();
    return "Values inserted\n";
  } else {
    db->rollback();
    return "NOT VALID INPUT\n";
  }
}

void Insert_Command::rollback(string params){
  cout << "do nothing" << endl;
}
