#pragma once

#include "stdafx.h"
#include "bpt.h"
#include "bpt+.h"
using namespace std;

class databaseIO {
public:
	//���캯����Ҫbpt bpt_id bpt_data��ԭ���Ƕ�β���bpt,���������ɷ���ֵ������
	//bpt bpt_id bpt_data�ڴ�classӦ�������� ��bpt�йصĲ��������ⲿ���
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
	//availableSpace.dat ���عرո���һ�� flush����һ��
	//ʣ��ʱ����vector<int> availableSpace�и���
};
