#pragma once

#include <algorithm>
#include "time.h"
#include "databaseIO.h"
#include "bpt.h"
#include "bpt+.h"

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
	void get(int id);
	void reopen(string databasePath);
	void flush() { db->flush(); }
	void readALL() { db->readALL(); }
private:
	BPlusTree * bpt;
	BPlusTreePlus * bpt_id;
	class databaseIO *db;
	void listTable(string databasePath);
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
