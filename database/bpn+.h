#pragma once

#include "stdafx.h"

typedef unsigned int KeyType;                    // 键类型  
struct indexBPTtype {
	unsigned int id;//or the hash from string
	unsigned int key;
	indexBPTtype * next;
};
										// 结点基类  
class BPTNodePlus {
public:
	BPTNodePlus();
	virtual ~BPTNodePlus();

	NODE_TYPE getType() const { return Type; }
	void setType(NODE_TYPE type) { Type = type; }
	int getKeyNum() const { return KeyNum; }
	void setKeyNum(int n) { KeyNum = n; }
	KeyType getKeyValue(int i) const { return KeyValues[i]; }
	void setKeyValue(int i, KeyType key) { KeyValues[i] = key; }
	int getKeyIndex(KeyType key)const;   // 找到键值在结点中存储的下标  

	virtual void removeKey(int keyIndex, int childIndex) = 0; // 从结点中移除键值  
	virtual bool removeKey(int keyIndex, int childIndex, KeyType uniqueKey) = 0; // 从结点中移除键值  
	virtual void split(BPTNodePlus *parentNode, int childIndex) = 0; // 分裂结点  
	virtual void mergeChild(BPTNodePlus *parentNode, BPTNodePlus *childNode, int keyIndex) = 0;// 合并结点  
	virtual void clear() = 0;// 清空结点，同时会清空结点所包含的子树结点  
	virtual void borrowFrom(BPTNodePlus *destNode, BPTNodePlus* parentNode, int keyIndex, SIBLING_DIRECTION d) = 0;// 从兄弟结点中借一个键值  
	virtual int getChildIndex(KeyType key, int keyIndex)const = 0;// 根据键值获取孩子结点指针下标  
protected:
	NODE_TYPE Type;
	int KeyNum;
	KeyType KeyValues[MAXNUM_KEY];
};

// 内结点  
class BPTInternalNodePlus : public BPTNodePlus {
public:
	BPTInternalNodePlus();
	virtual ~BPTInternalNodePlus();

	BPTNodePlus *getChild(int i) const { return Childs[i]; }
	void setChild(int i, BPTNodePlus *child) { Childs[i] = child; }
	void insert(int keyIndex, int childIndex, KeyType key, BPTNodePlus *childNode);
	virtual void split(BPTNodePlus *parentNode, int childIndex);
	virtual void mergeChild(BPTNodePlus *parentNode, BPTNodePlus *childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual bool removeKey(int keyIndex, int childIndex, KeyType uniqueKey);
	virtual void clear();
	virtual void borrowFrom(BPTNodePlus *destNode, BPTNodePlus *parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	BPTNodePlus *Childs[MAXNUM_CHILD];
};

// 叶子结点  
class BPTLeafNodePlus : public BPTNodePlus {
public:
	BPTLeafNodePlus();
	virtual ~BPTLeafNodePlus();

	BPTLeafNodePlus *getLeftSibling() const { return LeftSibling; }
	void setLeftSibling(BPTLeafNodePlus *node) { LeftSibling = node; }
	BPTLeafNodePlus *getRightSibling() const { return RightSibling; }
	void setRightSibling(BPTLeafNodePlus *node) { RightSibling = node; }
	void getData(int i, indexBPTtype &data);
	void getData0(int i, vector<indexBPTtype> &datas);
	void setData(int i, const indexBPTtype &data);
	void setData0(int i, const indexBPTtype &data);
	void insert(KeyType key, const indexBPTtype &data);
	void insert0(KeyType key, const indexBPTtype &data);
	virtual void split(BPTNodePlus *parentNode, int childIndex);
	virtual void mergeChild(BPTNodePlus *parentNode, BPTNodePlus *childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual bool removeKey(int keyIndex, int childIndex, KeyType uniqueKey);
	virtual void clear();
	virtual void borrowFrom(BPTNodePlus *destNode, BPTNodePlus *parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	BPTLeafNodePlus *LeftSibling;
	BPTLeafNodePlus *RightSibling;
	indexBPTtype Datas[MAXNUM_LEAF];
};
