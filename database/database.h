#pragma once

#include <algorithm>
#include "time.h"
#include "databaseIO.h"
#include "bpt.h"

class database {
public:
	database(string databasePath);
	~database();
	dataBPTtype fetch(vector<dataBPTtype> &dataBPT, unsigned int key);
	dataBPTtype fetchById(vector<dataBPTtype_id> &dataBPT_id, unsigned int id, vector<dataBPTtype> &dataBPT);
	void insert(datatype &data, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
	void insert(datatype &data);
	void modify(datatype &data, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
	void remove(int id, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
	void remove(unsigned int key);
	void get(int id, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
	void reopen(string databasePath);
	void flush();
	void readALL() { db->readALL(); }
private:
	BPlusTree * bpt;
	class databaseIO *db;
	vector<dataBPTtype> dataBPT;//b+tree key->indexPos,valuePos
	vector<dataBPTtype_id> dataBPT_id;//b+tree id->key
	vector<datatype> cache_insert;
	vector<unsigned int> cache_remove;
	void insert_update();
	void remove_update();
	static bool sortByKey(const dataBPTtype &v1, const dataBPTtype &v2);
	//sort in class from http://www.cnblogs.com/vongang/archive/2012/03/21/2409293.html
};

typedef struct times
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;
