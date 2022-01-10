#include"database.h"

using namespace std;

Database::Database(){
  commander = new Command_Facade(this);
  current_table = stack<Table*>();
  in_transaction = false;
  to_rollback = false;
  for (const auto& file: std::__fs::filesystem::directory_iterator("./tables")){
    string path = file.path();
    int last_slash_ind = path.find_last_of('/');
    int extention_ind = path.find_first_of('.', last_slash_ind);
    string name = path.substr(last_slash_ind+1, extention_ind-(last_slash_ind+1));
    this->tables[name] = new Table(name, path);
  }
}

Database::CommitJob::CommitJob(Table* table, string data, string operation){
  this->table = table;
  this->data = data;
  this->operation = operation;
}

Database::RollbackObj::RollbackObj(Command* command, string params){
  this->command = command;
  this->params = params;
}

void Database::commit(){
  if (in_transaction){
    return;
  }
  commit_queue_to_disk();
  clear_stack();
}

void Database::rollback(){
  if (in_transaction){
    to_rollback = true;
    return;
  }
  roll_back_stack();
  clear_queue();
}

void Database::start_commandline(){
  string input;
  getline(cin, input);
  while (input.compare("done") != 0){
    cout << commander->run_command(input);
    getline(cin, input);
  }
}

void Database::submit_job_to_queue(string data, string operation){
  disk_commit_queue.push(CommitJob(current_table.top(), data, operation));
}

void Database::commit_queue_to_disk(){
  while (!disk_commit_queue.empty()){
    CommitJob commit_job = disk_commit_queue.front();
    disk_commit_queue.pop();
    commit_job.table->submit_to_job_queue(commit_job.data, commit_job.operation);
  }
}

string Database::run_command(string command){
  return commander->run_command(command);
}

void Database::run_server(int port){
  cout << ("KDB is running on port " + to_string(port)) << endl;
  Server s = Server(this);
  s.run_server(port);
}

void Database::run_server(){
  cout << "KDB is running on port 3490" << endl;
  Server s = Server(this);
  s.run_server();
}

void Database::clear_queue(){
  if (in_transaction) return;
  queue<CommitJob> empty;
  swap(disk_commit_queue, empty);
}

void Database::clear_stack(){
  if (in_transaction) return;
  stack<RollbackObj> empty;
  swap(rollback_stack, empty);
}

void Database::add_to_rollback(Command* command, string params){
  rollback_stack.push(RollbackObj(command, params));
}

void Database::add_to_rollback(Command* command){
  rollback_stack.push(RollbackObj(command, ""));
}

void Database::roll_back_stack(){
  while (!rollback_stack.empty()){
    RollbackObj rbo = rollback_stack.top();
    rollback_stack.pop();
    rbo.command->rollback(rbo.params);
  }
}

void Database::add_table(string name){
  this->tables[name] = new Table(name);
}

void Database::set_cur_table(string name){
  if (tables.find(name) != tables.end()){
    current_table.push(tables[name]);
    return;
  }
  cout << "Table does not exist" << endl;
}
