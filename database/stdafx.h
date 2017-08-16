// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <io.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define DATASIZE 40
#define REMARKSIZE 20
#define INTSIZE sizeof(int)
#define UNINTSIZE sizeof(unsigned int)
#define DATATYPESIZE sizeof(datatype)

#define CACHESIZE 10

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


// TODO: 在此处引用程序需要的其他头文件
