#include"commands.h"

Trxn_Command::Trxn_Command(Database* db): Command(db){};

string Trxn_Command::command(string command, int ind){
  int end = command.find_first_of(';', ind);
  string task = command.substr(ind, end-ind);
  trim_whitespace(task);
  to_uppercase(task);
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
  if (db->to_rollback){
    db->rollback();
    db->to_rollback = false;
    return "Invalid Transaction\n";
  }
  db->commit();
  return "Transaction completed\n";
}
