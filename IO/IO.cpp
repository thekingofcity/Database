// IO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


databaseIO::databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id)
{
	//indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc);
	//valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc);
	availableSpaceFileStream.open(availableSpaceFileName, ios::binary | ios::in | ios::out);
	indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out);
	valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out);
	if (!indexFileStream.is_open()) { indexFileStream.close(); indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!valueFileStream.is_open()) { valueFileStream.close(); valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!availableSpaceFileStream.is_open()) { availableSpaceFileStream.close(); availableSpaceFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	//ios::trunc	Any contents that existed in the file before it is open are discarded.
	//enhanced from https://zhidao.baidu.com/question/146262844.html

	//read from indexFile,valueFile to create b+tree
	unsigned int key, id;
	int p_int;
	streampos p;
	dataBPTtype dataBPTtypeTmp;
	dataBPTtype_id dataBPTtype_idTmp;
	while (indexFileStream.peek() != EOF) {
		dataBPTtypeTmp.indexPos = indexFileStream.tellp();
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		//Here to add index to bpt.
		//Something like (key indexPos valuePos).
		if (key == 4294967295 && p_int == -1) {
			p = UNINTSIZE + INTSIZE;
			indexFileStream.seekg(-p, ios::cur);
			p = indexFileStream.tellp();
			p_int = p;
			availableSpaceIndex.push_back(p_int);
			p = UNINTSIZE + INTSIZE;//fix the first index was -1 then indexFileStream.seekg(-p, ios::cur) goes wrong.
			indexFileStream.seekg(p, ios::cur);
		}
		else {
			p = p_int;
			valueFileStream.seekg(p, ios::beg);
			valueFileStream.read((char *)(&id), UNINTSIZE);
			dataBPTtype_idTmp.id = id;
			dataBPTtype_idTmp.key = key;
			dataBPT_id.push_back(dataBPTtype_idTmp);
			dataBPTtypeTmp.key = key;
			dataBPTtypeTmp.valuePos = p_int;
			dataBPT.push_back(dataBPTtypeTmp);
		}
	}
	indexFileStream.clear();//Just in case.

	//create availableSpace table for valueFile
	availableSpaceFileName_ = availableSpaceFileName;
	int availableSpaceTmp;
	while (availableSpaceFileStream.peek() != EOF) {
		availableSpaceFileStream.read((char *)(&availableSpaceTmp), INTSIZE);
		availableSpace.push_back(availableSpaceTmp);
	}
	availableSpaceFileStream.close();
}

databaseIO::~databaseIO()
{
	flush();
	indexFileStream.close();
	valueFileStream.close();
	availableSpaceFileStream.open(availableSpaceFileName_, ios::binary | ios::in | ios::out | ios::trunc);
	int availableSpaceTmp, i, j;
	j = availableSpace.size();
	for (i = 0; i < j; i++) {
		availableSpaceTmp = availableSpace.at(availableSpace.size() - 1);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();
}

void databaseIO::reopen(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype>& dataBPT, vector<dataBPTtype_id>& dataBPT_id)
{
	flush();
	indexFileStream.close();
	valueFileStream.close();
	availableSpaceFileStream.open(availableSpaceFileName_, ios::binary | ios::in | ios::out | ios::trunc);
	int availableSpaceTmp, i, j;
	j = availableSpace.size();
	for (i = 0; i < j; i++) {
		availableSpaceTmp = availableSpace.at(availableSpace.size() - 1);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();


	//indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc);
	//valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc);
	availableSpaceFileStream.open(availableSpaceFileName, ios::binary | ios::in | ios::out);
	indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out);
	valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out);
	if (!indexFileStream.is_open()) { indexFileStream.close(); indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!valueFileStream.is_open()) { valueFileStream.close(); valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!availableSpaceFileStream.is_open()) { availableSpaceFileStream.close(); availableSpaceFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	//ios::trunc	Any contents that existed in the file before it is open are discarded.
	//enhanced from https://zhidao.baidu.com/question/146262844.html

	//read from indexFile,valueFile to create b+tree
	unsigned int key, id;
	int p_int;
	streampos p;
	dataBPTtype dataBPTtypeTmp;
	dataBPTtype_id dataBPTtype_idTmp;
	while (indexFileStream.peek() != EOF) {
		dataBPTtypeTmp.indexPos = indexFileStream.tellp();
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		//Here to add index to bpt.
		//Something like (key indexPos valuePos).
		if (key == 4294967295 && p_int == -1) {
			p = UNINTSIZE + INTSIZE;
			indexFileStream.seekg(-p, ios::cur);
			p = indexFileStream.tellp();
			p_int = p;
			availableSpaceIndex.push_back(p_int);
			p = UNINTSIZE + INTSIZE;//fix the first index was -1 then indexFileStream.seekg(-p, ios::cur) goes wrong.
			indexFileStream.seekg(p, ios::cur);
		}
		else {
			p = p_int;
			valueFileStream.seekg(p, ios::beg);
			valueFileStream.read((char *)(&id), UNINTSIZE);
			dataBPTtype_idTmp.id = id;
			dataBPTtype_idTmp.key = key;
			dataBPT_id.push_back(dataBPTtype_idTmp);
			dataBPTtypeTmp.key = key;
			dataBPTtypeTmp.valuePos = p_int;
			dataBPT.push_back(dataBPTtypeTmp);
		}
	}
	indexFileStream.clear();//Just in case.

							//create availableSpace table for valueFile
	availableSpaceFileName_ = availableSpaceFileName;
	while (availableSpaceFileStream.peek() != EOF) {
		availableSpaceFileStream.read((char *)(&availableSpaceTmp), INTSIZE);
		availableSpace.push_back(availableSpaceTmp);
	}
	availableSpaceFileStream.close();
}

void databaseIO::write() {
	int num = 30;
	indexFileStream.write((char *)(&num), sizeof(int));
	num--;
	indexFileStream.write((char *)(&num), sizeof(int));
	num--;
	indexFileStream.write((char *)(&num), sizeof(int));
}

void databaseIO::readALL()
{
	indexFileStream.clear();
	valueFileStream.clear();
	indexFileStream.seekp(0, ios::beg);
	valueFileStream.seekp(0, ios::beg);
	datatype data;
	unsigned int key;
	int p_int;
	streampos p;
	while (indexFileStream.peek() != EOF && valueFileStream.peek() != EOF) {
		//the method whether file hits the end is from http://bbs.csdn.net/topics/210052379
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		if (key != -1 && p_int != -1) {
			p = p_int;
			valueFileStream.seekp(p, ios::beg);
			valueFileStream.read((char *)(&data), DATATYPESIZE);
			printf_s("key     =%3u\n", key);
			printf_s("id      =%3u\n", data.id);
			printf_s("data    =%s\n", data.data);
			printf_s("remark  =%s\n", data.remark);
		}
	}
	cout << "That's the end of value.dat" << endl;
	cout << endl;
}

dataBPTtype databaseIO::insert(unsigned int key, datatype &data)
{
	int p_int;
	dataBPTtype dataBPTTmp;
	dataBPTTmp.key = key;
	indexFileStream.clear();
	valueFileStream.clear();
	if (availableSpace.size() != 0) {
		p_int = availableSpace.at(availableSpace.size() - 1);
		streampos p = p_int;
		dataBPTTmp.valuePos = p_int;
		valueFileStream.seekg(p, ios::beg);
		valueFileStream.write((char *)(&data), DATATYPESIZE);
		availableSpace.pop_back();
	}
	else {
		valueFileStream.seekg(0, ios::end);
		dataBPTTmp.valuePos = valueFileStream.tellg();
		p_int = valueFileStream.tellg();
		valueFileStream.write((char *)(&data), DATATYPESIZE);
	}
	if (availableSpaceIndex.size() != 0) {
		streampos p = availableSpaceIndex.at(availableSpaceIndex.size() - 1);
		availableSpaceIndex.pop_back();
		indexFileStream.seekg(p, ios::beg);
		dataBPTTmp.indexPos = indexFileStream.tellg();
		indexFileStream.write((char *)(&key), UNINTSIZE);
		indexFileStream.write((char *)(&p_int), INTSIZE);
	}
	else {
		indexFileStream.seekg(0, ios::end);
		dataBPTTmp.indexPos = indexFileStream.tellg();
		indexFileStream.write((char *)(&key), UNINTSIZE);
		indexFileStream.write((char *)(&p_int), INTSIZE);
	}
	return dataBPTTmp;
}

void databaseIO::get(dataBPTtype & pos, datatype &datatypeTmp)
{
	streampos p = pos.valuePos;
	valueFileStream.clear();
	valueFileStream.seekp(p, ios::beg);
	valueFileStream.read((char *)(&datatypeTmp), DATATYPESIZE);
}

void databaseIO::remove(dataBPTtype &pos)
{
	streampos p = pos.indexPos;
	int tmp = -1, p_int = p;
	availableSpaceIndex.push_back(p_int);
	indexFileStream.clear();
	indexFileStream.seekg(p, ios::beg);
	indexFileStream.write((char *)(&tmp), UNINTSIZE);
	indexFileStream.write((char *)(&tmp), INTSIZE);
	availableSpace.push_back(pos.valuePos);
}

void databaseIO::modify(dataBPTtype &pos, datatype &data)
{
	streampos p = pos.valuePos;
	valueFileStream.clear();
	valueFileStream.seekp(p, ios::beg);
	valueFileStream.write((char *)(&data), DATATYPESIZE);
}

void databaseIO::flush()
{
	indexFileStream.flush();
	valueFileStream.flush();
	availableSpaceFileStream.open(availableSpaceFileName_, ios::binary | ios::in | ios::out | ios::trunc);
	int availableSpaceTmp, i, j;
	j = availableSpace.size();
	for (i = 0; i < j; i++) {
		availableSpaceTmp = availableSpace.at(availableSpace.size() - 1);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();
}

int conversion(datatype &data, unsigned int id, string datastr, string remark) {
	if (datastr.size() >= DATASIZE || remark.size() >= REMARKSIZE) {
		return 0;
	}
	else {
		data.id = id;
		int i, j = datastr.size();
		for (i = 0; i <= j; i++) {
			data.data[i] = datastr[i];
		}
		data.data[i] = '\0';
		j = remark.size();
		for (i = 0; i <= j; i++) {
			data.remark[i] = remark[i];
		}
		data.remark[i] = '\0';
		return 1;
	}
}

bool sortByKey(const dataBPTtype &v1, const dataBPTtype &v2) {
	return v1.key < v2.key;//Ascend
}

dataBPTtype fetch(vector<dataBPTtype> &dataBPT, unsigned int key) {
	int i, j;
	j = dataBPT.size();
	for (i = 0; i < j; i++) {
		if (dataBPT.at(i).key == key) {
			dataBPTtype dataBPTtypeTmp = dataBPT.at(i);
			dataBPT.erase(dataBPT.begin() + i);
			//erase form http://www.cplusplus.com/reference/vector/vector/erase/
			return dataBPTtypeTmp;
			//why not use iterator -> can't return iterator
			//article below still can't fix it
			//http://www.cnblogs.com/wang7/archive/2012/04/27/2474138.html
		}
	}
	dataBPTtype return0 = { -1,-1,-1 };
	return return0;
}

dataBPTtype fetchById(vector<dataBPTtype_id> &dataBPT_id, unsigned int id, vector<dataBPTtype> &dataBPT) {
	int i, j, key;
	//Method of iterator from http://blog.csdn.net/dgyanyong/article/details/21268469
	vector<dataBPTtype_id>::iterator itor;
	vector<dataBPTtype_id>::iterator itor2;
	j = dataBPT_id.size();
	for (itor = dataBPT_id.begin(); itor != dataBPT_id.end();) {
		if (itor->id == id) {
			key = itor->key;
			itor2 = itor;
			itor = dataBPT_id.erase(itor);
			return fetch(dataBPT, key);
		}
		else {
			itor++;
		}
	}
	dataBPTtype return0 = { -1,-1,-1 };
	return return0;
}

void insert(datatype &data, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id, databaseIO &db) {
	int key;
	dataBPTtype_id dataBPT_idTmp;
	if (dataBPT.size() == 0) {
		key = 0;
	}
	else {
		sort(dataBPT.begin(), dataBPT.end(), sortByKey);
		key = dataBPT.at(dataBPT.size() - 1).key + 1;
	}
	dataBPT_idTmp.id = data.id;
	dataBPT_idTmp.key = key;
	dataBPT_id.push_back(dataBPT_idTmp);
	dataBPT.push_back(db.insert(key, data));
}

void modify(datatype &data, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id, databaseIO &db) {
	dataBPTtype dataBPTTmp = fetchById(dataBPT_id, data.id, dataBPT);
	if (dataBPTTmp.key == -1) {
		printf_s("Can't find id=%3d.\n", data.id);
	}
	else {
		db.modify(dataBPTTmp, data);
		printf_s("Successfully modified id=%3d from database.\n", data.id);
	}
}

void remove(int id, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id, databaseIO &db) {
	dataBPTtype dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
	if (dataBPTTmp.key == -1) {
		printf_s("Can't find id=%d.\n",id);
	}
	else {
		db.remove(dataBPTTmp);
		printf_s("Successfully removed id=%d from database.\n",id);
	}
}

void get(int id, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id, databaseIO &db) {
	dataBPTtype dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
	if (dataBPTTmp.key == -1) {
		printf_s("Can't find id.\n");
	}
	else {
		datatype datatypeTmp;
		db.get(dataBPTTmp, datatypeTmp);
		printf_s("key     =%3u\n", dataBPTTmp.key);
		printf_s("id      =%3u\n", datatypeTmp.id);
		printf_s("data    =%s\n", datatypeTmp.data);
		printf_s("remark  =%s\n", datatypeTmp.remark);
	}
}

void reopen(vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id, databaseIO &db) {
	//Time from http://blog.csdn.net/wangqing_12345/article/details/52092728
	int stampTime = 0;
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	Times standard;
	//Traverse the folder from http://www.cnblogs.com/fnlingnzb-learner/p/6424563.html
	const char *to_search = "C:\\database\\*.index";        //欲查找的文件，支持通配符

	long handle;                                    //用于查找的句柄
	struct _finddata_t fileinfo;                    //文件信息的结构体
	handle = _findfirst(to_search, &fileinfo);      //第一次查找
	if (-1 == handle) return;
	printf_s("%s   ", fileinfo.name);				//打印出找到的文件的文件名
	tick = fileinfo.time_access;
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	printf("Last modified on: %s\n", s);
	while (!_findnext(handle, &fileinfo));			//循环查找其他符合的文件，直到找不到其他的为止
	{
		printf_s("%s   ", fileinfo.name);
		tick = fileinfo.time_access;
		tm = *localtime(&tick);
		strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
		printf("Last modified on: %s\n", s);
	}
	_findclose(handle);                              //别忘了关闭句柄

	char table[40];
	printf_s("Input the table name: "); scanf_s("%s", &table, 40);
	string root = "C:/database/";
	int strsize = root.size();
	string indexFileName = root.append(table); indexFileName.append(".index"); root.erase(strsize);
	string valueFileName = root.append(table); valueFileName.append(".value"); root.erase(strsize);
	string availableSpaceFileName = root.append(table); availableSpaceFileName.append(".space");
	db.reopen(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	printf_s("table %s selected successfully.\n", table);
}

int main()
{
	vector<dataBPTtype> dataBPT;//b+tree key->indexPos,valuePos
	vector<dataBPTtype_id> dataBPT_id;//b+tree id->key

	//Time from http://blog.csdn.net/wangqing_12345/article/details/52092728
	int stampTime = 0;
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char timestr[100];
	Times standard;
	//Traverse the folder from http://www.cnblogs.com/fnlingnzb-learner/p/6424563.html
	const char *to_search = "C:\\database\\*.index";        //欲查找的文件，支持通配符

	long handle;                                    //用于查找的句柄
	struct _finddata_t fileinfo;                    //文件信息的结构体
	handle = _findfirst(to_search, &fileinfo);      //第一次查找
	if (-1 == handle) return -1;
	printf_s("%s   ", fileinfo.name);				//打印出找到的文件的文件名
	tick = fileinfo.time_access;
	tm = *localtime(&tick);
	strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm);
	printf("Last modified on: %s\n", timestr);
	while (!_findnext(handle, &fileinfo));			//循环查找其他符合的文件，直到找不到其他的为止
	{
		printf_s("%s   ", fileinfo.name);
		tick = fileinfo.time_access;
		tm = *localtime(&tick);
		strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm);
		printf("Last modified on: %s\n", timestr);
	}
	_findclose(handle);                              //别忘了关闭句柄

	char table[40];
	printf_s("Input the table name: "); scanf_s("%s", &table, 40);
	string root = "C:/database/";
	int strsize = root.size();
	string indexFileName = root.append(table); indexFileName.append(".index"); root.erase(strsize);
	string valueFileName = root.append(table); valueFileName.append(".value"); root.erase(strsize);
	string availableSpaceFileName = root.append(table); availableSpaceFileName.append(".space");
	//databaseIO db(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	class databaseIO *db = new databaseIO(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	printf_s("table %s selected successfully.\n",table);

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
			insert(data, dataBPT, dataBPT_id, *db);
			break;
		case 2:
			cout << "Please input id." << endl;
			cin >> data.id;
			cout << "Please input data." << endl;
			scanf_s("%s", &data.data, DATASIZE);
			cout << "Please input remark." << endl;
			scanf_s("%s", &data.remark, REMARKSIZE);
			modify(data, dataBPT, dataBPT_id, *db);
			break;
		case 3:
			cout << "Please input id." << endl;
			cin >> id;
			remove(id, dataBPT, dataBPT_id, *db);
			break;
		case 4:
			db->flush();
			break;
		case 5:
			db->readALL();
			break;
		case 6:
			cout << "Please input id." << endl;
			cin >> id;
			get(id, dataBPT, dataBPT_id, *db);
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
				insert(data, dataBPT, dataBPT_id, *db);
			}
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed in %fs.\n", tC);
			break;
		case 8:
			start = clock();
			for (int i = 1; i < 20; i++) {
				id = rand() % 20;
				remove(id, dataBPT, dataBPT_id, *db);
			}
			end = clock();
			tC = double(end - start) / CLOCKS_PER_SEC;
			printf_s("Command completed in %lf s.\n", tC);
		case 9:
		{
			reopen(dataBPT, dataBPT_id, *db);
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
	delete db;
	return 0;
}

