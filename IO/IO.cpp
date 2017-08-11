// IO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


databaseIO::databaseIO(string indexFileName, string valueFileName, string availableSpaceFileName, vector<dataBPTtype> &dataBPT, vector<dataBPTtype_id> &dataBPT_id)
{
	//indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc);
	//valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc);
	availableSpaceFileStream.open(availableSpaceFileName, ios::binary | ios::in | ios::out);
	indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out);
	valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out);
	if (!indexFileStream.is_open()) { indexFileStream.close(); indexFileStream.open(indexFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	if (!valueFileStream.is_open()) { valueFileStream.close(); valueFileStream.open(valueFileName, ios::binary | ios::in | ios::out | ios::trunc); }
	//ios::trunc	Any contents that existed in the file before it is open are discarded.
	//enhanced from https://zhidao.baidu.com/question/146262844.html

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
		if (key == 4294967295 && p_int == -1) {
			p = UNINTSIZE + INTSIZE;
			indexFileStream.seekg(-p, ios::cur);
			p = indexFileStream.tellp();
			p_int = p;
			availableSpaceIndex.push_back(p_int);
			indexFileStream.seekg(p, ios::cur);
			//p = indexFileStream.tellp();
			//p_int = p;
			//indexFileStream.clear();
			//indexFileStream.seekg(p, ios::beg);
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
	flush();
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
	while (indexFileStream.peek() != EOF && valueFileStream.peek() != EOF) {
		//the method whether file hits the end is from http://bbs.csdn.net/topics/210052379
		indexFileStream.read((char *)(&key), UNINTSIZE);
		indexFileStream.read((char *)(&p_int), INTSIZE);
		if (key != -1 && p_int != -1) {
			p = p_int;
			valueFileStream.seekp(p, ios::beg);
			valueFileStream.read((char *)(&data), DATATYPESIZE);
			cout << "key     =" << key << endl;
			cout << "id      =" << data.id << endl;
			cout << "data    =" << data.data << endl;
			cout << "remark  =" << data.remark << endl;
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
			dataBPTtype dataBPTtypeTmp = dataBPT.at(i);
			dataBPT.erase(dataBPT.begin() + i - 1);
			//erase form http://www.cplusplus.com/reference/vector/vector/erase/
			return dataBPTtypeTmp;
			//why not use iterator -> can't return iterator
			//article below still can't fix it
			//http://www.cnblogs.com/wang7/archive/2012/04/27/2474138.html
		}
	}
	dataBPTtype return0 = { -1,-1,-1 };
	return return0;
}

dataBPTtype fetchById(vector<dataBPTtype_id> &dataBPT_id, unsigned int id, vector<dataBPTtype> &dataBPT) {
	int i, j, key;
	//Method of iterator from http://blog.csdn.net/dgyanyong/article/details/21268469
	vector<dataBPTtype_id>::iterator itor;
	vector<dataBPTtype_id>::iterator itor2;
	j = dataBPT_id.size();
	for (itor = dataBPT_id.begin(); itor != dataBPT_id.end();) {
		if (itor->id == id) {
			key = itor->key;
			itor2 = itor;
			itor = dataBPT_id.erase(itor);
			return fetch(dataBPT, key);
		}
		else {
			itor++;
		}
	}
	dataBPTtype return0 = { -1,-1,-1 };
	return return0;
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
	dataBPTtype dataBPTTmp;
	dataBPTtype_id dataBPT_idTmp;
	int tmp = 0, id = 0, key = 0;
	char data_[DATASIZE];
	char remark[REMARKSIZE];
	cout << "1 for insert 2 for modify 3 for remove" << endl << "4 for flush 5 for read 6 for get." << endl;
	cout << "Input operation << ";
	cin >> tmp;
	while (tmp != 0) {
		switch (tmp) {
		case 1:
			//get key after sort
			//key.auto-incresement=true
			cout << "Please input id." << endl;
			cin >> data.id;
			cout << "Please input data." << endl;
			scanf_s("%s", &data.data, DATASIZE);
			cout << "Please input remark." << endl;
			scanf_s("%s", &data.remark, REMARKSIZE);
			if (dataBPT.size() == 0) {
				key = 0;
			}
			else {
				sort(dataBPT.begin(), dataBPT.end(), sortByKey);
				key = dataBPT.at(dataBPT.size() - 1).key + 1;
			}
			dataBPT_idTmp.id = data.id;
			dataBPT_idTmp.key = key;
			dataBPT_id.push_back(dataBPT_idTmp);
			dataBPT.push_back(db.insert(key, data));
			break;
		case 2:
			cout << "Please input id." << endl;
			cin >> data.id;
			cout << "Please input data." << endl;
			scanf_s("%s", &data.data, DATASIZE);
			cout << "Please input remark." << endl;
			scanf_s("%s", &data.remark, REMARKSIZE);
			dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
			if (dataBPTTmp.key == -1) {
				cout << "Can't find id." << endl;
			}
			else {
				db.modify(dataBPTTmp, data);
			}
			break;
		case 3:
			cout << "Please input id." << endl;
			cin >> id;
			dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
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
			db.readALL();
			break;
		case 6:
			cout << "Please input id." << endl;
			cin >> id;
			dataBPTTmp = fetchById(dataBPT_id, id, dataBPT);
			if (dataBPTTmp.key == -1) {
				cout << "Can't find id." << endl;
			}
			else {
				datatype datatypeTmp;
				db.get(dataBPTTmp, datatypeTmp);
				cout << "key     =" << dataBPTTmp.key << endl;
				cout << "id      =" << datatypeTmp.id << endl;
				cout << "data    =" << datatypeTmp.data << endl;
				cout << "remark  =" << datatypeTmp.remark << endl;
			}
			break;
		case 7:
			conversion(data, 1, "Hello World", "via Wzl");
			if (dataBPT.size() == 0) {
				key = 0;
			}
			else {
				sort(dataBPT.begin(), dataBPT.end(), sortByKey);
				key = dataBPT.at(dataBPT.size() - 1).key + 1;
			}
			dataBPT_idTmp.id = 1;
			dataBPT_idTmp.key = key;
			dataBPT_id.push_back(dataBPT_idTmp);
			dataBPT.push_back(db.insert(key, data));

			conversion(data, 2, "test", "test");
			sort(dataBPT.begin(), dataBPT.end(), sortByKey);
			key = dataBPT.at(dataBPT.size() - 1).key + 1;
			dataBPT_idTmp.id = 2;
			dataBPT_idTmp.key = key;
			dataBPT_id.push_back(dataBPT_idTmp);
			dataBPT.push_back(db.insert(key, data));

			conversion(data, 3, "Hello World", "via Wmy");
			sort(dataBPT.begin(), dataBPT.end(), sortByKey);
			key = dataBPT.at(dataBPT.size() - 1).key + 1;
			dataBPT_idTmp.id = 3;
			dataBPT_idTmp.key = key;
			dataBPT_id.push_back(dataBPT_idTmp);
			dataBPT.push_back(db.insert(key, data));

			conversion(data, 4, "I love you", "To shijia");
			sort(dataBPT.begin(), dataBPT.end(), sortByKey);
			key = dataBPT.at(dataBPT.size() - 1).key + 1;
			dataBPT_idTmp.id = 4;
			dataBPT_idTmp.key = key;
			dataBPT_id.push_back(dataBPT_idTmp);
			dataBPT.push_back(db.insert(key, data));

			dataBPTTmp = fetchById(dataBPT_id, 2, dataBPT);
			if (dataBPTTmp.key == -1) {
				cout << "Can't find id." << endl;
			}
			else {
				db.remove(dataBPTTmp);
			}
		default:
			break;
		}
		cout << "1 for insert 2 for modify 3 for remove" << endl << "4 for flush 5 for read 6 for get." << endl;
		cout << "Input operation << ";
		cin >> tmp;
	}
	getchar();
	return 0;
}

