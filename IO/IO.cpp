// IO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
vector<dataBPTtype> dataBPT;

databaseIO::databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName)
{
	indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc);
	valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc);
	availableSpaceFileStream.open(availableSpaceFileName, ios::binary | ios::in | ios::out);
	//indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out);
	//valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out);
	//ios::trunc	Any contents that existed in the file before it is open are discarded.
	//still need to be enhanced https://zhidao.baidu.com/question/146262844.html

	//read from indexFile
	unsigned int key;
	int p_int;
	streampos p;
	dataBPTtype dataBPTtypeTmp;
	while (indexFileStream.peek() != EOF) {
		dataBPTtypeTmp.indexPos = indexFileStream.tellp();
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		//Here to add index to bpt.
		//Something like (key indexPos valuePos).
		dataBPTtypeTmp.key = key;
		dataBPTtypeTmp.valuePos = p_int;
		dataBPT.push_back(dataBPTtypeTmp);
		if (key == 0 && p_int == 0) {
			indexFileStream.seekg(-(UNINTSIZE + INTSIZE), ios::cur);
			p = indexFileStream.tellp();
			indexFileStream.seekg(2 * (UNINTSIZE + INTSIZE), ios::cur);
			availableSpaceIndex.push_back(p);
		}
	}
	indexFileStream.clear();//Just in case.

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
	indexFileStream.close();
	valueFileStream.close();
	availableSpaceFileStream.open(availableSpaceFileName_, ios::binary | ios::in | ios::out | ios::trunc);
	int availableSpaceTmp, i, j;
	j = availableSpace.size();
	for (i = 0; i < j; i++) {
		availableSpaceTmp = availableSpace.at(availableSpace.size() - 1);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();
}

void databaseIO::write() {
	int num = 30;
	indexFileStream.write((char *)(&num), sizeof(int));
	num--;
	indexFileStream.write((char *)(&num), sizeof(int));
	num--;
	indexFileStream.write((char *)(&num), sizeof(int));
}

void databaseIO::read()
{
	indexFileStream.clear();
	valueFileStream.clear();
	indexFileStream.seekp(0, ios::beg);
	valueFileStream.seekp(0, ios::beg);
	datatype data;
	unsigned int key;
	int p_int;
	streampos p;
	while (indexFileStream.peek() != EOF && valueFileStream.peek() != EOF) {
		//the method whether file hits the end is from http://bbs.csdn.net/topics/210052379
		indexFileStream.read((char *)(&key), UNINTSIZE);
		//indexFileStream.read((char *)(&p), sizeof(streampos));
		indexFileStream.read((char *)(&p_int), INTSIZE);
		p = p_int;
		valueFileStream.seekp(p, ios::beg);
		valueFileStream.read((char *)(&data), DATATYPESIZE);
		cout << "id=" << data.id << endl;
		cout << "data=" << data.data << endl;
		cout << "remark=" << data.remark << endl;
	}
	cout << "That's the end of value.dat" << endl;
	cout << endl;
}

dataBPTtype databaseIO::insert(unsigned int key, datatype data)
{
	dataBPTtype dataBPTTmp;
	dataBPTTmp.key = key;
	indexFileStream.clear();
	valueFileStream.clear();
	valueFileStream.seekg(0, ios::end);
	dataBPTTmp.valuePos = valueFileStream.tellg();
	int p_int = valueFileStream.tellg();
	valueFileStream.write((char *)(&data), DATATYPESIZE);
	if (availableSpaceIndex.size() != 0) {
		p_int = availableSpaceIndex.at(availableSpaceIndex.size() - 1);
		streampos p = p_int;
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

int databaseIO::remove(unsigned int key)
{
	indexFileStream.clear();
	indexFileStream.seekp(0, ios::beg);
	unsigned int keyFromIndex;
	int p_int;
	streampos p;
	while (indexFileStream.peek() != EOF) {
		indexFileStream.read((char *)(&keyFromIndex), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		if (key == keyFromIndex) {

			indexFileStream.seekg(-(UNINTSIZE + INTSIZE), ios::cur);

		}
	}
	return 0;
}

int databaseIO::modify(unsigned int key, datatype data)
{
	indexFileStream.clear();
	indexFileStream.seekp(0, ios::beg);
	int keyFromIndex, p_int;
	streampos p;
	while (indexFileStream.peek() != EOF) {
		indexFileStream.read((char *)(&keyFromIndex), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		if (key == keyFromIndex) {
			valueFileStream.clear();
			p = p_int;
			valueFileStream.seekp(p, ios::beg);
			valueFileStream.write((char *)(&data), DATATYPESIZE);
			return 1;
		}
	}
	return 0;
}

void databaseIO::flush()
{
	indexFileStream.flush();
	valueFileStream.flush();
}

int conversion(datatype &data, unsigned int id, string datastr, string remark) {
	if (datastr.size() >= DATASIZE || remark.size() >= REMARKSIZE) {
		return 0;
	}
	else {
		data.id = id;
		int i, j = datastr.size();
		for (i = 0; i <= j; i++) {
			data.data[i] = datastr[i];
		}
		data.data[i] = '\0';
		j = remark.size();
		for (i = 0; i <= j; i++) {
			data.remark[i] = remark[i];
		}
		data.remark[i] = '\0';
		return 1;
	}
}

bool sortByKey(const dataBPTtype &v1, const dataBPTtype &v2) {
	return v1.key < v2.key;//Ascend
}

dataBPTtype fetch(vector<dataBPTtype> &dataBPT, unsigned int key) {
	int i, j;
	j = dataBPT.size();
	for (i = 0; i < j; i++) {
		if (dataBPT.at(i).key == key) {
			return dataBPT.at(i);
		}
	}
	dataBPTtype return0 = { -1,-1,-1 };
}

int main()
{
	string indexFileName = "J:/database/index.dat";
	string valueFileName = "J:/database/value.dat";
	string availableSpaceFileName = "J:/database/availableSpace.dat";
	databaseIO db(indexFileName, valueFileName, availableSpaceFileName);
	datatype data;
	if (conversion(data, 1, "Hello World", "via Wzl")) {
		//获得即将添加的key
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	if (conversion(data, 2, "test", "test")) {
		//获得即将添加的key
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	if (conversion(data, 3, "Hello World", "via Wmy")) {
		//获得即将添加的key
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	db.flush();
	db.read();
	sort(dataBPT.begin(), dataBPT.end(), sortByKey);
	if (conversion(data, 2, "HW", "shijia")) {
		dataBPTtype dataBPTTmp = fetch(dataBPT, 1);
		if (dataBPTTmp.key == -1) {
			cout << "Can't find id." << endl;
		}
		else {
			db.modify(dataBPTTmp.key, data);
		}
	}
	//db.remove(2);
	db.flush();
	db.read();
	getchar();
	return 0;
}

