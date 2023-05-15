#ifndef REQUEST_OBJ_H
#define REQUEST_OBJ_H

#include<iostream>
#include"database.h"
#include<string>

using namespace std;

class Request_Obj {
private:
  void parse_start_line(string start_line);
  void parse_header(string header);
  enum http_reqest_type { POST, GET, PUSH, DELETE, PUT};
public:
  ~Request_Obj(){};
  http_reqest_type http_type;
  string path;
  string http_version;
  int content_length;
  unordered_map<string, string> headers;
  unordered_map<string, string> response_headers;

  string query;

  Request_Obj(char buffer [], int buffer_size);
  void add_response_code(string response);
  string query_database(Database* db);
};

#endif
