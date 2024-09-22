#ifndef COMMAND_FACADE_H
#define COMMAND_FACADE_H

#include<string>

class Database;
class Command;

using namespace std;

class Command_Facade{
public:
  Command* root;
  Database* db;

  Command_Facade(Database* db);
  string run_command(string command);
};

#endif
