#include"JSON_elements.h"

JSON_value::JSON_value() {
  contents = "";
}

JSON_value::JSON_value(string& JSON_string) {
  trim_whitespace_left(JSON_string);
  JSON_string.erase(0,1);
  int value_end = JSON_string.find_first_of("\"");
  string json_value = JSON_string.substr(0, value_end);
  trim_whitespace(json_value);
  contents = json_value;
  JSON_string.erase(0, value_end+1);
}

string JSON_value::stringify(){
  return contents;
}

void JSON_value::set_value(string contents) {
  this->contents = contents;
}
