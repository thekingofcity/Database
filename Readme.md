#1 Instruction
##1.1 Open/Create a database
Type the path of the database (Make sure the folder is existed).

Example: C:\database\
###1.1.1 Create a database
Just input the name of the table (name shouldn't contain whitespaces).
###1.1.2 Open a database
Type in the name listed above and then table you choose should be opened.
##1.2 Manipulate table
As there is a so-called interpreter, this program support some basic SQL sentences.

Here are the functions supported.

* SELECT
* WHERE
* INSERT
* UPDATE
* DELETE

###1.2.1 SELECT
At this time, we only support the SELECT *, which means each key is displayed in [key,id,data,remark].

Example: SELECT *
###1.2.2 WHERE
As the original SQL, WHERE can be added after other 4 keywords.

string after data= shuold be added ' before and after.

Example: SELECT WHERE id=10 and data='something'

###1.2.3 INSERT
Example: INSERT (1, 'Hello World', 'via thekingofcity')

###1.2.4 UPDATE
Example: UPDATE (1, 'Hello World', 'via thekingofcity') WHERE data='something'

Note: where is compulsory.

###1.2.5 DELETE
Example: DELETE where id=10

##1.3 Other supported function
###1.3.1 reopen
It acts like restarting the program.
###1.3.2 exit 
###1.3.3 flush
Deprecated
###1.3.4 truncate
Truncate the table.
