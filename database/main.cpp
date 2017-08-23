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
	vector<dataBPTtype> dataBPT;//b+tree key->indexPos,valuePos
	vector<dataBPTtype_id> dataBPT_id;//b+tree id->key

	//cout << "Input the path of database(like C:\\database\\):" << endl;
	string databasePath;
	//cin >> databasePath;
	databasePath = "C:\\database\\";
	database DB(databasePath);

	char s[27] = "abcdefghijklmnopqrstuvwxyz";
	datatype data;
	time_t start, end;
	double tC;
	char searchTmp[DATASIZE];
	unsigned int key;
	int tmp = 0, id = 0;
	cout << "1 for insert 2 for modify 3 for remove" << endl << "4 for flush 5 for read 6 for get." << endl;
	cout << "Input operation << ";
	cin >> tmp;
	while (tmp != 0) {
		switch (tmp) {
		case 1:
			//get key after sort
			//key.auto-incresement=true
			cout << "Please input id." << endl;
			cin >> data.id;
			cout << "Please input data." << endl;
			scanf_s("%s", &data.data, DATASIZE);
			cout << "Please input remark." << endl;
			scanf_s("%s", &data.remark, REMARKSIZE);
			//DB.insert(data, dataBPT, dataBPT_id);
			DB.insert(data);
			break;
		case 2:
			cout << "Please input id." << endl;
			cin >> data.id;
			DB.get(data.id);
			cout << "Please input key." << endl;
			cin >> key;
			cout << "Please input data." << endl;
			scanf_s("%s", &data.data, DATASIZE);
			cout << "Please input remark." << endl;
			scanf_s("%s", &data.remark, REMARKSIZE);
			//DB.modify(data, dataBPT, dataBPT_id);
			DB.modify(data, key);
			break;
		case 3:
			//cout << "Please input id." << endl;
			//cin >> id;
			//DB.get(id);
			//cout << "Please input data." << endl;
			//cin >> searchTmp;
			//DB.get(searchTmp);
			cout << "Please input key to remove." << endl;
			cin >> key;
			//DB.remove(id, dataBPT, dataBPT_id);
			DB.remove(key);
			break;
		case 4:
			DB.flush();
			break;
		case 5:
			DB.readALL();
			break;
		case 6:
			cout << "Please input id." << endl;
			cin >> id;
			DB.get(id, dataBPT, dataBPT_id);
			break;
		case 7:
			start = clock();
			for (int i = 1; i < 10; i++) {
				data.id = rand() % 3 + 1;
				data.data[0] = s[rand() % 26];
				data.data[1] = s[rand() % 26];
				data.data[2] = s[rand() % 26];
				data.data[3] = s[rand() % 26];
				data.data[4] = '\0';
				data.remark[0] = s[rand() % 26];
				data.remark[1] = s[rand() % 26];
				data.remark[2] = '\0';
				//DB.insert(data, dataBPT, dataBPT_id);
				DB.insert(data);
			}
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed in %fs.\n", tC);
			break;
		case 8:
			int key;
			start = clock();
			for (int i = 1; i < 20; i++) {
				key = rand() % 20;
				//DB.remove(id, dataBPT, dataBPT_id);
				DB.remove(key);
			}
			//for (int i = 1; i < 4; i++) {
			//	DB.get(i);
			//	cout << endl;
			//}
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed in %lf s.\n", tC);
			break;
		case 9:
		{
			cout << "Input the path of database(like C:\\database\\): ";
			string databasePath;
			cin >> databasePath;
			DB.reopen(databasePath);
			break;
		}
		case 10:
		{
			getchar();
			string command;
			cout << "Input the Command: ";
			getline(cin, command);
			if (DB.execute(command)) {
				printf_s("Command complete sucessfully.\n");
			}
			else {
				printf_s("Syntax error.\n");
			}
			break;
			//select * where id=3 or data=xfir
		}
		default:
			printf_s("Syntax error.\n");
			break;
		}
		cout << "1 for insert 2 for modify 3 for remove" << endl << "4 for flush 5 for read 6 for get." << endl;
		cout << "Input operation << ";
		cin >> tmp;
	}
	getchar();
	return 0;
}
