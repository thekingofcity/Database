#pragma once

#ifndef databaseIO_
#define databaseIO_

#include "stdafx.h"
using namespace std;

class databaseIO {
public:
	//���캯����ҪdataBPT��dataBPT_id��ԭ���Ƕ�β���bpt,���������ɷ���ֵ������
	//dataBPT��dataBPT_id�ڴ�classӦ�������� ��bpt�йصĲ��������ⲿ���
	databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName,
		vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id
		);
	~databaseIO();
	void reopen(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id);
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

#endif