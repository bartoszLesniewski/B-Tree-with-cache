#pragma once
#include <string>
#include "Node.h"

class BTree
{
private:
	Node* root;
	int t;

public:
	enum printFormat
	{
		NORMAL = 0,
		WITH_BRACKETS = 1
	};

	BTree();
	~BTree();
	void freeMemory(Node* node);
	void setOrder(int value);
	Node* createNode(bool isLeaf);
	void print(printFormat format) const;
	void search(int key) const;
	void insert(int key);
	void insertNonFull(Node* node, int key);
	void splitChild(Node* parent, int index, Node* son);
	void load(int order);
	void load(Node* nodeToLoad);
	void remove(int key);
	void removeFromNode(Node* node, int key);
	void removeFromInternalNode(Node* node, int index);
	void merge(Node* node, Node* brother, Node* parent, int parentKeyPosition) const;
	void measure(int size);

	static void visitNode(Node* nodeToVisit, printFormat format);
	static void search(Node* nodeToSearch, int key);
	static int searchForKey(Node* node, int key);
	static void removeFromLeaf(Node* node, int index);
	static int findPredecessor(Node* node);
	static int findSuccessor(Node* node);
	static void moveKeyFromLeftBrother(Node* node, Node* leftBrother, Node* parent, int nodePosition);
	static void moveKeyFromRightBrother(Node* node, Node* rightBrother, Node* parent, int nodePosition);
	
	static void calculateDepth(int key, Node* node, int* depth);
	static bool isInCache(int* cache, int size, int key);
	static void addToCache(int* cache, int size, int *currentSize, int key);
};