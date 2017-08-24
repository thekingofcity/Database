// IO.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "databaseIO.h"
#include "database.h"

database::database(string databasePath)
{
	listTable(databasePath);
	int strsize = databasePath.size();
	char table[40];
	printf_s("Input the table name: "); scanf_s("%s", &table, 40);
	string indexFileName = databasePath.append(table); indexFileName.append(".index"); databasePath.erase(strsize);
	string valueFileName = databasePath.append(table); valueFileName.append(".value"); databasePath.erase(strsize);
	string availableSpaceFileName = databasePath.append(table); availableSpaceFileName.append(".space");

	bpt = new BPlusTree();
	bpt_id = new BPlusTreePlus();
	bpt_data = new BPlusTreePlus();
	//db = new databaseIO(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	db = new databaseIO(indexFileName, valueFileName, availableSpaceFileName, bpt, bpt_id, bpt_data);
	printf_s("Table %s selected successfully.\n", table);

}

database::~database()
{
	delete bpt_data;
	delete bpt_id;
	delete bpt;
	delete db;
}

void database::insert(datatype &data)
{//All the datas can be duplicated.
	int key = bpt->maxKey();
	indexBPTtype indexBPTtypeTmp;
	indexBPTtypeTmp.key = key;
	indexBPTtypeTmp.next = NULL;
	//id->key
	indexBPTtypeTmp.id = data.id;
	bpt_id->insert(data.id, indexBPTtypeTmp);
	//data->key
	indexBPTtypeTmp.id = bkdr_hash(data.data);
	bpt_data->insert(indexBPTtypeTmp.id, indexBPTtypeTmp);
	//key->address in file
	bpt->insert(key, db->insert(key, data));
}

void database::modify(datatype & data, unsigned int key)
{
	dataBPTtype dataBPTTmp;
	if (bpt->search(key) && bpt->search0(key, dataBPTTmp)) {//this line could be simplified
		//boring stuff to update bpt_id.
		datatype datatypeTmp;
		indexBPTtype indexBPTtypeTmp;
		db->get(dataBPTTmp, datatypeTmp);
		bpt_id->remove(datatypeTmp.id, key);
		indexBPTtypeTmp.id = data.id;
		indexBPTtypeTmp.key = key;
		indexBPTtypeTmp.next = NULL;
		bpt_id->insert0(key, indexBPTtypeTmp);
		//boring stuff to update bpt_data.
		db->get(dataBPTTmp, datatypeTmp);
		bpt_id->remove(datatypeTmp.id, key);
		indexBPTtypeTmp.id = bkdr_hash(data.data);
		bpt_id->insert0(key, indexBPTtypeTmp);
		//Here's how to update files.
		db->modify(dataBPTTmp, data);
		printf_s("Successfully modified key=%d from database.\n", key);
	}
	else {
		printf_s("Can't find key=%d.\n", key);
	}

}

void database::remove(unsigned int key)
{
	//printf_s("Input the key you want to delete: ");
	//scanf_s("%d", &uniqueKey);
	dataBPTtype dataBPTTmp;
	if (bpt->search(key) && bpt->search0(key, dataBPTTmp)) {//this line could be simplified
		//boring stuff to remove key in bpt_id.
		datatype datatypeTmp;
		//indexBPTtype indexBPTtypeTmp;
		unsigned int id;
		db->get(dataBPTTmp, datatypeTmp);
		id = datatypeTmp.id;
		bpt_id->remove(id, key);
		//boring stuff to remove key in bpt_data.
		id = bkdr_hash(datatypeTmp.data);
		bpt_id->remove(id, key);
		//Here's how to update bpt and files.
		bpt->remove(key);
		db->remove(dataBPTTmp);
		printf_s("Successfully removed key=%d from database.\n", key);
	}
	else {
		printf_s("Can't find key=%d.\n", key);
	}
}

void database::listTable(string databasePath)
{
	printf_s("===================================================\n");
	printf_s("|   file name |    Last modified    |   file size |\n");
	int strsize = databasePath.size();
	//Time from http://blog.csdn.net/wangqing_12345/article/details/52092728
	int stampTime = 0;
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char timestr[100];
	Times standard;
	//Traverse the folder from http://www.cnblogs.com/fnlingnzb-learner/p/6424563.html
	string toSearch = databasePath.append("*.index"); databasePath.erase(strsize);
	//const char *to_search = "C:\\database\\*.index";        //�����ҵ��ļ���֧��ͨ���
	const char *to_search = toSearch.c_str();        //�����ҵ��ļ���֧��ͨ���
	long handle;                                    //���ڲ��ҵľ��
	struct _finddata_t fileinfo;                    //�ļ���Ϣ�Ľṹ��
	handle = _findfirst(to_search, &fileinfo);      //��һ�β���
	if (-1 == handle) {

	}
	else {
		while (!_findnext(handle, &fileinfo));			//ѭ�������������ϵ��ļ���ֱ���Ҳ���������Ϊֹ
		{
			printf_s("|%12s ", fileinfo.name);			//��ӡ���ҵ����ļ����ļ���
			tick = fileinfo.time_access;
			tm = *localtime(&tick);
			strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm);
			printf("|%21s", timestr);
			printf("|%12u |\n", fileinfo.size);
		}
		_findclose(handle);                              //�����˹رվ��
	}
	printf_s("|   file name |    Last modified    |   file size |\n");
	printf_s("===================================================\n");
}

void database::get(unsigned int key)
{
	dataBPTtype dataBPTTmp;
	datatype datatypeTmp;
	printf_s("|%10u", key);
	bpt->search0(key, dataBPTTmp);
	db->get(dataBPTTmp, datatypeTmp);
	printf_s("|%10u", datatypeTmp.id);
	printf_s("|%10s", datatypeTmp.data);
	printf_s("|%10s|\n", datatypeTmp.remark);
}

void database::reopen(string databasePath) {
	int strsize = databasePath.size();
	listTable(databasePath);
	char table[40];
	printf_s("Input the table name: "); scanf_s("%s", &table, 40);
	string indexFileName = databasePath.append(table); indexFileName.append(".index"); databasePath.erase(strsize);
	string valueFileName = databasePath.append(table); valueFileName.append(".value"); databasePath.erase(strsize);
	string availableSpaceFileName = databasePath.append(table); availableSpaceFileName.append(".space");
	//db->reopen(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	delete bpt_data;
	delete bpt_id;
	delete bpt;
	delete db;
	bpt = new BPlusTree();
	bpt_id = new BPlusTreePlus();
	bpt_data = new BPlusTreePlus();
	db = new databaseIO(indexFileName, valueFileName, availableSpaceFileName, bpt, bpt_id, bpt_data);
	printf_s("table %s selected successfully.\n", table);
}

int database::execute(const string command)
{
	vector<unsigned int> keys;
	vector<string> commands;
	//vector<string> select;
	SplitString(command, commands, " ");
	vector<string> where;
	bool whereFlag = false;
	for (int i = 0; i < commands.size(); i++) {
		if (whereFlag) {
			where.push_back(commands.at(i));
		}
		if (commands.at(i) == "where") {
			whereFlag = true;
		}
	}
	if (whereFlag) {
		getKeysFromWhere(keys, where);
	}
	//transform(commands.at(0).begin(), commands.at(0).end(), commands.at(0).begin(), ::tolower);
	if (commands.at(0) == "select" && whereFlag) {
		printf_s("=============================================\n");
		printf_s("|   key    |    id    |   data   |  remark  |\n");
		for (int i = 0; i < keys.size(); i++) {
			get(keys.at(i));
		}
		printf_s("|   key    |    id    |   data   |  remark  |\n");
		printf_s("=============================================\n");
	}
	else if (commands.at(0) == "select" && !whereFlag) {
		db->readALL();
	}
	else if (commands.at(0) == "insert") {
		vector<string> data;
		SplitString(command, data, "(");
		string tmp_ = data.at(1);
		SplitString(tmp_, data, ")");
		tmp_ = data.at(0);
		SplitString(tmp_, data, ",");
		//for (int i = 0; i < data.size(); i++)
		datatype data_;
		data_.id = atoi(data.at(0).c_str());
		vector<string> tmp;
		SplitString(data.at(1), tmp, "'");
		strcpy(data_.data, tmp.at(1).c_str());
		SplitString(data.at(2), tmp, "'");
		strcpy(data_.remark, tmp.at(1).c_str());
		insert(data_);
	}
	else if (commands.at(0) == "update" && whereFlag) {
		vector<string> data;
		SplitString(command, data, "(");
		string tmp_ = data.at(1);
		SplitString(tmp_, data, ")");
		tmp_ = data.at(0);
		SplitString(tmp_, data, ",");
		//for (int i = 0; i < data.size(); i++)
		datatype data_;
		data_.id = atoi(data.at(0).c_str());
		vector<string> tmp;
		SplitString(data.at(1), tmp, "'");
		strcpy(data_.data, tmp.at(1).c_str());
		SplitString(data.at(2), tmp, "'");
		strcpy(data_.remark, tmp.at(1).c_str());
		for (int i = 0; i < keys.size(); i++) {
			modify(data_, keys.at(i));
		}
	}
	else if (commands.at(0) == "delete" && whereFlag) {
		for (int i = 0; i < keys.size(); i++) {
			remove(keys.at(i));
		}
	}
	else if (commands.at(0) == "reopen") {
		cout << "Input the path of database(like C:\\database\\): ";
		string databasePath;
		cin >> databasePath;
		reopen(databasePath);
	}
	else if (commands.at(0) == "flush") {
		db->flush();
	}
	else if (commands.at(0) == "exit") {}
	else if (commands.at(0) == "test1") {
		char s[27] = "abcdefghijklmnopqrstuvwxyz";
		datatype data;
		for (int i = 1; i < 1000; i++) {
			data.id = rand() % 50 + 1;
			data.data[0] = s[rand() % 26];
			data.data[1] = s[rand() % 26];
			data.data[2] = s[rand() % 26];
			data.data[3] = s[rand() % 26];
			data.data[4] = s[rand() % 26];
			data.data[5] = '\0';
			data.remark[0] = s[rand() % 26];
			data.remark[1] = s[rand() % 26];
			data.remark[2] = s[rand() % 26];
			data.remark[3] = '\0';
			insert(data);
		}
	}
	else if (commands.at(0) == "test2") {
		unsigned int key;
		for (int i = 1; i < 200; i++) {
			key = rand() % 200;
			remove(key);
		}
	}
	else {
		return 0;
	}
	return 1;
}

void database::SplitString(const std::string & s, std::vector<std::string>& v, const std::string & c)
{
	v.clear();
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

bool database::getKeysFromWhere(vector<unsigned int> &keys, vector<string> &where)
{
	keys.clear();
	string idStr = "id";
	string dataStr = "data";
	int flag = 0;
	bool inKeysFlag = false;
	for (int i = 0; i < where.size(); i++) {
		if (startWith(where.at(i), idStr)) {
			where.at(i).erase(where.at(i).begin(), where.at(i).begin() + idStr.size() + 1);
			int id = atoi(where.at(i).c_str());
			if (bpt_id->search(id)) {
				vector<indexBPTtype> datas;
				bpt_id->search0(id, datas);
				vector<indexBPTtype>::iterator itor;
				inKeysFlag = false;
				if (flag == 1) {
					for (itor = datas.begin(); itor != datas.end(); itor++) {
						for (int j = 0; j < keys.size(); j++) {
							if (keys.at(j) != itor->key) {
								keys.erase(keys.begin() + j);
								j--;
							}
						}
					}
				}
				else if (flag == 2) {
					for (itor = datas.begin(); itor != datas.end(); itor++) {
						for (int j = 0; j < keys.size(); j++) {
							if (keys.at(j) == itor->key) {
								inKeysFlag = true;
							}
						}
						if (!inKeysFlag) keys.push_back(itor->key);
					}
				}
				else {
					for (itor = datas.begin(); itor != datas.end(); itor++) {
						keys.push_back(itor->key);
					}
				}
			}
		}
		else if (startWith(where.at(i), dataStr)) {
			where.at(i).erase(where.at(i).begin(), where.at(i).begin() + dataStr.size() + 1);
			int id = bkdr_hash(where.at(i).c_str());
			if (bpt_data->search(id)) {
				vector<indexBPTtype> datas;
				bpt_data->search0(id, datas);
				vector<indexBPTtype>::iterator itor;
				inKeysFlag = false;
				if (flag == 1) {
					for (itor = datas.begin(); itor != datas.end(); itor++) {
						for (int j = 0; j < keys.size(); j++) {
							if (keys.at(j) != itor->key) {
								keys.erase(keys.begin() + j);
								j--;
							}
						}
					}
				}
				else if (flag == 2) {
					for (itor = datas.begin(); itor != datas.end(); itor++) {
						for (int j = 0; j < keys.size(); j++) {
							if (keys.at(j) == itor->key) {
								inKeysFlag = true;
							}
						}
						if (!inKeysFlag) keys.push_back(itor->key);
					}
				}
				else {
					for (itor = datas.begin(); itor != datas.end(); itor++) {
						keys.push_back(itor->key);
					}
				}
			}
		}
		else if (where.at(i) == "and" || where.at(i) == "&&") {
			flag = 1;
		}
		else if (where.at(i) == "or" || where.at(i) == "||") {
			flag = 2;
		}
		else {
			return false;
		}
	}
	return true;
}

bool database::startWith(string &str, string &startWith)
{
	for (int i = 0; i < startWith.size() - 1; i++){
		if (str[i] != startWith[i]) return false;
	}
	return true;
}

void database::toLower(string & str)
{
	for (unsigned int i = 0; i < str.length(); ++i) {
		str[i] = tolower(str[i]);
	}
}
