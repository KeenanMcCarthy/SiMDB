#ifndef SERVER_H
#define SERVER_H

#include<iostream>
#include<fstream>
#include<string>
#include<unordered_map>
#include<queue>
#include<filesystem>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<unistd.h>
#include"database.h"
#include"request_obj.h"

#define BUFFER_SIZE 1024

using namespace std;

class Server{
public:
  Database* db;
  Server(Database* db);
  void run_server(int);
  void run_server();
};

#endif
