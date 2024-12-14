#include"engineTests.h"

void EngineTests::assertEqual(string testName, string expected, string actual){
    if (expected != actual) {
        throw invalid_argument("Failed " + testName + ":\nexpected: " + expected + " actual value: " + actual);
    }
}

void EngineTests::testCreate(){
    Database* db = new Database();
    db->run_command("CREATE testTable: name 5, age 3;");
    db->run_command("INSERT testTable: Bill, 47;");
    string result = db->run_command("GET testTable: *;");
    db->run_command("DELETE testTable;");
    assertEqual("testCreate","Bill,47\n",result);
}

void EngineTests::testUpdate(){
    Database* db = new Database();
    db->run_command("CREATE employees: id 5, name 10, salary 7;");
    db->run_command("INSERT employees: 001, Bill, 80000;");
    db->run_command("UPDATE employees: 001, salary, 85000;");
    string result = db->run_command("GET employees: *;");
    db->run_command("DELETE employees;");
    assertEqual("testUpdate","001,Bill,85000\n",result);
}

void EngineTests::testGetPK(){
    Database* db = new Database();
    db->run_command("CREATE employees: id 5, name 10, salary 7;");
    db->run_command("INSERT employees: 001, Bill, 85000;");
    string result = db->run_command("GET employees: 001;");
    db->run_command("DELETE employees;");
    assertEqual("testGetPK","001,Bill,85000\n",result);
}

void EngineTests::testGetEntry(){
    Database* db = new Database();
    db->run_command("CREATE employees: id 5, name 10, salary 7;");
    db->run_command("INSERT employees: 001, Bill, 85000;");
    string result = db->run_command("GET employees.salary: 001;");
    db->run_command("DELETE employees;");
    assertEqual("testGetEntry","85000\n",result);
}

void EngineTests::conditionalGet(){
    Database* db = new Database();
    db->run_command("CREATE employees: id 5, name 10, salary 7;");
    db->run_command("INSERT employees: 001, Bill, 85000;");
    db->run_command("INSERT employees: 003, Jan, 75000;");
    string result = db->run_command("GET employees: * IF salary > 80000;");
    db->run_command("DELETE employees;");
    assertEqual("conditionalGet","001,Bill,85000\n",result);
}

void EngineTests::conditionalGetWithOOP(){
    Database* db = new Database();
    db->run_command("CREATE employees: id 5, name 10, salary 7;");
    db->run_command("INSERT employees: 001, Bill, 85000;");
    db->run_command("INSERT employees: 002, Jan, 75000;");
    db->run_command("INSERT employees: 003, John, 95000;");
    db->run_command("INSERT employees: 004, Jan, 65000;");
    string result = db->run_command("GET employees: * IF salary > 90000 OR (IF name = Jan, AND salary > 70000);");
    db->run_command("DELETE employees;");
    assertEqual("conditionalGetWithOOP","003,John,95000\n002,Jan,75000\n",result);
}

void EngineTests::getSubquery(){
    Database* db = new Database();
    db->run_command("CREATE employees: id 5, name 10, salary 7;");
    db->run_command("CREATE overdue_timesheets: id 5, employeeID 5");
    db->run_command("INSERT employees: 001, Bill, 85000;");
    db->run_command("INSERT employees: 002, Jan, 75000;");
    db->run_command("INSERT overdue_timesheets: 001, 002;");
    string result = db->run_command("GET employees: * IF id IN (GET overdue_timesheets.id: *;);");
    db->run_command("DELETE employees;");
    db->run_command("DELETE overdue_timesheets;");
    assertEqual("getSubquery","002,Jan,75000\n",result);
}
