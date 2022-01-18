#include"command_facade.h"
#include"commands.h"

using namespace std;

Command_Facade::Command_Facade(Database* db){
  this->db = db;
  Root_Command* root = new Root_Command(db);
  Make_Table_Command* make_table = new Make_Table_Command(db);
  Create_Command* create = new Create_Command(db);
  Set_Table_Command* set_table_pk = new Set_Table_Command(db);
  Set_Table_Command* set_table_get = new Set_Table_Command(db);
  Set_Table_Command* set_table_update = new Set_Table_Command(db);
  Insert_PK_Command* insert_pk = new Insert_PK_Command(db);
  Insert_Command* insert = new Insert_Command(db);
  Get_Command* get = new Get_Command(db);
  Get_All_Command* get_all = new Get_All_Command(db);
  Delete_Command* delete_command = new Delete_Command(db);
  Update_Command* update_command = new Update_Command(db);
  Trxn_Command* trxn_command = new Trxn_Command(db);
  Start_Trxn_Command* start_trxn_command = new Start_Trxn_Command(db);
  End_Trxn_Command* end_trxn_command = new End_Trxn_Command(db);
  Condition_Command* condition_command = new Condition_Command(db);
  In_Command* in_command = new In_Command(db);
  Sum_Command* sum_command = new Sum_Command(db);
  insert_pk->add_command("addValue", insert);
  insert->add_command("addValue", insert);
  root->add_command("CREATE", make_table);
  root->add_command("DELETE", delete_command);
  root->add_command("UPDATE", set_table_update);
  root->add_command("SUM", sum_command);
  sum_command->add_command("addValue", set_table_get);
  trxn_command->add_command("START", start_trxn_command);
  trxn_command->add_command("END", end_trxn_command);
  root->add_command("TRANSACTION", trxn_command);
  make_table->add_command("addSchema", create);
  root->add_command("INSERT", set_table_pk);
  in_command->add_command("ROOT", root);
  get_all->add_command("COND", condition_command);
  get_all->add_command("PARENTHESES", get_all);
  get_all->add_command("IN", in_command);
  get->add_command("*", get_all);
  root->add_command("GET", set_table_get);
  set_table_pk->add_command("addValue", insert_pk);
  set_table_get->add_command("addValue", get);
  set_table_update->add_command("addValue", update_command);
  create->add_command("addValue", create);
  this->root = root;
}

string Command_Facade::run_command(string command){
  return root->command(command, 0);
}
