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
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    try {
        engineTests->testUpdate();
        cout << "testUpdate PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    try {
        engineTests->testGetPK();
        cout << "testGetPK PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    try {
        engineTests->testGetEntry();
        cout << "testGetEntry PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    try {
        engineTests->conditionalGet();
        cout << "testConditionalGet PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    try {
        engineTests->conditionalGetWithOOP();
        cout << "testConditionalGetWithOOP PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    /*
    * TODO: refactor flaky test
    try {
        engineTests->getSubquery();
        cout << "getSubquery PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }*/
    try {
        engineTests->sumElements();
        cout << "sumElements PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    try {
        engineTests->transactionTest();
        cout << "transactionTest PASSED" << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    return 0;
}
