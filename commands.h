#ifndef CREATE_COMMAND_H
#define CREATE_COMMAND_H

#include<string>
#include<filesystem>
#include"database.h"
#include"table.h"

using namespace std;

class Command{
public:
  unordered_map<string, Command*> commands;
  Database* db;
  Command(Database*);
  virtual string command(string command, int ind) = 0;
  virtual void rollback(string params);
  Table* get_current_table();
  void add_command(string name, Command* command);
  void trim_whitespace_left(string& str);
  void trim_whitespace_right(string& str);
  void trim_whitespace(string& str);
};

class Root_Command: public Command{
public:
  Root_Command(Database* db);
  string command(string command, int ind);
};

class Make_Table_Command: public Command{
public:
  Make_Table_Command(Database* db);
  string command(string command, int ind);
  void rollback(string params);
};

class Create_Command: public Command{
public:
  Create_Command(Database* db);
  string command(string command, int ind);
  void rollback(string params);
};

class Set_Table_Command: public Command{
public:
  Set_Table_Command(Database* db);
  string command(string command, int ind);
  void rollback(string params);
};

class Get_Command: public Command{
public:
  Get_Command(Database* db);
  string command(string command, int ind);
};

class Get_All_Command: public Command{
public:
  Get_All_Command(Database* db);
  string command(string command, int ind);
};

class Insert_PK_Command: public Command{
public:
  Insert_PK_Command(Database* db);
  string command(string command, int ind);
  void rollback(string params);
};

class Trxn_Command: public Command{
public:
  Trxn_Command(Database* db);
  string command(string command, int ind);
};

class Start_Trxn_Command: public Command{
public:
  Start_Trxn_Command(Database* db);
  string command(string command, int ind);
};

class End_Trxn_Command: public Command{
public:
  End_Trxn_Command(Database* db);
  string command(string command, int ind);
};

class Delete_Command: public Command{
public:
  Delete_Command(Database* db);
  string command(string command, int ind);
};

class Insert_Command: public Command{
public:
  Insert_Command(Database* db);
  string command(string command, int ind);
  void rollback(string params);
};

class Update_Command: public Command{
public:
  Update_Command(Database* db);
  string command(string command, int ind);
  void rollback(string params);
};

#endif
