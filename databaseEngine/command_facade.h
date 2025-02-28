#ifndef COMMAND_FACADE_H
#define COMMAND_FACADE_H

#include<string>

class Database;
class Command;

using namespace std;

/**
  * Command facade abstracts internal implmentation
  * of the command graph which processes all SiMDB commands.
  * It contains a reference to the database object and the
  * root node of the command graph.
**/
class Command_Facade{
public:
  Command* root;
  Database* db;

  Command_Facade(Database* db);
  string run_command(string command);
};

#endif
