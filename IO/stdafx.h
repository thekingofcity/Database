// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include "time.h"//test unit

#include <random>//test unit
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define DATASIZE 40
#define REMARKSIZE 20
#define INTSIZE sizeof(int)
#define UNINTSIZE sizeof(unsigned int)
#define DATATYPESIZE sizeof(datatype)

using namespace std;
struct datatype {
	unsigned int id;
	char data[DATASIZE];
	char remark[REMARKSIZE];
	//can't use string cus this is a non-POD type
	//for more intel https://stackoverflow.com/questions/20914917/c-access-violation/20915057
	//and http://www.cplusplus.com/forum/general/39764/
};
struct dataBPTtype {
	unsigned int key;
	int indexPos;
	int valuePos;
};
struct dataBPTtype_id {
	unsigned int key;
	unsigned int id;
};

class databaseIO {
public:
	//构造函数需要dataBPT和dataBPT_id的原因是多次插入bpt,不适宜做成返回值或数组
	//dataBPT和dataBPT_id在此class应尽量不用 与bpt有关的操作都在外部完成
	databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
	~databaseIO();
	void readALL();
	void write();
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


// TODO: 在此处引用程序需要的其他头文件
