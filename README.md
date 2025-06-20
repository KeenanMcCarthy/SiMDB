# SiMDB
  This is an in memory database engine that uses a thread pool running in the background to backup changes to disk.
In the event of a failure, the database can be recovered from the back up, thus ensuring that the data
recovered is as up to date as possible (barring updates lost in the backup queue at the time of failure).
Therefore, end users can benefit from the speed of an in-memory database, and enjoy a significant degree
of persistence and recoverablility.

  In order to run the database first build the executable with the following command:
make -f Makefile

  To run the database in interactive mode, run the executable without any command-line parameters (./SiMDB.exe).
If you would like to run the database in server mode, include the keyword SERVER as the first
parameter. The default port is defined in the config.json file, however the default port may be overriden 
by providing an additional command-line parameter denoting the desired port. (ex. ./SiMDB.exe SERVER 8080)

## The following details the querying language upon which the database operates:

### Create a table and schema:<br />
```
CREATE tablename: column_name size, column_name size, ... ;
Ex: CREATE employees: employeeID 5, first_name 10, last_name 10, department 12, salary 7;
```
The first element of the schema will always serve as the primary key for the table, and the
size of the column denotes the maximum number of characters which a field in that column may have.

### Add to a table:<br />
```
INSERT tablename: value1, value2, ... , valueN;
Ex: INSERT employees: 001, John, Smith, accounting, 80000;
```
### Update a field:<br />
```
UPDATE tablename: Primary_key, column_name, new_value;
EX: UPDATE employees: 001, salary, 85000;
```
### Get all elements in a table:<br />
```
GET tablename: *;
Ex: GET employees: *;
```
### Get an entry by primary key:<br />
```
GET tablename: Primary_key;
Ex: GET employees: 001;
```
### Get specific fields from an entry:<br />
```
GET tablename.field1,field2: identifier;
Ex: GET employees.first_name,last_name: 001;
Ex: GET employees.first_name: *;
```
### Conditional get:<br />
```
GET tablename: * IF column_name1 = value1, OR column_name2 = value2;
Ex: GET employees: * IF first_name = John;
GET employees: * IF salary >= 60000, AND salary <=100000;
```
Equality symbols include =, <, >, >=, <=, !(!=) and supported operations
are OR, and AND

### Conditional get with order of operations logic:<br />
```
GET tablename: * IF column_name1 = value1, OR (IF column_name2 = value2, AND column_name3 = value3);
Ex: GET employees: * IF first_name = John, OR (IF first_name = Ann, AND salary > 90000);
```
### Get elements if in subquery:<br />
```
GET tablename1: * IF column_name1 IN (GET tablename2: *;);
Ex: GET employees: * IF first_name IN (GET overdue_timesheets.first_name: *;);
```
### Sum elements of a query:<br />
```
SUM tablename: * IF field1=value1;
Ex: SUM employees.salary: * IF salary>50000;
```
### Transactions:<br />
```
Begin transaction:
TRANSACTION START;
End transaction:
TRANSACTION END;
```
Only complete transactions will be backed up to disk
### Delete a table:<br />
```
DELETE tablename;
Ex: DELETE employees;
```
Table removal cannot be undone
