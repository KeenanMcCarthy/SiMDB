#ifndef ENGINE_TEST_H
#define ENGINE_TEST_H

#include<iostream>
#include"database.h"

using namespace std;

class EngineTests{
public:
    void assertEqual(string testName, string expected, string actual);
    void testCreate();
    void testUpdate();
    void testGetPK();
    void testGetEntry();
    void conditionalGet();
    void conditionalGetWithOOP();
    void getSubquery();
    void sumElements();
};

#endif
