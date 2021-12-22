#ifndef DATABASE_H
#define DATABASE_H

#include<iostream>
#include<string>
#include<unordered_map>
#include<queue>
#include<stack>
#include<vector>
#include<thread>
#include"command_facade.h"
#include"commands.h"
#include"table.h"
#include"server.h"

using namespace std;

class Database{
  class CommitJob{
  public:
    Table* table;
    string data;
    string operation;
    CommitJob(Table* table, string data, string operation);
  };
  class RollbackObj{
  public:
    Command* command;
    string params;
    RollbackObj(Command* command, string params);
  };
public:
  Command_Facade* commander;
  unordered_map<string, Table*> tables;
  Table* current_table;
  queue<CommitJob> disk_commit_queue;
  stack<RollbackObj> rollback_stack;
  bool in_transaction;

  Database();

  void start_commandline();
  void submit_job_to_queue(string data, string operation);
  void commit_queue_to_disk();
  string run_command(string command);
  void add_table(string name);
  void set_cur_table(string name);
  void run_server();
  void run_server(int port);
  void clear_queue();
  void clear_stack();
  void add_to_rollback(Command* command, string params);
  void add_to_rollback(Command* command);
  void roll_back();
};

#endif
