#include "stdafx.h"
//#include "databaseIO.h"
#include "database.h"
#include "time.h"
#include <random>//test unit

unsigned int bkdr_hash(const char* key)
{
	//bkdr_hash from http://blog.csdn.net/wanglx_/article/details/40400693
	char* str = const_cast<char*>(key);

	unsigned int seed = 31; // 31 131 1313 13131 131313 etc.. 37  
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	return hash;
}

int main()
{
	cout << "Input the path of database(like C:\\database\\):";
	string databasePath;
	cin >> databasePath;
	//databasePath = "C:\\database\\";
	database DB(databasePath);
	getchar();

	datatype data;
	time_t start, end;
	double tC;
	string command = "";
	while (command != "exit") {
		cout << "Input the Command: ";
		getline(cin, command);
		if (command == "") continue;
		start = clock();
		if (DB.execute(command)) {
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed sucessfully in %lf s.\n", tC);
		}
		else {
			printf_s("Syntax error.\n\n");
		}
		//select * where id=3 or data=xfir
		//select * where id=3 and data=rtkj

	}
	printf_s("Bye.");
	getchar();
	return 0;
}
