#pragma once

#include "stdafx.h"

enum NODE_TYPE { INTERNAL, LEAF };            // 结点类型：内结点、叶子结点  
enum SIBLING_DIRECTION { LEFT, RIGHT };    // 兄弟结点方向：左兄弟结点、右兄弟结点  
typedef unsigned int KeyType;                    // 键类型  
const int ORDER = 4;        // B+树的阶（非根内结点的最小子树个数）  
const int MINNUM_KEY = ORDER - 1;       // 最小键值个数  
const int MAXNUM_KEY = 2 * ORDER - 1;// 最大键值个数  
const int MINNUM_CHILD = MINNUM_KEY + 1;//最小子树个数  
const int MAXNUM_CHILD = MAXNUM_KEY + 1;//最大子树个数  
const int MINNUM_LEAF = MINNUM_KEY;// 最小叶子结点键值个数  
const int MAXNUM_LEAF = MAXNUM_KEY;// 最大叶子结点键值个数  

										// 结点基类  
class BPTNode {
public:
	BPTNode();
	virtual ~BPTNode();

	NODE_TYPE getType() const { return Type; }
	void setType(NODE_TYPE type) { Type = type; }
	int getKeyNum() const { return KeyNum; }
	void setKeyNum(int n) { KeyNum = n; }
	KeyType getKeyValue(int i) const { return KeyValues[i]; }
	void setKeyValue(int i, KeyType key) { KeyValues[i] = key; }
	int getKeyIndex(KeyType key)const;   // 找到键值在结点中存储的下标  

	virtual void removeKey(int keyIndex, int childIndex) = 0; // 从结点中移除键值  
	virtual void split(BPTNode* parentNode, int childIndex) = 0; // 分裂结点  
	virtual void mergeChild(BPTNode* parentNode, BPTNode* childNode, int keyIndex) = 0;// 合并结点  
	virtual void clear() = 0;// 清空结点，同时会清空结点所包含的子树结点  
	virtual void borrowFrom(BPTNode* destNode, BPTNode* parentNode, int keyIndex, SIBLING_DIRECTION d) = 0;// 从兄弟结点中借一个键值  
	virtual int getChildIndex(KeyType key, int keyIndex)const = 0;// 根据键值获取孩子结点指针下标  
protected:
	NODE_TYPE Type;
	int KeyNum;
	KeyType KeyValues[MAXNUM_KEY];
};

// 内结点  
class BPTInternalNode : public BPTNode {
public:
	BPTInternalNode();
	virtual ~BPTInternalNode();

	BPTNode* getChild(int i) const { return Childs[i]; }
	void setChild(int i, BPTNode* child) { Childs[i] = child; }
	void insert(int keyIndex, int childIndex, KeyType key, BPTNode* childNode);
	virtual void split(BPTNode* parentNode, int childIndex);
	virtual void mergeChild(BPTNode* parentNode, BPTNode* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(BPTNode* destNode, BPTNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	BPTNode* Childs[MAXNUM_CHILD];
};

// 叶子结点  
class BPTLeafNode : public BPTNode {
public:
	BPTLeafNode();
	virtual ~BPTLeafNode();

	BPTLeafNode* getLeftSibling() const { return LeftSibling; }
	void setLeftSibling(BPTLeafNode* node) { LeftSibling = node; }
	BPTLeafNode* getRightSibling() const { return RightSibling; }
	void setRightSibling(BPTLeafNode* node) { RightSibling = node; }
	void getData(int i, dataBPTtype &data) { data.key = Datas[i].key; data.indexPos = Datas[i].indexPos; data.valuePos = Datas[i].valuePos; }
	void setData(int i, const dataBPTtype &data) { Datas[i].key = data.key; Datas[i].indexPos = data.indexPos; Datas[i].valuePos = data.valuePos; }
	void insert(KeyType key, const dataBPTtype &data);
	virtual void split(BPTNode* parentNode, int childIndex);
	virtual void mergeChild(BPTNode* parentNode, BPTNode * childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(BPTNode* destNode, BPTNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	BPTLeafNode* LeftSibling;
	BPTLeafNode* RightSibling;
	dataBPTtype Datas[MAXNUM_LEAF];
};

