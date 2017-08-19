#pragma once

#include "stdafx.h"
#include "bpt.h"
using namespace std;

class databaseIO {
public:
	//构造函数需要dataBPT和dataBPT_id的原因是多次插入bpt,不适宜做成返回值或数组
	//dataBPT和dataBPT_id在此class应尽量不用 与bpt有关的操作都在外部完成
	databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName,
		vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id
		);
	databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName,
		BPlusTree *bpt, vector<datatype> cache_insert, vector<unsigned int> cache_remove
	);
	~databaseIO();
	void reopen(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
	void readALL();
	dataBPTtype insert(unsigned int key, datatype &data);
	void get(dataBPTtype & pos, datatype &datatypeTm);
	void remove(dataBPTtype &pos);
	void modify(dataBPTtype &pos, datatype &data);
	void flush();
private:
	fstream indexFileStream;
	fstream valueFileStream;
	fstream availableSpaceFileStream;
	vector<int> availableSpace;
	vector<int> availableSpaceIndex;
	string availableSpaceFileName_;
	//availableSpace.dat 加载关闭更新一次 flush更新一次
	//剩余时候都在vector<int> availableSpace中更新
};
