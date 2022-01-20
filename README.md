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

Create a table and schema:<br />
CREATE tablename: column_name size, column_name size, ... ;<br />
Ex:</br>
CREATE employees: employeeID 5, first_name 10, last_name 10, department 12, salary 7;<br />
Note: The first element of the schema will always serve as the primary key for the table, and the
size of the column denotes the maximum number of characters which a field in that column may have. 
```
#Add to a table:<br />
INSERT tablename: value1, value2, ... , valueN;<br />
Ex:<br />
INSERT employees: 001, John, Smith, accounting, 80000;<br />
```
```
#Update a field:<br />
UPDATE tablename: Primary_key, column_name, new_value;<br />
Ex:<br />
UPDATE employees: 001, salary, 85000;<br />
```
Get an entry:<br />
GET tablename: Primary_key;<br />
Ex:<br />
GET employees: 001;<br />

Get all elements in a table:<br />
GET tablename: *;<br />
Ex:<br />
GET employees: *;<br />

Get an entry by primary key:<br />
GET tablename: Primary_key;<br />
Ex:<br />
GET employees: 001;

Get specific fields from an entry:<br />
GET tablename.field1,field2: identifier;
Ex:<br />
GET employees.first_name,last_name: 001;
GET employees.first_name: *;

Conditional get:<br />
GET tablename: * IF column_name1 = value1, = column_name2 (eq) value2;<br />
Ex:<br />
GET employees: * IF first_name = John;<br />
GET employees: * IF salary >= 60000, AND salary <=100000;<br />
Note: equality symbols include =, <, >, >=, <=, !(!=) and supported operations
are OR, and AND

Conditional get with order of operations logic:<br />
GET tablename: * IF column_name1 = value1, (OP) (IF column_name2 = value2, (OP) column_name3 = value3);<br />
Ex:<br />
GET employees: * IF first_name = John, OR (IF first_name = Ann, AND salary > 90000);<br />

Get elements if in subquery:<br />
GET tablename1: * IF column_name1 IN (GET tablename2: *;);<br />
Ex:<br />
GET employees: * IF first_name IN (GET overdue_timesheets.first_name: *;); <br />

Sum elements of a query:<br />
SUM tablename: * IF field1=value1;<br />
Ex:<br />
SUM employees.salary: * IF salary>50000;<br />

Transactions:<br />
Begin transaction:<br />
TRANSACTION START;<br />
End transaction:<br />
TRANSACTION END:\;<br />
Note: Only complete transactions will be backed up to disk<br />

Delete a table:<br />
DELETE tablename;<br />
Ex:<br />
DELETE employees;<br />
Note: Table removal cannot be undone

