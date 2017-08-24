#pragma once

#include "stdafx.h"
#include "bpt.h"
#include "bpt+.h"
using namespace std;

class databaseIO {
public:
	//构造函数需要bpt bpt_id bpt_data的原因是多次插入bpt,不适宜做成返回值或数组
	//bpt bpt_id bpt_data在此class应尽量不用 与bpt有关的操作都在外部完成
	databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName,
		BPlusTree *bpt, BPlusTreePlus *bpt_id, BPlusTreePlus *bpt_data);
	~databaseIO();
	void readALL();
	dataBPTtype insert(unsigned int key, datatype &data);
	void get(dataBPTtype & pos, datatype &datatypeTmp);
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
