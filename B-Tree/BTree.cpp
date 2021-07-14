#include <iostream>
#include "BTree.h"

// FUNKCJE WYKONUJ¥CE OPERACJE NA B-DRZEWIE ZOSTA£Y ZAIMPLEMENTOWANE NA PODSTAWIE PSEUDOKODÓW 
// ZAWARTYCH W KSI¥¯CE T. CORMENA "WPROWADZENIE DO ALGORYTMÓW" ORAZ W MATERIA£ACH WYK£ADOWYCH

BTree::BTree()
{
	t = 0;
	root = nullptr;
}

BTree::~BTree()
{
	freeMemory(root);
}

void BTree::freeMemory(Node* node)
{
	if (!node->isLeaf)
	{
		for (int i = 0; i <= node->cntKey; i++)
			freeMemory(node->sons[i]);
	}

	delete[] node->keys;
	delete[] node->sons;
	delete node;
}

void BTree::setOrder(int value)
{
	t = value;
}

Node* BTree::createNode(bool isLeaf)
{
	Node* newNode = new Node;
	newNode->isLeaf = isLeaf;
	newNode->cntKey = 0;
	newNode->keys = new int[2 * t - 1];
	newNode->sons = new Node * [2 * t];

	return newNode;
}

void BTree::print(printFormat format) const
{
	if (format == WITH_BRACKETS)
		std::cout << t << std::endl;

	if (root != nullptr)
	{
		visitNode(root, format);
		std::cout << std::endl;
	}
}

void BTree::visitNode(Node* nodeToVisit, printFormat format)
{
	if (format == WITH_BRACKETS && nodeToVisit->cntKey > 0)
		std::cout << "( ";

	for (int i = 0; i < nodeToVisit->cntKey; i++)
	{
		if (!nodeToVisit->isLeaf)
			visitNode(nodeToVisit->sons[i], format);

		std::cout << nodeToVisit->keys[i] << " ";
	}

	if (!nodeToVisit->isLeaf)
		visitNode(nodeToVisit->sons[nodeToVisit->cntKey], format);
	
	if (format == WITH_BRACKETS && nodeToVisit->cntKey > 0)
		std::cout << ") ";
}

void BTree::search(int key) const
{
	search(root, key);
}

void BTree::search(Node* nodeToSearch, int key) 
{
	int i = 0;

	while (i < nodeToSearch->cntKey && key > nodeToSearch->keys[i])
		i++;

	if (i < nodeToSearch->cntKey && key == nodeToSearch->keys[i])
		std::cout << key << " +" << std::endl;
	else
	{
		if (nodeToSearch->isLeaf)
			std::cout << key << " -" << std::endl;
		else
			search(nodeToSearch->sons[i], key);
	}
}

void BTree::splitChild(Node* parent, int index, Node* son)
{
	Node* newNode = createNode(son->isLeaf);
	newNode->cntKey = t - 1;

	for (int i = 0; i < t - 1; i++)
		newNode->keys[i] = son->keys[i + t];

	if (!son->isLeaf)
	{
		for (int i = 0; i < t; i++)
			newNode->sons[i] = son->sons[i + t];
	}

	son->cntKey = t - 1;

	// robi miejsce dla nowego syna
	for (int i = parent->cntKey; i >= index + 1; i--)
	{
		parent->sons[i + 1] = parent->sons[i];
		parent->keys[i] = parent->keys[i - 1];
	}

	// wstawia nowego syna bezpoœrednio po synu, który jest rozbijany
	parent->sons[index + 1] = newNode;
	parent->keys[index] = son->keys[t - 1];
	parent->cntKey++;
}

void BTree::insert(int key)
{
	if (root == nullptr)
	{
		root = createNode(true);
		root->keys[0] = key;
		root->cntKey++;
	}
	else
	{
		if (root->cntKey == 2 * t - 1)
		{
			Node* newRoot = createNode(false);
			newRoot->sons[0] = root;

			splitChild(newRoot, 0, root);
			root = newRoot;

			insertNonFull(root, key);
		}
		else
			insertNonFull(root, key);
	}
}

void BTree::insertNonFull(Node* node, int key)
{
	int i = node->cntKey - 1;

	if (node->isLeaf)
	{
		while (i >= 0 && key < node->keys[i])
		{
			node->keys[i + 1] = node->keys[i];
			i--;
		}

		node->keys[i + 1] = key;
		node->cntKey++;
	}
	else
	{
		while (i >= 0 && key < node->keys[i])
			i--;

		i++;

		if (node->sons[i]->cntKey == 2 * t - 1)
		{
			splitChild(node, i, node->sons[i]);

			if (key > node->keys[i])
				i++;
		}
	
		insertNonFull(node->sons[i], key);
	}
}

void BTree::load(int order)
{
	t = order;
	load(root);
}

void BTree::load(Node* nodeToLoad)
{
	std::string str;

	while (std::cin >> str)
	{
		if (str == "(")
		{
			if (nodeToLoad == nullptr)
			{
				nodeToLoad = createNode(true);
				root = nodeToLoad;
			}
			else
			{
				Node* son = createNode(true);
				nodeToLoad->isLeaf = false;
				nodeToLoad->sons[nodeToLoad->cntKey] = son;
				load(son);
			}
		}
		else if (str != ")")
		{
			nodeToLoad->keys[nodeToLoad->cntKey] = atoi(str.c_str());
			nodeToLoad->cntKey++;
		}
		else
			break;
	}
}

void BTree::remove(int key)
{
	if (root != nullptr)
	{
		removeFromNode(root, key);

		if (root->cntKey == 0)
		{
			Node* oldRoot = root;

			if (!root->isLeaf)
			{
				root = root->sons[0];
				delete oldRoot;
			}
			else
				root = nullptr;
		}
	}
}

void BTree::removeFromNode(Node* node, int key)
{
	int index = searchForKey(node, key);

	if (node->isLeaf && index < node->cntKey && node->keys[index] == key)
		removeFromLeaf(node, index);
	else if (!node->isLeaf && index < node->cntKey && node->keys[index] == key)
		removeFromInternalNode(node, index);
	else if (!node->isLeaf)
	{
		Node* canContainKey = node->sons[index];

		if (canContainKey->cntKey == t - 1)
		{	
			Node* leftBrother = nullptr, *rightBrother = nullptr;
			if (index > 0)
				leftBrother = node->sons[index - 1];

			if (index < node->cntKey)
				rightBrother = node->sons[index + 1];

			if (leftBrother != nullptr && leftBrother->cntKey > t - 1) 
				moveKeyFromLeftBrother(canContainKey, leftBrother, node, index);

			else if (rightBrother != nullptr && rightBrother->cntKey > t - 1)
				moveKeyFromRightBrother(canContainKey, rightBrother, node, index);

			else
			{
				if (rightBrother == nullptr && leftBrother != nullptr)
				{
					merge(leftBrother, canContainKey, node, index - 1);
					canContainKey = leftBrother;
				}
				else
					merge(canContainKey, rightBrother, node, index);
			}
		}
		removeFromNode(canContainKey, key);
	}
}

int BTree::searchForKey(Node* node, int key)
{
	int i = 0;

	for (; i < node->cntKey; i++)
	{
		if (node->keys[i] >= key)
			break;
	}

	return i;
}

void BTree::removeFromLeaf(Node* node, int index)
{
	for (int i = index; i < node->cntKey - 1; i++)
		node->keys[i] = node->keys[i + 1];

	node->cntKey--;
}

void BTree::removeFromInternalNode(Node* node, int index)
{
	if (node->sons[index]->cntKey >= t)
	{
		int predecessor = findPredecessor(node->sons[index]);
		node->keys[index] = predecessor;
		removeFromNode(node->sons[index], predecessor);
	}
	else if (node->sons[index + 1]->cntKey >= t)
	{
		int successor = findSuccessor(node->sons[index + 1]);
		node->keys[index] = successor;
		removeFromNode(node->sons[index + 1], successor);
	}
	else
	{
		int key = node->keys[index];
		merge(node->sons[index], node->sons[index + 1], node, index);
		removeFromNode(node->sons[index], key);
	}
}

int BTree::findPredecessor(Node* node)
{
	int predecessor = node->keys[node->cntKey - 1];

	while (!node->isLeaf)
	{
		node = node->sons[node->cntKey];
		predecessor = node->keys[node->cntKey - 1];
	}
	
	return predecessor;
}

int BTree::findSuccessor(Node* node)
{
	int successor = node->keys[0];

	while (!node->isLeaf)
	{
		node = node->sons[0];
		successor = node->keys[0];
	}

	return successor;
}

void BTree::moveKeyFromLeftBrother(Node* node, Node* leftBrother, Node* parent, int nodePosition)
{
	for (int i = node->cntKey - 1; i >= 0; i--)
		node->keys[i + 1] = node->keys[i];

	node->keys[0] = parent->keys[nodePosition - 1];
	parent->keys[nodePosition - 1] = leftBrother->keys[leftBrother->cntKey - 1];

	if (!node->isLeaf)
	{
		for (int i = node->cntKey; i >= 0; i--)
			node->sons[i + 1] = node->sons[i];

		node->sons[0] = leftBrother->sons[leftBrother->cntKey];
	}

	node->cntKey++;
	leftBrother->cntKey--;
}

void BTree::moveKeyFromRightBrother(Node* node, Node* rightBrother, Node* parent, int nodePosition)
{
	node->keys[node->cntKey] = parent->keys[nodePosition];
	parent->keys[nodePosition] = rightBrother->keys[0];

	if (!node->isLeaf)
		node->sons[node->cntKey + 1] = rightBrother->sons[0];

	for (int i = 0; i < rightBrother->cntKey - 1; i++)
		rightBrother->keys[i] = rightBrother->keys[i + 1];

	if (!rightBrother->isLeaf)
	{
		for (int i = 0; i < rightBrother->cntKey; i++)
			rightBrother->sons[i] = rightBrother->sons[i + 1];
	}

	node->cntKey++;
	rightBrother->cntKey--;
}

void BTree::merge(Node* node, Node* brother, Node* parent, int parentKeyPosition) const
{
	node->keys[t - 1] = parent->keys[parentKeyPosition];

	for (int i = 0; i < brother->cntKey; i++)
		node->keys[i + t] = brother->keys[i];

	if (!node->isLeaf)
	{
		for (int i = 0; i <= brother->cntKey; i++)
			node->sons[i + t] = brother->sons[i];
	}

	// wype³nia "dziurê" w kluczach po przesuniêciu klucza z bie¿¹cego wêz³a do wêz³a scalonego
	for (int i = parentKeyPosition; i < parent->cntKey - 1; i++)
		parent->keys[i] = parent->keys[i + 1];

	// wype³nia "dziurê" we wskaŸnikach do synów 
	for (int i = parentKeyPosition + 1; i < parent->cntKey; i++)
		parent->sons[i] = parent->sons[i + 1];

	node->cntKey += brother->cntKey + 1;
	parent->cntKey--;

	delete[] brother->keys;
	delete[] brother->sons;
	delete brother;
}

void BTree::measure(int size)
{
	int* cache = new int[size];
	int currentSize = 0, key, noCache = 0, withCache = 0, depth = 0;

	while (std::cin >> key)
	{
		calculateDepth(key, root, &depth);
		noCache += depth;

		if (!isInCache(cache, currentSize, key))
		{
			withCache += depth;
			addToCache(cache, size, &currentSize, key);
		}

		depth = 0;
	}

	std::cout << "NO CACHE: " << noCache << " CACHE: " << withCache << std::endl;

	delete[] cache;
}

void BTree::calculateDepth(int key, Node* node, int *depth)
{
	int i = 0;

	while (i < node->cntKey && key > node->keys[i])
		i++;

	(*depth)++;

	if ((key != node->keys[i] || i > node->cntKey) && !node->isLeaf)
		calculateDepth(key, node->sons[i], depth);
}

bool BTree::isInCache(int* cache, int size, int key)
{
	for (int i = 0; i < size; i++)
	{
		if (cache[i] == key)
			return true;
	}

	return false;
}

void BTree::addToCache(int* cache, int size, int* currentSize, int key)
{
	if (*currentSize < size)
	{
		cache[size - *currentSize - 1] = key;
		(*currentSize)++;
	}
	else
	{
		for (int i = size - 1; i >= 1; i--)
			cache[i] = cache[i - 1];

		cache[0] = key;
	}
}