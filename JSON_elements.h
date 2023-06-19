#ifndef JSON_ELEMENTS_H
#define JSON_ELEMENTS_H

#include<string>
#include<unordered_map>
#include<vector>
#include<iostream>

using namespace std;

class JSON_element {
public:
  virtual string stringify() = 0;
  virtual ~JSON_element() {};
  void trim_whitespace_left(string& str);
  void trim_whitespace_right(string& str);
  void trim_whitespace(string& str);
};

class JSON_value: public JSON_element {
public:
  string contents;
  JSON_value();
  string stringify();
  JSON_value(string& JSON_string);
  void set_value(string contents);
};

class JSON_list: public JSON_element {
public:
  vector<JSON_element*> contents;
  string stringify();
  JSON_list();
  ~JSON_list();
  JSON_list(string& JSON_string);
  void add_element(JSON_element* element);
  JSON_element* get_element_by_index(int ind);
};

class JSON_object: public JSON_element{
public:
  unordered_map<string, JSON_element*> contents;
  string stringify();
  JSON_object();
  ~JSON_object();
  JSON_object(string& JSON_object);
  void add_element(string key, JSON_element* element);
  JSON_element* get_element(string key);
  void remove_quotes(string& str);
};

#endif
