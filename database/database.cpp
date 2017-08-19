// IO.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "databaseIO.h"
#include "database.h"

bool database::sortByKey(const dataBPTtype &v1, const dataBPTtype &v2) {
	return v1.key < v2.key;//Ascend
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


database::database(string databasePath)
{
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
		printf_s("%s   ", fileinfo.name);				//��ӡ���ҵ����ļ����ļ���
		tick = fileinfo.time_access;
		tm = *localtime(&tick);
		strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm);
		printf("Last modified on: %s\n", timestr);
		while (!_findnext(handle, &fileinfo));			//ѭ�������������ϵ��ļ���ֱ���Ҳ���������Ϊֹ
		{
			printf_s("%s   ", fileinfo.name);
			tick = fileinfo.time_access;
			tm = *localtime(&tick);
			strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm);
			printf("Last modified on: %s\n", timestr);
		}
		_findclose(handle);                              //�����˹رվ��
	}

	char table[40];
	printf_s("Input the table name: "); scanf_s("%s", &table, 40);
	string indexFileName = databasePath.append(table); indexFileName.append(".index"); databasePath.erase(strsize);
	string valueFileName = databasePath.append(table); valueFileName.append(".value"); databasePath.erase(strsize);
	string availableSpaceFileName = databasePath.append(table); availableSpaceFileName.append(".space");

	bpt = new BPlusTree();
	//db = new databaseIO(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	db = new databaseIO(indexFileName, valueFileName, availableSpaceFileName, bpt, cache_insert, cache_remove);
	printf_s("table %s selected successfully.\n", table);

}

database::~database()
{
	delete bpt;
	delete db;
}

dataBPTtype database::fetch(vector<dataBPTtype> &dataBPT, unsigned int key) {
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

dataBPTtype database::fetchById(vector<dataBPTtype_id> &dataBPT_id, unsigned int id, vector<dataBPTtype> &dataBPT) {
	int key;
	//Method of iterator from http://blog.csdn.net/dgyanyong/article/details/21268469
	vector<dataBPTtype_id>::iterator itor;
	vector<dataBPTtype_id>::iterator itor2;
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

void database::insert(datatype &data, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id) {
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
	dataBPT.push_back(db->insert(key, data));
}

void database::insert(datatype &data)
{//All the datas can be duplicated.
	//cache_insert.push_back(data);
	//if (cache_insert.size() > CACHESIZE) {
	//	insert_update();
	//}
	int key = bpt->maxKey();
	bpt->insert(key, db->insert(key, data));
}

void database::insert_update()
{
	int key, i, j = cache_insert.size();
	datatype datatypeTmp;
	for (i = 0; i < j; i++) {
		key = bpt->maxKey();
		datatypeTmp = cache_insert.at(i);
		bpt->insert(key, db->insert(key, datatypeTmp));
	}
	cache_insert.clear();
}

void database::modify(datatype &data, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id) {
	dataBPTtype dataBPTTmp = fetchById(dataBPT_id, data.id, dataBPT);
	if (dataBPTTmp.key == -1) {
		printf_s("Can't find id=%3d.\n", data.id);
	}
	else {
		db->modify(dataBPTTmp, data);
		printf_s("Successfully modified id=%3d from database.\n", data.id);
	}
}

void database::remove(int id, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id) {
	dataBPTtype dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
	if (dataBPTTmp.key == -1) {
		printf_s("Can't find id=%d.\n", id);
	}
	else {
		db->remove(dataBPTTmp);
		printf_s("Successfully removed id=%d from database.\n", id);
	}
}

void database::remove(unsigned int key)
{
	dataBPTtype dataBPTTmp;
	if (bpt->search0(key, dataBPTTmp)) {
		bpt->remove(key);
		db->remove(dataBPTTmp);
		printf_s("Successfully removed key=%d from database.\n", key);
	}
	else {
		printf_s("Can't find key=%d.\n", key);
	}
}

void database::remove_update()
{

}

void database::get(int id, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id) {
	dataBPTtype dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
	if (dataBPTTmp.key == -1) {
		printf_s("Can't find id.\n");
	}
	else {
		datatype datatypeTmp;
		db->get(dataBPTTmp, datatypeTmp);
		printf_s("key     =%3u\n", dataBPTTmp.key);
		printf_s("id      =%3u\n", datatypeTmp.id);
		printf_s("data    =%s\n", datatypeTmp.data);
		printf_s("remark  =%s\n", datatypeTmp.remark);
	}
}

void database::reopen(string databasePath) {
	int strsize = databasePath.size();
	//Time from http://blog.csdn.net/wangqing_12345/article/details/52092728
	int stampTime = 0;
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	Times standard;
	//Traverse the folder from http://www.cnblogs.com/fnlingnzb-learner/p/6424563.html
	string toSearch = databasePath.append("*.index"); databasePath.erase(strsize);
	//const char *to_search = "C:\\database\\*.index";        //�����ҵ��ļ���֧��ͨ���
	const char *to_search = toSearch.c_str();        //�����ҵ��ļ���֧��ͨ���

	long handle;                                    //���ڲ��ҵľ��
	struct _finddata_t fileinfo;                    //�ļ���Ϣ�Ľṹ��
	handle = _findfirst(to_search, &fileinfo);      //��һ�β���
	if (-1 == handle) return;
	printf_s("%s   ", fileinfo.name);				//��ӡ���ҵ����ļ����ļ���
	tick = fileinfo.time_access;
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	printf("Last modified on: %s\n", s);
	while (!_findnext(handle, &fileinfo));			//ѭ�������������ϵ��ļ���ֱ���Ҳ���������Ϊֹ
	{
		printf_s("%s   ", fileinfo.name);
		tick = fileinfo.time_access;
		tm = *localtime(&tick);
		strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
		printf("Last modified on: %s\n", s);
	}
	_findclose(handle);                              //�����˹رվ��

	char table[40];
	printf_s("Input the table name: "); scanf_s("%s", &table, 40);
	string indexFileName = databasePath.append(table); indexFileName.append(".index"); databasePath.erase(strsize);
	string valueFileName = databasePath.append(table); valueFileName.append(".value"); databasePath.erase(strsize);
	string availableSpaceFileName = databasePath.append(table); availableSpaceFileName.append(".space");
	db->reopen(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	printf_s("table %s selected successfully.\n", table);
}

void database::flush()
{
	insert_update();
	db->flush();
}


