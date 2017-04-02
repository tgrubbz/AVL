// AVL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	AVL<int> tree;
	tree.insert(3);
	tree.insert(2);
	tree.insert(1);
	tree.remove(2);
    return 0;
}

