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
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    if ((valread = read(new_socket, buffer, BUFFER_SIZE))<0){
      perror("valread err");
      exit(1);
    };
    Request_Obj request_obj = Request_Obj(buffer, BUFFER_SIZE);
    string response = "";
    string ret = request_obj.query_database(this->db);
    unordered_map<string, string>::iterator itr;
    for (itr=request_obj.response_headers.begin();
        itr!=request_obj.response_headers.end(); ++itr) {
      response += itr->first;
      response += itr->second;
      response += "\r\n\r\n";
    }
    response += ret;
    send(new_socket, response.c_str(), response.length(), 0);
    close(new_socket);
  }
}
