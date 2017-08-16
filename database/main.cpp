#include "stdafx.h"
//#include "databaseIO.h"
#include "database.h"
#include "time.h"
#include <random>//test unit

int main()
{
	vector<dataBPTtype> dataBPT;//b+tree key->indexPos,valuePos
	vector<dataBPTtype_id> dataBPT_id;//b+tree id->key

	cout << "Input the path of database(add \ at the end pls):" << endl;
	string databasePath;
	cin >> databasePath;
	database DB(databasePath);

	char s[27] = "abcdefghijklmnopqrstuvwxyz";
	datatype data;
	time_t start, end;
	double tC;
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
			DB.insert(data, dataBPT, dataBPT_id);
			break;
		case 2:
			cout << "Please input id." << endl;
			cin >> data.id;
			cout << "Please input data." << endl;
			scanf_s("%s", &data.data, DATASIZE);
			cout << "Please input remark." << endl;
			scanf_s("%s", &data.remark, REMARKSIZE);
			DB.modify(data, dataBPT, dataBPT_id);
			break;
		case 3:
			cout << "Please input id." << endl;
			cin >> id;
			DB.remove(id, dataBPT, dataBPT_id);
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
			for (int i = 1; i < 200; i++) {
				data.id = i;
				data.data[0] = s[rand() % 26];
				data.data[1] = s[rand() % 26];
				data.data[2] = s[rand() % 26];
				data.data[3] = s[rand() % 26];
				data.data[4] = '\0';
				data.remark[0] = s[rand() % 26];
				data.remark[1] = s[rand() % 26];
				data.remark[2] = '\0';
				DB.insert(data, dataBPT, dataBPT_id);
			}
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed in %fs.\n", tC);
			break;
		case 8:
			start = clock();
			for (int i = 1; i < 20; i++) {
				id = rand() % 20;
				DB.remove(id, dataBPT, dataBPT_id);
			}
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed in %lf s.\n", tC);
			break;
		case 9:
		{
			cout << "Input the path of database(add \ at the end pls):" << endl;
			string databasePath;
			cin >> databasePath;
			DB.reopen(databasePath);
			break;
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
