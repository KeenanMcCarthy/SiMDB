#include"server.h"

Server::Server(Database* db){
  this->db = db;
}

void Server::run_server(){
  run_server(3490);
}

void Server::run_server(int port){
  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("socket failed");
    exit(1);
  }
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = INADDR_ANY;
  if (::bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0){
    perror("bind failed");
    exit(1);
  }
  if (listen(sockfd, 5) < 0){
    perror("Listen");
    exit(1);
  }
  int add_size = sizeof(address);
  while (1) {
    int new_socket;
    if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)& add_size))<0){
      perror("new socket failed");
      exit(1);
    }
    char buffer[1024] = {0};
    int valread;
    if ((valread = read(new_socket, buffer, 1024))<0){
      perror("valread err");
      exit(1);
    };
    string input = "";
    for (int i=146; i<1024; i++){
      input += buffer[i];
    }
    string ret = db->run_command(input);
    send(new_socket, ret.c_str(), ret.length(), 0);
    close(new_socket);
  }
}
