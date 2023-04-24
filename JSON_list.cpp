#include"JSON_elements.h"
#include<iostream>

using namespace std;

JSON_list::JSON_list(){
  this->contents = vector<JSON_element*>();
}

JSON_list::~JSON_list(){
  for (JSON_element* ele : this->contents) {
    delete ele;
  }
}

JSON_list::JSON_list(string& JSON_string) {
  trim_whitespace(JSON_string);
  JSON_string.erase(0, 1);
  int element_end_ind;
  while (JSON_string.size() > 0) {
    element_end_ind = JSON_string.find_first_of(",[]]{");
    int element_end_delimiter = JSON_string[element_end_ind];
    if (element_end_delimiter == ',') {
      JSON_value* js_val = new JSON_value(JSON_string);
      if (js_val->contents != ""){
        contents.push_back(js_val);
      }
      JSON_string.erase(0, 1);
    }
    if (element_end_delimiter == '[') {
      JSON_list* js_list = new JSON_list(JSON_string);
      contents.push_back(js_list);
      trim_whitespace_left(JSON_string);
      JSON_string.erase(0,1);
    }
    if (element_end_delimiter == '{') {
      JSON_object* js_obj = new JSON_object(JSON_string);
      contents.push_back(js_obj);
      trim_whitespace_left(JSON_string);
      JSON_string.erase(0,1);
    }
    if (element_end_delimiter == ']') {
      JSON_value* js_val = new JSON_value(JSON_string);
      if (js_val->contents != ""){
        contents.push_back(js_val);
      }
      JSON_string.erase(0, 1);
      break;
    }
  }
}

string JSON_list::stringify() {
  string list_string = "[";
  for (int i=0; i<this->contents.size(); i++) {
    list_string += contents[i]->stringify() + " , ";
  }
  list_string.pop_back();
  list_string.pop_back();
  list_string.pop_back();
  return list_string + "]";
}

void JSON_list::add_element(JSON_element* element){
  contents.push_back(element);
}

JSON_element* JSON_list::get_element_by_index(int ind) {
  return contents[ind];
}
