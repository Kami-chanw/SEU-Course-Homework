#pragma once
#include <vector>
#include "Action.h"

/*
	对应书中图3.10给出的数据结构。
*/
class Node
{
public:
	Node(Node* parent, Action action, int cost, std::vector<int> state);
	Node();
	~Node();

	Node* parent;
	Action action;
	int cost;
	std::vector<int> state;

	struct cmp
	{
		bool operator() (Node* a, Node* b) const;
	};
};

