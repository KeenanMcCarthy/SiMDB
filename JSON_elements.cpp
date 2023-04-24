#include"JSON_elements.h"

void JSON_element::trim_whitespace_left(string& str) {
  str.erase(str.begin(), str.begin()+str.find_first_not_of(" \n\r\t"));
}

void JSON_element::trim_whitespace_right(string& str) {
    str.erase(str.find_last_not_of(" \n\r\t")+1);
}

void JSON_element::trim_whitespace(string& str){
  trim_whitespace_left(str);
  trim_whitespace_right(str);
}
