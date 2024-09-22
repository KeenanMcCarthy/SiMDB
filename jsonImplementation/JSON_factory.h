#ifndef JSON_FACTORY_H
#define JSON_FACTORY_H

#include"JSON_elements.h"

using namspace std;

class JSON_factory{
public:
  string json_string;
  JSON_constructor(string json_string);
  JSON_element* construct_JSON(string json_string);
}

#endif
