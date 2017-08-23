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

unsigned int bkdr_hash(const char* key);//String hash function


enum NODE_TYPE { INTERNAL, LEAF };            // 结点类型：内结点、叶子结点  
enum SIBLING_DIRECTION { LEFT, RIGHT };    // 兄弟结点方向：左兄弟结点、右兄弟结点  

const int ORDER = 4;        // B+树的阶（非根内结点的最小子树个数）  
const int MINNUM_KEY = ORDER - 1;       // 最小键值个数  
const int MAXNUM_KEY = 2 * ORDER - 1;// 最大键值个数  
const int MINNUM_CHILD = MINNUM_KEY + 1;//最小子树个数  
const int MAXNUM_CHILD = MAXNUM_KEY + 1;//最大子树个数  
const int MINNUM_LEAF = MINNUM_KEY;// 最小叶子结点键值个数  
const int MAXNUM_LEAF = MAXNUM_KEY;// 最大叶子结点键值个数  

// TODO: 在此处引用程序需要的其他头文件
