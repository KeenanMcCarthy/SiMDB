#include"config_singleton.h"

Config_Singleton* Config_Singleton::singleton = nullptr;

Config_Singleton::Config_Singleton(){
  std::ifstream t("config.json");
  std::stringstream buffer;
  buffer << t.rdbuf();
  string buffer_str = buffer.str();
  this->config_values = new JSON_object(buffer_str);
}

Config_Singleton* Config_Singleton::get_instance(){
  if (Config_Singleton::singleton == nullptr) {
       Config_Singleton::singleton = new Config_Singleton();
   }
   return Config_Singleton::singleton;
}

JSON_object* Config_Singleton::get_config_values() {
  return this->config_values;
}
