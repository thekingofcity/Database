#include "bpn.h"


BPTNode::BPTNode() {
	setType(LEAF);
	setKeyNum(0);
}

BPTNode::~BPTNode() {
	setKeyNum(0);
}

int BPTNode::getKeyIndex(KeyType key)const {  //二分
	int left = 0;
	int right = getKeyNum() - 1;
	int current;
	while (left != right) {
		current = (left + right) / 2;
		KeyType currentKey = getKeyValue(current);
		if (key>currentKey) {
			left = current + 1;
		}
		else {
			right = current;
		}
	}
	return left;
}


BPTInternalNode::BPTInternalNode() :BPTNode() {
	setType(INTERNAL);
}

BPTInternalNode::~BPTInternalNode() {

}

void BPTInternalNode::clear()
{
	
}

void BPTInternalNode::split(BPTNode* parentNode, int childIndex)
{
	BPTInternalNode* newNode = new BPTInternalNode();//分裂后的右节点  
	newNode->setKeyNum(MINNUM_KEY);
	int i;
	for (i = 0; i<MINNUM_KEY; ++i)// 拷贝关键字的值  
	{
		newNode->setKeyValue(i, KeyValues[i + MINNUM_CHILD]);
	}
	for (i = 0; i<MINNUM_CHILD; ++i) // 拷贝孩子节点指针  
	{
		newNode->setChild(i, Childs[i + MINNUM_CHILD]);
	}
	setKeyNum(MINNUM_KEY);   //更新左子树的关键字个数  
	((BPTInternalNode*)parentNode)->insert(childIndex, childIndex + 1, KeyValues[MINNUM_KEY], newNode);
}

void BPTInternalNode::insert(int keyIndex, int childIndex, KeyType key, BPTNode* childNode) {
	int i;
	for (i = getKeyNum(); i>keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位  
	{
		setChild(i + 1, Childs[i]);
		setKeyValue(i, KeyValues[i - 1]);
	}
	if (i == childIndex)
	{
		setChild(i + 1, Childs[i]);
	}
	setChild(childIndex, childNode);
	setKeyValue(keyIndex, key);
	setKeyNum(KeyNum + 1);
}

void BPTInternalNode::mergeChild(BPTNode* parentNode, BPTNode* childNode, int keyIndex)
{
	// 合并数据  
	insert(MINNUM_KEY, MINNUM_KEY + 1, parentNode->getKeyValue(keyIndex), ((BPTInternalNode*)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i)
	{
		insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, childNode->getKeyValue(i - 1), ((BPTInternalNode*)childNode)->getChild(i));
	}
	//父节点删除index的key  
	parentNode->removeKey(keyIndex, keyIndex + 1);
	
}

void BPTInternalNode::removeKey(int keyIndex, int childIndex)
{
	for (int i = 0; i<getKeyNum() - keyIndex; ++i)
	{
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

void BPTInternalNode::borrowFrom(BPTNode* siblingNode, BPTNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT:  // 从左兄弟结点借  
	{
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((BPTInternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	}
	break;
	case RIGHT:  // 从右兄弟结点借  
	{
		insert(getKeyNum(), getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((BPTInternalNode*)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	}
	break;
	default:
		break;
	}
}

int BPTInternalNode::getChildIndex(KeyType key, int keyIndex)const {
	if (key >= getKeyValue(keyIndex)) {
		return keyIndex + 1;
	}
	else {
		return keyIndex;
	}
}

 
BPTLeafNode::BPTLeafNode() :BPTNode() {
	setType(LEAF);
	setLeftSibling(nullptr);
	setRightSibling(nullptr);
}

BPTLeafNode::~BPTLeafNode() {

}

void BPTLeafNode::clear()
{
	
}

void BPTLeafNode::insert(KeyType key, const dataBPTtype &data)
{
	int i;
	for (i = KeyNum; i >= 1 && KeyValues[i - 1]>key; --i)
	{
		setKeyValue(i, KeyValues[i - 1]);
		setData(i, Datas[i - 1]);
	}
	setKeyValue(i, key);
	setData(i, data);
	setKeyNum(KeyNum + 1);
}

void BPTLeafNode::split(BPTNode* parentNode, int childIndex)
{
	BPTLeafNode* newNode = new BPTLeafNode();//分裂后的右节点  
	setKeyNum(MINNUM_LEAF);
	newNode->setKeyNum(MINNUM_LEAF + 1);
	newNode->setRightSibling(getRightSibling());
	setRightSibling(newNode);
	newNode->setLeftSibling(this);
	int i;
	for (i = 0; i<MINNUM_LEAF + 1; ++i)// 拷贝关键字的值  
	{
		newNode->setKeyValue(i, KeyValues[i + MINNUM_LEAF]);
	}
	for (i = 0; i<MINNUM_LEAF + 1; ++i)// 拷贝数据  
	{
		newNode->setData(i, Datas[i + MINNUM_LEAF]);
	}
	((BPTInternalNode*)parentNode)->insert(childIndex, childIndex + 1, KeyValues[MINNUM_LEAF], newNode);
}

void BPTLeafNode::mergeChild(BPTNode* parentNode, BPTNode* childNode, int keyIndex)
{
	// 合并数据  
	for (int i = 0; i<childNode->getKeyNum(); ++i)
	{
		dataBPTtype data;
		((BPTLeafNode*)childNode)->getData(i, data);
		insert(childNode->getKeyValue(i), data);
	}
	setRightSibling(((BPTLeafNode*)childNode)->getRightSibling());
	//父节点删除index的key，  
	parentNode->removeKey(keyIndex, keyIndex + 1);
}

void BPTLeafNode::removeKey(int keyIndex, int childIndex)
{
	dataBPTtype data;
	for (int i = keyIndex; i<getKeyNum() - 1; ++i)
	{
		setKeyValue(i, getKeyValue(i + 1));
		getData(i + 1, data);
		setData(i, data);
	}
	setKeyNum(getKeyNum() - 1);
}

void BPTLeafNode::borrowFrom(BPTNode* siblingNode, BPTNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	dataBPTtype data;
	switch (d)
	{
	case LEFT:// 从左兄弟结点借  
	{
		((BPTLeafNode*)siblingNode)->getData(siblingNode->getKeyNum() - 1, data);
		insert(siblingNode->getKeyValue(siblingNode->getKeyNum() - 1), data);
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum() - 1);
		parentNode->setKeyValue(keyIndex, getKeyValue(0));
	}
	break;
	case RIGHT: // 从右兄弟结点借  
	{
		((BPTLeafNode*)siblingNode)->getData(0, data);
		insert(siblingNode->getKeyValue(0), data);
		siblingNode->removeKey(0, 0);
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
	}
	break;
	default:
		break;
	}
}

int BPTLeafNode::getChildIndex(KeyType key, int keyIndex)const {
	return keyIndex;
}
