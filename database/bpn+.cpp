#include "bpn+.h"


BPTNodePlus::BPTNodePlus() {
	setType(LEAF);
	setKeyNum(0);
}

BPTNodePlus::~BPTNodePlus() {
	setKeyNum(0);
}

int BPTNodePlus::getKeyIndex(KeyType key)const {  //二分
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


BPTInternalNodePlus::BPTInternalNodePlus() :BPTNodePlus() {
	setType(INTERNAL);
}

BPTInternalNodePlus::~BPTInternalNodePlus() {

}

void BPTInternalNodePlus::clear()
{
	
}

void BPTInternalNodePlus::split(BPTNodePlus *parentNode, int childIndex)
{
	BPTInternalNodePlus* newNode = new BPTInternalNodePlus();//分裂后的右节点  
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
	((BPTInternalNodePlus*)parentNode)->insert(childIndex, childIndex + 1, KeyValues[MINNUM_KEY], newNode);
}

void BPTInternalNodePlus::insert(int keyIndex, int childIndex, KeyType key, BPTNodePlus *childNode) {
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

void BPTInternalNodePlus::mergeChild(BPTNodePlus *parentNode, BPTNodePlus *childNode, int keyIndex)
{
	// 合并数据  
	insert(MINNUM_KEY, MINNUM_KEY + 1, parentNode->getKeyValue(keyIndex), ((BPTInternalNodePlus*)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i)
	{
		insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, childNode->getKeyValue(i - 1), ((BPTInternalNodePlus*)childNode)->getChild(i));
	}
	//父节点删除index的key  
	parentNode->removeKey(keyIndex, keyIndex + 1);
	
}

void BPTInternalNodePlus::removeKey(int keyIndex, int childIndex)
{
	for (int i = 0; i<getKeyNum() - keyIndex; ++i)
	{
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

bool BPTInternalNodePlus::removeKey(int keyIndex, int childIndex, KeyType uniqueKey)
{
	for (int i = 0; i<getKeyNum() - keyIndex; ++i)
	{
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}
	setKeyNum(getKeyNum() - 1);
	return true;
}

void BPTInternalNodePlus::borrowFrom(BPTNodePlus *siblingNode, BPTNodePlus *parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT:  // 从左兄弟结点借  
	{
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((BPTInternalNodePlus*)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	}
	break;
	case RIGHT:  // 从右兄弟结点借  
	{
		insert(getKeyNum(), getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((BPTInternalNodePlus*)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	}
	break;
	default:
		break;
	}
}

int BPTInternalNodePlus::getChildIndex(KeyType key, int keyIndex)const {
	if (key >= getKeyValue(keyIndex)) {
		return keyIndex + 1;
	}
	else {
		return keyIndex;
	}
}

 
BPTLeafNodePlus::BPTLeafNodePlus() :BPTNodePlus() {
	setType(LEAF);
	setLeftSibling(nullptr);
	setRightSibling(nullptr);
}

BPTLeafNodePlus::~BPTLeafNodePlus() {

}

void BPTLeafNodePlus::clear()
{
	
}

void BPTLeafNodePlus::insert(KeyType key, const indexBPTtype &data)
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

void BPTLeafNodePlus::insert0(KeyType key, const indexBPTtype &data)
{
	int i;
	for (i = KeyNum; i >= 1 && KeyValues[i - 1] > key; --i) {}
	i--;
	setData0(i, data);
}

void BPTLeafNodePlus::split(BPTNodePlus *parentNode, int childIndex)
{
	BPTLeafNodePlus* newNode = new BPTLeafNodePlus();//分裂后的右节点  
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
	((BPTInternalNodePlus*)parentNode)->insert(childIndex, childIndex + 1, KeyValues[MINNUM_LEAF], newNode);
}

void BPTLeafNodePlus::mergeChild(BPTNodePlus *parentNode, BPTNodePlus *childNode, int keyIndex)
{
	// 合并数据  
	for (int i = 0; i<childNode->getKeyNum(); ++i)
	{
		indexBPTtype data;
		((BPTLeafNodePlus*)childNode)->getData(i, data);
		insert(childNode->getKeyValue(i), data);
	}
	setRightSibling(((BPTLeafNodePlus*)childNode)->getRightSibling());
	//父节点删除index的key，  
	parentNode->removeKey(keyIndex, keyIndex + 1);
}

void BPTLeafNodePlus::removeKey(int keyIndex, int childIndex)
{
	indexBPTtype data;
	for (int i = keyIndex; i<getKeyNum() - 1; ++i)
	{
		setKeyValue(i, getKeyValue(i + 1));
		getData(i + 1, data);
		setData(i, data);
	}
	setKeyNum(getKeyNum() - 1);
}

bool BPTLeafNodePlus::removeKey(int keyIndex, int childIndex, KeyType uniqueKey)
{
	indexBPTtype data;
	int i = keyIndex;
	getData(i, data);
	if (data.next == nullptr) {
		for (; i<getKeyNum() - 1; ++i)
		{
			setKeyValue(i, getKeyValue(i + 1));
			getData(i + 1, data);
			setData(i, data);
		}
		setKeyNum(getKeyNum() - 1);
		return true;
	}
	else {
		indexBPTtype *next = &Datas[i];
		indexBPTtype *last = next;
		while (next->next != nullptr) {
			if (next->key = uniqueKey) {
				last->next = next->next;
				delete next;
				return false;
			}
			else {
				last = next;
				next = next->next;
			}
		}
		return false;
	}
}

void BPTLeafNodePlus::borrowFrom(BPTNodePlus *siblingNode, BPTNodePlus *parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	indexBPTtype data;
	switch (d)
	{
	case LEFT:// 从左兄弟结点借  
	{
		((BPTLeafNodePlus*)siblingNode)->getData(siblingNode->getKeyNum() - 1, data);
		insert(siblingNode->getKeyValue(siblingNode->getKeyNum() - 1), data);
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum() - 1);
		parentNode->setKeyValue(keyIndex, getKeyValue(0));
	}
	break;
	case RIGHT: // 从右兄弟结点借  
	{
		((BPTLeafNodePlus*)siblingNode)->getData(0, data);
		insert(siblingNode->getKeyValue(0), data);
		siblingNode->removeKey(0, 0);
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
	}
	break;
	default:
		break;
	}
}

int BPTLeafNodePlus::getChildIndex(KeyType key, int keyIndex)const {
	return keyIndex;
}

void BPTLeafNodePlus::getData(int i, indexBPTtype &data)
{
	data.id = Datas[i].id;
	data.key = Datas[i].key;
}

void BPTLeafNodePlus::getData0(int i, vector<indexBPTtype> &datas)
{
	indexBPTtype *next = &Datas[i];
	while (next->next != nullptr) {
		datas.push_back(*next);
		next = next->next;
	}
	datas.push_back(*next);
}

void BPTLeafNodePlus::setData(int i, const indexBPTtype &data)
{
	Datas[i].id = data.id;
	Datas[i].key = data.key;
	Datas[i].next = 0;
}

void BPTLeafNodePlus::setData0(int i, const indexBPTtype &data)
{
	indexBPTtype *this_ = new indexBPTtype;
	this_->id = data.id;
	this_->key = data.key;
	this_->next = NULL;
	indexBPTtype *next = &Datas[i];
	while (next->next != nullptr) {
		next = next->next;
	}
	next->next = this_;
}
