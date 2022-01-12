# kDB
This is an in memory database that uses threads running in the background to backup changes to disk. 
In the event of a failure, the database can be recovered from the back up, thus ensuring that the data 
recovered is as up to date as possible (barring updates lost in the backup queue at the time of failure).
Therefore, end users can benefit from the speed of an in-memory database, and enjoy a significant degree
of persistence and recoverablility. 

In order to run the database first build the executable with the following command:
make -f Makefile

To run the database in interactive mode, run the executable without any command-line parameters.
If you would like to run the database in server mode, include the keyword SERVER as the first 
parameter. The database runs on port 3490 by default, however the port may be modified by providing
an additional command-line parameter denoting the desired port. (ex. ./a.out SERVER 8080)

The following details the querying language upon which the database operates:

Create a table and schema:
CREATE tablename: column_name size, column_name size, ... ;
Ex:
CREATE employees: employeeID 5, first_name 10, last_name 10, department 12, salary 7;
Note: The first element of the schema will always serve as the primary key for the table, and the
size of the column denotes the maximum number of characters which a field in that column may have. 

Add to a table:
INSERT tablename: value1, value2, ... , valueN;
Ex:
INSERT employees: 001, John, Smith, accounting, 80000;

Update a field:
UPDATE tablename: Primary_key, column_name, new_value;
Ex:
UPDATE employees: 001, salary, 85000;

Get an entry:
GET tablename: Primary_key;
Ex:
GET employees: 001;

Get all elements in a table:
GET tablename: *;
Ex:
GET employees: *;

Conditional get:
GET tablename: * IF column_name1 = value1, = column_name2 (eq) value2;
Ex:
GET employees: * IF first_name = John;
GET employees: * IF salary >= 60000, AND salary <=100000;
Note: equality symbols include =, <, >, >=, <=, !(!=) and supported operations
are OR, and AND

Conditional get with order of operations logic:
GET tablename: * IF column_name1 = value1, (OP) (IF column_name2 = value2, (OP) column_name3 = value3);
Ex:
GET employees: * IF first_name = John, OR (IF first_name = Ann, AND salary > 90000);

Get elements if in subquery:
GET tablename1: * IF column_name1 IN (GET tablename2: *;);
Ex:
GET employees: * IF first_name IN (GET overdue_timesheets: *;); 

Transactions:
Begin transaction:
TRANSACTION START:
End transaction:
TRANSACTION END:
Note: Only complete transactions will be backed up to disk

Delete a table:
DELETE tablename;
Ex:
DELETE employees;
Note: Table removal cannot be undone

