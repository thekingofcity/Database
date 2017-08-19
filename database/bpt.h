#pragma once

#include "bpn.h"

class BPlusTree{
public:
	BPlusTree();
	~BPlusTree();
	bool insert(KeyType key, const dataBPTtype &data);
	bool remove(KeyType key);
	bool search(KeyType key); // 查找是否存在  
	bool search0(KeyType key, dataBPTtype &data);
	KeyType maxKey() { return MaxKey + 1; }
	BPTLeafNode * head() { return DataHead; }
private:
	void recursive_insert(BPTNode* parentNode, KeyType key, const dataBPTtype &data);
	void recursive_remove(BPTNode* parentNode, KeyType key);
	bool recursive_search(BPTNode *pNode, KeyType key)const;
	bool recursive_search0(BPTNode * pNode, KeyType key, dataBPTtype &data);
	void changeKey(BPTNode *pNode, KeyType oldKey, KeyType newKey);
	
private:
	BPTNode* Root;
	BPTLeafNode* DataHead;
	KeyType MaxKey;  // B+树中的最大键  
};
