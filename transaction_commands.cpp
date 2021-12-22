#include"commands.h"

Trxn_Command::Trxn_Command(Database* db): Command(db){};

string Trxn_Command::command(string command, int ind){
  int end = command.find_first_of(';', ind);
  string task = command.substr(ind, end-ind);
  trim_whitespace(task);
  if (commands.find(task) != commands.end()){
    return commands[task]->command(command, end);
  }
  return "Invalid Input\n";
}

Start_Trxn_Command::Start_Trxn_Command(Database* db): Command(db){}

string Start_Trxn_Command::command(string command, int ind){
  db->in_transaction = true;
  return "Transaction started\n";
}

End_Trxn_Command::End_Trxn_Command(Database* db): Command(db){}

string End_Trxn_Command::command(string command, int ind){
  db->in_transaction = false;
  db->clear_stack();
  db->commit_queue_to_disk();
  return "Transaction completed\n";
}
