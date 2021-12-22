#include"commands.h"

Delete_Command::Delete_Command(Database* db): Command(db){}

string Delete_Command::command(string command, int ind){
  int end_ind = command.find_first_of(';', ind);
  string table_name = command.substr(ind, end_ind-ind);
  trim_whitespace(table_name);
  if (db->tables.find(table_name) == db->tables.end()){
    return "Table does not exist\n";
  } else {
    db->tables.erase(table_name);
    try {
      std::__fs::filesystem::remove("tables/"+table_name+".txt");
      return "Table " + table_name + " deleted\n";
    } catch (const std::__fs::filesystem::filesystem_error& err){
      return "filesystem error\n";
    }
  }
}
