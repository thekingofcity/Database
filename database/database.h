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
	void insert(datatype &data);
	void modify(datatype &data,unsigned int key);
	void remove(unsigned int key);
	void get(unsigned int key);
	void reopen(string databasePath);
	int execute(const string command);
private:
	BPlusTree * bpt;
	BPlusTreePlus * bpt_id;
	BPlusTreePlus * bpt_data;
	class databaseIO *db;
	void listTable(string databasePath);
	//sort in class from http://www.cnblogs.com/vongang/archive/2012/03/21/2409293.html
	void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
	bool getKeysFromWhere(vector<unsigned int> &keys, vector<string> &where);
	bool startWith(string &str, string &startWith);
	void toLower(string &str);
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
