#include<string>
#include"commands.h"

Insert_PK_Command::Insert_PK_Command(Database* db): Command(db){}

string Insert_PK_Command::command(string command, int ind){
  int delimeter = command.find_first_of(",;", ind);
  string pk_name = command.substr(ind, delimeter-ind);
  trim_whitespace(pk_name);
  db->add_to_rollback(this, pk_name);
  Table::SchemaObj co = get_current_table()->schema[0];
  if (pk_name.size() > co.size){
    db->rollback();
    return "Value too large, schema size: " + to_string(co.size) + " value size: " + to_string(pk_name.size()) + '\n';
  }
  get_current_table()->rows[pk_name] = get_current_table()->table.size();
  get_current_table()->table.push_back(vector<string>({pk_name}));
  if (command[delimeter] == ','){
    db->submit_job_to_queue(pk_name, "VALUE " + to_string(0));
    return commands["addValue"]->command(command, delimeter+1);
  } else if (command[delimeter] == ';' &&
  get_current_table()->table.back().size() == get_current_table()->schema.size()){
    db->submit_job_to_queue(pk_name+'\n', "VALUE " + to_string(0));
    db->commit();
    return "Values inserted\n";
  } else {
    db->rollback();
    return "ERROR: NOT VALID INPUT\n";
  }
}

void Insert_PK_Command::rollback(string params){
  get_current_table()->rows.erase(params);
  get_current_table()->table.pop_back();
}
