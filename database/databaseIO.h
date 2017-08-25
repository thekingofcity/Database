#pragma once

#include <io.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include "bpt.h"
#include "bpt+.h"
using namespace std;

class databaseIO {
public:
	//Because the constructor need to insert tons of data into bpt,
	//it's not wise to update the bpt outside the class.
	//It is also important to note that expect constructor all the manipulation to bpt should be done outside. 
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
	string availableSpaceFileName_; // Have an copy of availableSpaceFileName.
									//updated in constructor   used in flush and destructor
	fstream indexFileStream;
	fstream valueFileStream;
	fstream availableSpaceFileStream;
	vector<int> availableSpace;
	vector<int> availableSpaceIndex;
	//availableSpace.dat update only when loading unloading or flush().
	//maintain the newest version in vector<int> availableSpace.
};
