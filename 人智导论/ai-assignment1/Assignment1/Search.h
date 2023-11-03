#pragma once
#include "Node.h"
#include "Problem.h"
#include "Action.h"
#include "Heuristic.h"
#include <vector>
#include <set>
#include <queue>
#include <map>

namespace search
{
	Node childNode(Problem& problem, Node& parent, Action action, int cost);	// ��ӦCHILD-NODE����
	std::vector<Action> getSolution(Node node);		// ��ӦSOLUTION����
	bool inSet(std::set<std::vector<int>>& collection, std::vector<int>& state);
	bool inMap(std::map<std::vector<int>, Node*>& map, std::vector<int>& state);
	// ��oldNode�������滻ΪnewNode�е����ݣ�oldNode��ҪΪfrontier�е�Ԫ��
	void updateNode(std::priority_queue<Node*, std::vector<Node*>, Node::cmp>& frontier, Node* oldNode, Node* newNode);
	// breadth first search �㷨
	std::vector<Action> bfs(Problem& problem);
	// depth limit search �㷨
	std::vector<Action> recursiveDLS(Node& node, Problem& problem, int limit);
	std::vector<Action> dls(Problem& problem, int limit);
	std::vector<Action> dlsWrapper(Problem& problem);
	// a* �㷨
	typedef int(*heuristicFunc)(std::vector<int>& state, std::vector<int>& goalState);
	std::vector<Action> aStar(Problem& problem, heuristicFunc heuristicFunc);
	std::vector<Action> aStarMisplace(Problem& problem);
	std::vector<Action> aStarManhattan(Problem& problem);
};

