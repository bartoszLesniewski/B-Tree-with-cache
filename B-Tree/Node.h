#pragma once

struct Node
{
	bool isLeaf;
	int cntKey;
	int* keys;
	Node** sons;
};