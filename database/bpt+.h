#pragma once

#include "bpn+.h"
#include <iostream>
#include <algorithm>

class BPlusTreePlus{
public:
	BPlusTreePlus();
	~BPlusTreePlus();
	bool insert(KeyType key, const indexBPTtype &data);
	bool insert0(KeyType key, const indexBPTtype &data);
	bool remove(KeyType key);
	bool remove(KeyType key, KeyType uniqueKey);
	bool search(KeyType key); // 查找是否存在  
	bool search0(KeyType key, vector<indexBPTtype> &datas);
	KeyType maxKey() { return MaxKey + 1; }
	BPTLeafNodePlus * head() { return DataHead; }
private:
	void recursive_insert(BPTNodePlus *parentNode, KeyType key, const indexBPTtype &data);
	void recursive_insert0(BPTNodePlus *parentNode, KeyType key, const indexBPTtype &data);
	void recursive_remove(BPTNodePlus *parentNode, KeyType key);
	void recursive_remove0(BPTNodePlus *parentNode, KeyType key, KeyType uniqueKey);
	bool recursive_search(BPTNodePlus *pNode, KeyType key)const;
	bool recursive_search0(BPTNodePlus *pNode, KeyType key, vector<indexBPTtype> &datas);
	void changeKey(BPTNodePlus *pNode, KeyType oldKey, KeyType newKey);
	
private:
	BPTNodePlus *Root;
	BPTLeafNodePlus* DataHead;
	KeyType MaxKey;  // B+树中的最大键  
};
