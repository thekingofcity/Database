// IO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


databaseIO::databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id)
{
	indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc);
	valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc);
	availableSpaceFileStream.open(availableSpaceFileName, ios::binary | ios::in | ios::out);
	//indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out);
	//valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out);
	//ios::trunc	Any contents that existed in the file before it is open are discarded.
	//still need to be enhanced https://zhidao.baidu.com/question/146262844.html

	//read from indexFile,valueFile to create b+tree
	unsigned int key, id;
	int p_int;
	streampos p;
	dataBPTtype dataBPTtypeTmp;
	dataBPTtype_id dataBPTtype_idTmp;
	while (indexFileStream.peek() != EOF) {
		dataBPTtypeTmp.indexPos = indexFileStream.tellp();
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		//Here to add index to bpt.
		//Something like (key indexPos valuePos).
		if (key == -1 && p_int == -1) {
			indexFileStream.seekg(-(UNINTSIZE + INTSIZE), ios::cur);
			p = indexFileStream.tellp();
			indexFileStream.seekg(2 * (UNINTSIZE + INTSIZE), ios::cur);
			int p_int = p;
			availableSpaceIndex.push_back(p_int);
		}
		else {
			p = p_int;
			valueFileStream.seekg(p, ios::beg);
			valueFileStream.read((char *)(&id), UNINTSIZE);
			dataBPTtype_idTmp.id = id;
			dataBPTtype_idTmp.key = key;
			dataBPT_id.push_back(dataBPTtype_idTmp);
			dataBPTtypeTmp.key = key;
			dataBPTtypeTmp.valuePos = p_int;
			dataBPT.push_back(dataBPTtypeTmp);
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
		if (key != -1 && p_int != -1) {
			p = p_int;
			valueFileStream.seekp(p, ios::beg);
			valueFileStream.read((char *)(&data), DATATYPESIZE);
			cout << "id=" << data.id << endl;
			cout << "data=" << data.data << endl;
			cout << "remark=" << data.remark << endl;
		}
	}
	cout << "That's the end of value.dat" << endl;
	cout << endl;
}

dataBPTtype databaseIO::insert(unsigned int key, datatype &data)
{
	int p_int;
	dataBPTtype dataBPTTmp;
	dataBPTTmp.key = key;
	indexFileStream.clear();
	valueFileStream.clear();
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
		availableSpaceTmp = availableSpace.at(availableSpace.size() - 1);
		availableSpaceFileStream.write((char *)(&availableSpaceTmp), INTSIZE);
	}
	availableSpaceFileStream.close();
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

dataBPTtype fetchById(vector<dataBPTtype_id> &dataBPT_id, unsigned int id, vector<dataBPTtype> &dataBPT) {
	int i, j, key;
	j = dataBPT_id.size();
	for (i = 0; i < j; i++) {
		if (dataBPT_id.at(i).id == id) {
			return fetch(dataBPT, dataBPT_id.at(i).key);
		}
	}
	dataBPTtype return0 = { -1,-1,-1 };
}

int main()
{
	vector<dataBPTtype> dataBPT;//b+tree key->indexPos,valuePos
	vector<dataBPTtype_id> dataBPT_id;//b+tree id->key
	string indexFileName = "C:/database/index.dat";
	string valueFileName = "C:/database/value.dat";
	string availableSpaceFileName = "C:/database/availableSpace.dat";
	databaseIO db(indexFileName, valueFileName, availableSpaceFileName, dataBPT, dataBPT_id);
	datatype data;
	dataBPTtype_id dataBPT_idTmp;
	if (conversion(data, 1, "Hello World", "via Wzl")) {
		//get the key from dataBPT.size()
		//key.auto-incresement=true
		dataBPT_idTmp.id = 1;
		dataBPT_idTmp.key = dataBPT.size();
		dataBPT_id.push_back(dataBPT_idTmp);
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	if (conversion(data, 2, "test", "test")) {
		dataBPT_idTmp.id = 2;
		dataBPT_idTmp.key = dataBPT.size();
		dataBPT_id.push_back(dataBPT_idTmp);
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	if (conversion(data, 3, "Hello World", "via Wmy")) {
		dataBPT_idTmp.id = 3;
		dataBPT_idTmp.key = dataBPT.size();
		dataBPT_id.push_back(dataBPT_idTmp);
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	if (conversion(data, 4, "I love you", "To shijia")) {
		dataBPT_idTmp.id = 4;
		dataBPT_idTmp.key = dataBPT.size();
		dataBPT_id.push_back(dataBPT_idTmp);
		dataBPT.push_back(db.insert(dataBPT.size(), data));
	}
	db.flush();
	db.read();
	//sort(dataBPT.begin(), dataBPT.end(), sortByKey);
	//if (conversion(data, 2, "HW", "shijia")) {
	//	dataBPTtype dataBPTTmp = fetch(dataBPT, 1);
	//	if (dataBPTTmp.key == -1) {
	//		cout << "Can't find id." << endl;
	//	}
	//	else {
	//		db.modify(dataBPTTmp, data);
	//	}
	//}
	//db.remove(fetch(dataBPT, 1));
	//db.remove(fetch(dataBPT, 2));
	////db.flush();
	////db.read();
	//if (conversion(data, 2, "Hello World", "shijia")) {
	//	//指定修改的key
	//	dataBPT.push_back(db.insert(1, data));
	//}
	//db.flush();
	//db.read();
	int tmp = 0, id = 0;
	char data_[DATASIZE];
	char remark[REMARKSIZE];
	cout << "1 for insert 2 for modify 3 for remove 4 for flush 5 for read." << endl;
	cout << "Input operation <<";
	cin >> tmp;
	while (tmp != 0) {
		switch (tmp){
		case 1:
			cin >> id;
			cin >> data_;
			cin >> remark;
			if (conversion(data, id, data_, remark)) {
				dataBPT_idTmp.id = id;
				dataBPT_idTmp.key = dataBPT.size();
				dataBPT_id.push_back(dataBPT_idTmp);
				db.insert(dataBPT.size(), data);
			}
			break;
		case 2:
			cin >> id;
			cin >> data_;
			cin >> remark;
			if (conversion(data, id, data_, remark)) {
				dataBPTtype dataBPTTmp = fetch(dataBPT, 1);
				if (dataBPTTmp.key == -1) {
					cout << "Can't find id." << endl;
				}
				else {
					db.modify(dataBPTTmp, data);
				}
			}
			break;
		case 3:
			cin >> id;
			dataBPTtype dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
			if (dataBPTTmp.key == -1) {
				cout << "Can't find id." << endl;
			}
			else {
				db.remove(dataBPTTmp);
			}
			break;
		case 4:
			db.flush();
			break;
		case 5:
			db.read();
			break;
		}
		cout << "1 for insert 2 for modify 3 for remove 4 for flush 5 for read." << endl;
		cout << "Input operation <<" << endl;
		cin >> tmp;
	}
	getchar();
	return 0;
}

