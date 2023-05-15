#include"database.h"
#include"commands.h"

Make_Table_Command::Make_Table_Command(Database* db): Command(db){}

string Make_Table_Command::command(string command, int ind){
  int end_ind = command.find_first_of(':', ind);
  string table_name = command.substr(ind, end_ind-ind);
  trim_whitespace(table_name);
  if (db->tables.find(table_name) != db->tables.end()){
    return "Table already exists\n";
  }
  db->add_to_rollback(this, table_name);
  db->add_table(table_name);
  db->set_cur_table(table_name);
  return commands["addSchema"]->command(command, end_ind+1);
}

void Make_Table_Command::rollback(string params){
  db->tables.erase(params);
  try {
    std::__fs::filesystem::remove("tables/"+params+".txt");
  } catch (const std::__fs::filesystem::filesystem_error& err){
    cout << "ERROR: filesystem error\n" << endl;
  }
}
