#ifndef CONFIG_SINGLETON
#define CONFIG_SINGLETON

#include"JSON_elements.h"
#include<sstream>
#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class Config_Singleton {
private:
  static Config_Singleton* singleton;
  JSON_object* config_values;
  Config_Singleton();
public:
  static Config_Singleton* get_instance();
  JSON_object* get_config_values();
};

#endif
