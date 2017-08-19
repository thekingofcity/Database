#include "bpt.h"
#include <iostream>
#include <algorithm>
using namespace std;

BPlusTree::BPlusTree() {
	Root = nullptr;
	DataHead = nullptr;
	MaxKey = 0;
}

BPlusTree::~BPlusTree() {
}

bool BPlusTree::insert(KeyType key, const dataBPTtype &data) {
	// 是否已经存在  
	if (search(key)) {
		return false;
	}
	// 找到可以插入的叶子结点，否则创建新的叶子结点  
	if (Root == nullptr) {
		Root = new BPTLeafNode();
		DataHead = (BPTLeafNode*)Root;
		MaxKey = key;
	}
	if (Root->getKeyNum() >= MAXNUM_KEY) // 根结点已满，分裂  
	{
		BPTInternalNode* newNode = new BPTInternalNode();   //创建新的根节点  
		BPTNode * tempNode = Root;
		newNode->setChild(0, tempNode);
		tempNode->split(newNode, 0);   // 叶子结点分裂  
		Root = newNode;  //更新根节点指针
	}
	if (key>MaxKey) // 更新最大键值  
	{
		MaxKey = key;
	}
	if (Root->getType() == LEAF) ((BPTLeafNode*)Root)->insert(key, data);
	else {

		// 找到子结点  
		int keyIndex = Root->getKeyIndex(key);
		int childIndex = Root->getChildIndex(key, keyIndex); // 孩子结点指针索引  
		BPTNode* childNode = ((BPTInternalNode*)Root)->getChild(childIndex);
		if (childNode->getKeyNum() >= MAXNUM_LEAF)    // 子结点已满，需进行分裂  
		{
			childNode->split(Root, childIndex);
			if (Root->getKeyValue(childIndex) <= key) // 确定目标子结点  
			{
				childNode = ((BPTInternalNode*)Root)->getChild(childIndex + 1);
			}
		}
		recursive_insert(childNode, key, data);
	}
	return true;
}


void BPlusTree::recursive_insert(BPTNode* parentNode, KeyType key, const dataBPTtype &data)
{
	if (parentNode->getType() == LEAF)  // 叶子结点，直接插入  
	{
		((BPTLeafNode*)parentNode)->insert(key, data);
	}
	else {
		// 找到子结点  
		int keyIndex = parentNode->getKeyIndex(key);
		int childIndex = parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引  
		BPTNode* childNode = ((BPTInternalNode*)parentNode)->getChild(childIndex);
		if (childNode->getKeyNum() >= MAXNUM_LEAF)    // 子结点已满，需进行分裂  
		{
			childNode->split(parentNode, childIndex);
			if (parentNode->getKeyValue(childIndex) <= key) // 确定目标子结点  
			{
				childNode = ((BPTInternalNode*)parentNode)->getChild(childIndex + 1);
			}
		}
		recursive_insert(childNode, key, data);
	}
}


bool BPlusTree::search(KeyType key)
{
	return recursive_search(Root, key);
}

bool BPlusTree::recursive_search(BPTNode *pNode, KeyType key)const {
	if (pNode == nullptr) {//检测节点指针是否为空，或该节点是否为叶子节点
		return false;
	}
	else {
		int keyIndex = pNode->getKeyIndex(key);
		int childIndex = pNode->getChildIndex(key, keyIndex);// 孩子结点指针索引  
		if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex))
		{
			return true;
		}
		else {
			if (pNode->getType() == LEAF)  //检查该节点是否为叶子节点  
			{
				return false;
			}
			else {
				return recursive_search(((BPTInternalNode*)pNode)->getChild(childIndex), key);
			}
		}
	}
}

bool BPlusTree::remove(KeyType key)
{
	if (!search(key))  //不存在  
	{
		return false;
	}
	if (Root->getKeyNum() == 1) {//特殊情况处理  
		if (Root->getType() == LEAF)
		{
			Root = nullptr;
			DataHead = nullptr;
			return true;
		}
		else {
			BPTNode *pChild1 = ((BPTInternalNode*)Root)->getChild(0);
			BPTNode *pChild2 = ((BPTInternalNode*)Root)->getChild(1);
			if (pChild1 != nullptr && pChild1 != nullptr && pChild1->getKeyNum() == MINNUM_KEY && pChild2->getKeyNum() == MINNUM_KEY)
			{
				pChild1->mergeChild(Root, pChild2, 0);
				delete Root;
				Root = pChild1;
			}
		}
	}
	recursive_remove(Root, key);
	return true;
}


void BPlusTree::recursive_remove(BPTNode* parentNode, KeyType key)
{
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex);  // 孩子结点指针索引  
	if (parentNode->getType() == LEAF)// 找到目标叶子节点  
	{
		if (key == MaxKey&&keyIndex>0)
		{
			MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		parentNode->removeKey(keyIndex, childIndex); // 直接删除  
													 // 如果键值在内部结点中存在，也要相应的替换内部结点  
		if (childIndex == 0 && Root->getType() != LEAF && parentNode != DataHead)
		{
			changeKey(Root, key, parentNode->getKeyValue(0));
		}
	}
	else // 内结点  
	{
		BPTNode *pChildNode = ((BPTInternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点  
		if (pChildNode != nullptr && pChildNode->getKeyNum() == MINNUM_KEY)              // 包含关键字达到下限值，进行相关操作  
		{
			BPTNode *pLeft = childIndex>0 ? ((BPTInternalNode*)parentNode)->getChild(childIndex-1) : nullptr;   //左兄弟节点  
			BPTNode *pRight = childIndex<parentNode->getKeyNum() ? ((BPTInternalNode*)parentNode)->getChild(childIndex + 1) : nullptr;//右兄弟节点  
																																	  // 先考虑从兄弟结点中借  
			if (pLeft != nullptr && pLeft->getKeyNum()>MINNUM_KEY)// 左兄弟结点可借  
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight != nullptr && pRight->getKeyNum()>MINNUM_KEY)//右兄弟结点可借  
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			//左右兄弟节点都不可借，考虑合并  
			else if (pLeft != nullptr)   //与左兄弟合并  
			{
				pLeft->mergeChild(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight != nullptr)                 //与右兄弟合并  
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			//	pChildNode = pRight;
			}
		}
		recursive_remove(pChildNode, key);
	}
}

void BPlusTree::changeKey(BPTNode* pNode, KeyType oldKey, KeyType newKey)
{
	if (pNode != nullptr && pNode->getType() != LEAF)
	{
		int keyIndex = pNode->getKeyIndex(oldKey);
		if (keyIndex<pNode->getKeyNum() && oldKey == pNode->getKeyValue(keyIndex))// 找到  
		{
			pNode->setKeyValue(keyIndex, newKey);
		}
		else // 继续找  
		{
			changeKey(((BPTInternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
		}
	}
}

bool BPlusTree::search0(KeyType key, dataBPTtype &data) {
	return recursive_search0(Root, key, data);
}

bool BPlusTree::recursive_search0(BPTNode * pNode, KeyType key, dataBPTtype &data) {
	//int keyIndex = pNode->getKeyIndex(key);
	//int childIndex = pNode->getChildIndex(key, keyIndex);
	//if (pNode->getType() == LEAF) {
	//	((BPTLeafNode*)pNode)->getData(keyIndex, data);
	//	return true;
	//}
	//else {
	//	return recursive_search0(((BPTInternalNode*)pNode)->getChild(childIndex), key, data);
	//}
	if (pNode == nullptr) {//检测节点指针是否为空，或该节点是否为叶子节点
		return false;
	}
	else {
		int keyIndex = pNode->getKeyIndex(key);
		int childIndex = pNode->getChildIndex(key, keyIndex);// 孩子结点指针索引  
		if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex))
		{
			((BPTLeafNode*)pNode)->getData(keyIndex, data);
			return true;
		}
		else {
			if (pNode->getType() == LEAF)  //检查该节点是否为叶子节点  
			{
				return false;
			}
			else {
				return recursive_search(((BPTInternalNode*)pNode)->getChild(childIndex), key);
			}
		}
	}
}


