// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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
	//���캯����ҪdataBPT��dataBPT_id��ԭ���Ƕ�β���bpt,���������ɷ���ֵ������
	//dataBPT��dataBPT_id�ڴ�classӦ�������� ��bpt�йصĲ��������ⲿ���
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
	//availableSpace.dat ���عرո���һ�� flush����һ��
	//ʣ��ʱ����vector<int> availableSpace�и���
};


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
