#include"JSON_elements.h"

JSON_object::JSON_object() {
  contents = unordered_map<string, JSON_element*>();
}

JSON_object::~JSON_object() {
  unordered_map<string, JSON_element*>::iterator itr;
  for (itr=contents.begin(); itr!=contents.end(); ++itr) {
    delete itr->second;
  }
}

JSON_object::JSON_object(string& JSON_string) {
  trim_whitespace(JSON_string);
  JSON_string.erase(0, 1);
  int key_end_ind;
  int element_end_ind;
  while (JSON_string.size() > 0) {
    key_end_ind = JSON_string.find_first_of(":");
    string key = string(JSON_string.begin(), JSON_string.begin()+key_end_ind);
    trim_whitespace(key);
    JSON_string.erase(0, key_end_ind+1);
    element_end_ind = JSON_string.find_first_of("\",[]]{}");
    int element_end_delimiter = JSON_string[element_end_ind];
    if (element_end_delimiter == ',' || element_end_delimiter == '\"') {
      JSON_value* js_val = new JSON_value(JSON_string);
      if (js_val->contents != ""){
        contents[key] = js_val;
      }
      trim_whitespace_left(JSON_string);
      JSON_string.erase(0, 1);
    }
    if (element_end_delimiter == '[') {
      JSON_list* js_list = new JSON_list(JSON_string);
      contents[key] = js_list;
      trim_whitespace_left(JSON_string);
      JSON_string.erase(0,1);
    }
    if (element_end_delimiter == '{') {
      JSON_object* js_obj = new JSON_object(JSON_string);
      contents[key] = js_obj;
      trim_whitespace_left(JSON_string);
      JSON_string.erase(0,1);
    }
    if (element_end_delimiter == '}') {
      JSON_value* js_val = new JSON_value(JSON_string);
      contents[key] = js_val;
      trim_whitespace_left(JSON_string);
      JSON_string.erase(0,1);
      break;
    }
  }
}

string JSON_object::stringify() {
  string JSON_string = "{";
  unordered_map<string, JSON_element*>::iterator itr;
  for (itr=contents.begin(); itr!=contents.end(); ++itr) {
    JSON_string += itr->first + " : ";
    JSON_string += itr->second->stringify();
    JSON_string += ", ";
  }
  JSON_string.pop_back();
  JSON_string.pop_back();
  JSON_string += "}";
  return JSON_string;
}

void JSON_object::add_element(string key, JSON_element* element) {
  contents[key] = element;
}

JSON_element* JSON_object::get_element(string key) {
  return contents[key];
}
