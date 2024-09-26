#include<iostream>
#include<string>
#include"engineTests.h"

using namespace std;

int main(int argc, char **argv){
    cout << "INITIALIZING TESTS" << endl;
    EngineTests* engineTests = new EngineTests();
    try {
        engineTests->testCreate();
        cout << "testCreate PASSED" << endl;
        engineTests->testUpdate();
        cout << "testUpdate PASSED" << endl;
        engineTests->testGetPK();
        cout << "testGetPK PASSED" << endl;
        engineTests->testGetEntry();
        cout << "testGetEntry PASSED" << endl;
        engineTests->conditionalGet();
        cout << "testConditionalGet PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    return 0;
}