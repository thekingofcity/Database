#include "stdafx.h"
#include "databaseIO.h"

databaseIO::databaseIO(
	string indexFileName, string valueFileName, string availableSpaceFileName,
	BPlusTree * bpt, BPlusTreePlus *bpt_id, BPlusTreePlus *bpt_data)
{
	availableSpaceFileStream.open(availableSpaceFileName, ios::binary | ios::in | ios::out);
	indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out);
	valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out);
	if (!indexFileStream.is_open()) { indexFileStream.close(); indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!valueFileStream.is_open()) { valueFileStream.close(); valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!availableSpaceFileStream.is_open()) { availableSpaceFileStream.close(); availableSpaceFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	//ios::trunc	Any contents that existed in the file before it is open are discarded.
	//enhanced from https://zhidao.baidu.com/question/146262844.html

	//read from indexFile,valueFile to create b+tree
	unsigned int key, id;
	char data[DATASIZE];
	int p_int;
	streampos p;
	dataBPTtype dataBPTtypeTmp;
	indexBPTtype indexBPTtypeTmp;
	while (indexFileStream.peek() != EOF) {
		dataBPTtypeTmp.indexPos = indexFileStream.tellp();
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		//Here to add index to bpt.
		//Something like (key indexPos valuePos).
		if (key == 4294967295 && p_int == -1) {
			p = UNINTSIZE + INTSIZE;
			indexFileStream.seekg(-p, ios::cur);
			p = indexFileStream.tellp();
			p_int = p;
			availableSpaceIndex.push_back(p_int);
			p = UNINTSIZE + INTSIZE;//fix the first index was -1 then indexFileStream.seekg(-p, ios::cur) goes wrong.
			indexFileStream.seekg(p, ios::cur);
		}
		else {
			p = p_int;
			valueFileStream.seekg(p, ios::beg);
			//id->key
			valueFileStream.read((char *)(&id), UNINTSIZE);
			indexBPTtypeTmp.id = id;
			indexBPTtypeTmp.key = key;
			indexBPTtypeTmp.next = NULL;
			bpt_id->insert(id, indexBPTtypeTmp);
			valueFileStream.read((char *)(&data), DATASIZE);
			//data->key
			indexBPTtypeTmp.id = bkdr_hash(data);
			bpt_data->insert(indexBPTtypeTmp.id, indexBPTtypeTmp);
			//key->address in file
			dataBPTtypeTmp.key = key;
			dataBPTtypeTmp.valuePos = p_int;
			bpt->insert(key, dataBPTtypeTmp);
			//printf_s("Key %u loaded successfully.\n", key);
		}
	}

	//create availableSpace table for valueFile
	availableSpaceFileName_ = availableSpaceFileName;
	int availableSpaceTmp;
	while (availableSpaceFileStream.peek() != EOF) {
		availableSpaceFileStream.read((char *)(&availableSpaceTmp), INTSIZE);
		availableSpace.push_back(availableSpaceTmp);
	}
	availableSpaceFileStream.close();
}

databaseIO::~databaseIO()
{
	flush();
	indexFileStream.close();
	valueFileStream.close();
	availableSpaceFileStream.open(availableSpaceFileName_, ios::binary | ios::in | ios::out | ios::trunc);
	int availableSpaceTmp, i, j;
	j = availableSpace.size();
	for (i = 0; i < j; i++) {
		availableSpaceTmp = availableSpace.at(i);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();
}

void databaseIO::readALL()
{
	indexFileStream.clear();
	valueFileStream.clear();
	indexFileStream.seekp(0, ios::beg);
	valueFileStream.seekp(0, ios::beg);
	datatype data;
	unsigned int key;
	int p_int;
	streampos p;
	printf_s("=============================================\n");
	printf_s("|   key    |    id    |   data   |  remark  |\n");
	while (indexFileStream.peek() != EOF && valueFileStream.peek() != EOF) {
		//the method whether file hits the end is from http://bbs.csdn.net/topics/210052379
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		if (key != -1 && p_int != -1) {
			p = p_int;
			valueFileStream.seekp(p, ios::beg);
			valueFileStream.read((char *)(&data), DATATYPESIZE);
			printf_s("|%10u", key);
			printf_s("|%10u", data.id);
			printf_s("|%10s", data.data);
			printf_s("|%10s|\n", data.remark);
		}
	}
	printf_s("|   key    |    id    |   data   |  remark  |\n");
	printf_s("=============================================\n");
}

dataBPTtype databaseIO::insert(unsigned int key, datatype &data)
{
	int p_int;
	dataBPTtype dataBPTTmp;
	dataBPTTmp.key = key;
	indexFileStream.clear();
	valueFileStream.clear();
	//If there is vacant in valueFile, just overwritten it.
	if (availableSpace.size() != 0) {
		p_int = availableSpace.at(availableSpace.size() - 1);
		streampos p = p_int;
		dataBPTTmp.valuePos = p_int;
		valueFileStream.seekg(p, ios::beg);
		valueFileStream.write((char *)(&data), DATATYPESIZE);
		availableSpace.pop_back();
	}
	else {
		valueFileStream.seekg(0, ios::end);
		dataBPTTmp.valuePos = valueFileStream.tellg();
		p_int = valueFileStream.tellg();
		valueFileStream.write((char *)(&data), DATATYPESIZE);
	}
	//If there is vacant in indexFile, just overwritten it.
	if (availableSpaceIndex.size() != 0) {
		streampos p = availableSpaceIndex.at(availableSpaceIndex.size() - 1);
		availableSpaceIndex.pop_back();
		indexFileStream.seekg(p, ios::beg);
		dataBPTTmp.indexPos = indexFileStream.tellg();
		indexFileStream.write((char *)(&key), UNINTSIZE);
		indexFileStream.write((char *)(&p_int), INTSIZE);
	}
	else {
		indexFileStream.seekg(0, ios::end);
		dataBPTTmp.indexPos = indexFileStream.tellg();
		indexFileStream.write((char *)(&key), UNINTSIZE);
		indexFileStream.write((char *)(&p_int), INTSIZE);
	}
	return dataBPTTmp;
}

void databaseIO::get(dataBPTtype & pos, datatype &datatypeTmp)
{
	streampos p = pos.valuePos;
	valueFileStream.clear();
	valueFileStream.seekp(p, ios::beg);
	valueFileStream.read((char *)(&datatypeTmp), DATATYPESIZE);
}

void databaseIO::remove(dataBPTtype &pos)
{
	streampos p = pos.indexPos;
	int tmp = -1, p_int = p;
	availableSpaceIndex.push_back(p_int);
	indexFileStream.clear();
	indexFileStream.seekg(p, ios::beg);
	indexFileStream.write((char *)(&tmp), UNINTSIZE);
	indexFileStream.write((char *)(&tmp), INTSIZE);
	availableSpace.push_back(pos.valuePos);
}

void databaseIO::modify(dataBPTtype &pos, datatype &data)
{
	streampos p = pos.valuePos;
	valueFileStream.clear();
	valueFileStream.seekp(p, ios::beg);
	valueFileStream.write((char *)(&data), DATATYPESIZE);
}

void databaseIO::flush()
{
	indexFileStream.flush();
	valueFileStream.flush();
	availableSpaceFileStream.open(availableSpaceFileName_, ios::binary | ios::in | ios::out | ios::trunc);
	int availableSpaceTmp, i, j;
	j = availableSpace.size();
	for (i = 0; i < j; i++) {
		availableSpaceTmp = availableSpace.at(i);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();
}
