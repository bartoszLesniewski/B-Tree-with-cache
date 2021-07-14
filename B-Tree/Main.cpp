#include <iostream>
#include "BTree.h"
using namespace std;

void chooseCommand(BTree *bt)
{
	char command;
	int value;

	while (std::cin >> command)
	{	
		if (command == 'I')
		{
			std::cin >> value;
			bt->setOrder(value);
		}
		else if (command == 'A')
		{
			std::cin >> value;
			bt->insert(value);
		}
		else if (command == '?')
		{
			std::cin >> value;
			bt->search(value);
		}
		else if (command == 'P')
			bt->print(BTree::NORMAL);
		else if (command == 'L')
		{
			std::cin >> value;
			bt->load(value);
		}
		else if (command == 'S')
			bt->print(BTree::WITH_BRACKETS);
		else if (command == 'R')
		{
			std::cin >> value;
			bt->remove(value);
		}
		else if (command == 'C')
		{
			std::cin >> value;
			bt->measure(value);
		}
		else if (command == 'X')
			break;
	}
}

int main()
{
	std::ios_base::sync_with_stdio(0);

	BTree bt;
	chooseCommand(&bt);
}