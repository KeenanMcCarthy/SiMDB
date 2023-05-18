#include"request_obj.h"
#include"JSON_elements.h"

void Request_Obj::parse_header(string header) {
  int start_ind = header.find_first_not_of(' ');
  int end_ind = header.find_first_of(':');
  string header_key = header.substr(start_ind, end_ind - start_ind);
  header_key.erase(header_key.find_last_not_of(" \n\r\t")+1);
  start_ind = header.find_first_not_of(' ', end_ind+1);
  end_ind = header.find_first_of(" \r\n", start_ind);
  string header_val = header.substr(start_ind, end_ind - start_ind);
  header_val.erase(header_val.find_last_not_of(" \n\r\t")+1);
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
    for (; i<buffer_size && buffer[i] != '\0'; i++){
        data += buffer[i];
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

void Request_Obj::add_response_code(string response) {
  int colon_ind = response.find_first_of(':');
  if (colon_ind != -1){
    string response_start = response.substr(0, colon_ind);
    response_start.erase(response_start.begin(), response_start.begin()+response_start.find_first_not_of(" \n\r\t"));
    response_start.erase(response_start.find_last_not_of(" \n\r\t")+1);
    if (response_start == "ERROR") {
      response_headers["400"] = response.substr(colon_ind+1, response.size());
      return;
    }
  }
  response_headers["200"] = " OK";
}

string Request_Obj::query_database(Database* db){
  if (this->path != "/" +db->config->get_element("ContextRoot")->stringify()) {
    return "ERROR: Invalid endpoint";
  }
  string to_query = this->query;
  if (headers["Content-Type"] == "application/json") {
    JSON_object* jo = new JSON_object(this->query);
    to_query = jo->get_element("query")->stringify();
  }
  string response = db->run_command(to_query);
  if (headers["Accept"] == "application/json") {
    add_response_code(response);
    JSON_object* json_response = new JSON_object();
    JSON_value* query_val = new JSON_value();
    query_val->set_value(to_query);
    JSON_value* response_val = new JSON_value();
    response_val->set_value(response);
    json_response->add_element("query", query_val);
    json_response->add_element("response", response_val);
    return json_response->stringify();
  }
  return response;
}
