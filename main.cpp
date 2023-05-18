#include<iostream>
#include"database.h"
#include<string>

using namespace std;

int main(int argc, char **argv){
  Database db = Database();
  if (argc>1 && std::strcmp(argv[1],"SERVER") == 0){
    if (argc > 2){
      db.run_server(stoi(argv[2]));
    } else {
      db.run_server();
    }
  } else {
    db.start_commandline();
  }
  return 0;
}
