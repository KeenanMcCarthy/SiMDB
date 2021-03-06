#include"request_obj.h"

void Request_Obj::parse_header(string header) {
  int start_ind = header.find_first_not_of(' ');
  int end_ind = header.find_first_of(':');
  string header_key = header.substr(start_ind, end_ind - start_ind);
  start_ind = header.find_first_not_of(' ', end_ind+1);
  end_ind = header.find_first_of(" \r\n", start_ind);
  string header_val = header.substr(start_ind, end_ind - start_ind);
  this->headers[header_key] = header_val;
}

Request_Obj::Request_Obj(char buffer [], int buffer_size){
  string start_line = "";
  int i;
  for (i=0; buffer[i]!='\r' && buffer[i+1]!='\n'; i++){
    start_line += buffer[i];
  }
  parse_start_line(start_line);
  while (true) {
    i += 2;
    if (buffer[i] == '\r' && buffer[i+1] == '\n') {
      break;
    }
    string header = "";
    for (; buffer[i]!='\r' && buffer[i+1]!='\n'; i++) {
      header += buffer[i];
    }
    parse_header(header);
  }
  if (this->http_type == POST) {
    i += 2;
    string data = "";
    cout <<"bufferi " << buffer[i] << endl;
    if (buffer[i] == '{'){
      ++i;
      for (; buffer[i]!='}'; i++){
        data += buffer[i];
      }
    }
    this->query = data;
  }
}

void Request_Obj::parse_start_line(string start_line){
  int start_ind = start_line.find_first_not_of(' ');
  int end_ind = start_line.find_first_of(' ', start_ind);
  string request_type_str = start_line.substr(start_ind, end_ind - start_ind);

  if (request_type_str == "POST"){
    this->http_type = POST;
  } else if (request_type_str == "GET"){
    this->http_type = GET;
  } else if (request_type_str == "PUSH"){
    this->http_type = PUSH;
  } else if (request_type_str == "DELETE"){
    this->http_type = DELETE;
  } else if (request_type_str == "POST"){
    this->http_type = PUT;
  } else {
    cout << "INVALID HTTP REQUEST TYPE" << endl;
  }
  start_ind = start_line.find_first_not_of(' ', end_ind);
  end_ind = start_line.find_first_of(' ', start_ind);

  this->path = start_line.substr(start_ind, end_ind - start_ind);

  start_ind = start_line.find_first_not_of(' ', end_ind);
  end_ind = start_line.find_first_of(" \r\n", start_ind);

  this->http_version = start_line.substr(start_ind, end_ind - start_ind);
}

string Request_Obj::query_database(Database* db){
  cout << "QUERY : " << this->query << endl;
  return db->run_command(this->query);
}
