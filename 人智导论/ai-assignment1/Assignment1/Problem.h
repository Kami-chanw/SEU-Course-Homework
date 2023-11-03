#pragma once
#include <vector>
#include <set>
#include "Action.h"
#include "Node.h"

/*
	��Ӧ���е����������㷨���ᵽ��Problem
*/
class Problem
{
public:
	int size;
	int suffleSteps;	// ��Ŀ��״̬����������ߵĲ�����������ϴ�Ƶ����ã��������������ʼ״̬
	std::vector<int> initState;
	std::vector<int> goalState;
	std::set<Node*> nodePtrs;	// �������������д����Ľڵ��ָ�룬�����ڴ��������ڴ�����ʹ����new Node����Ҫ�Ѹ�node��ָ��ŵ��ñ�����

	Problem(int size);
	Problem(int size, int suffleSteps);
	~Problem();

	std::vector<int> result(std::vector<int> state, Action action); // ��Ӧproblem.RESULT����
	bool isGoal(std::vector<int>& state);	// ��Ӧproblem.GOAL-TEST����
	std::vector<Action> getValidActions(std::vector<int>& state);	// ��Ӧproblem.ACTIONS����
	bool isValidAction(std::vector<int>& state, Action action);
	bool isSolution(std::vector<Action>& solution, std::vector<int> initState);

	void updateInitState();		// �����һ�����µĳ�ʼ״̬��������
	void printState(std::vector<int>& state);
	void printSolution(std::vector<Action>& solution);
	void freeMemory();	// �����ڴ�����ͷŴ�����nodePtrs��ָ����ָ����ڴ�
	void freeMemory(Node* node);	// �����ڴ������ʵ��DLS��Ҫ����
private:
	int getAgent(std::vector<int>& state);
};
