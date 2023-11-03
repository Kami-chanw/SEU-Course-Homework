#pragma once

#include <vector>
#include <queue>
#include "Queen.h"
#include "relation.h"

// ��Ӧac-3��backtrackingSearch�㷨α�����е�csp
class Csp {
public:
	Csp();
	// �ø�����variables��ʼ��CSP����ʼ�������ж���variables��arcs��constrains��
	Csp(std::vector<Queen*> variables);
	~Csp();

	std::vector<Queen*> variables;
	// variables����й�ϵ����ô���ǻᱻ����Ϊһ��pair
	std::queue<std::pair<Queen*, Queen*>> arcs;
	// ����Լ������
	relation::relationFunc constraints;

	// ��ȡcurrent Queen����expection Queen֮������Queen����ac-3�㷨��ʹ��
	std::vector<Queen*> getNeighboursExcept(Queen& current, Queen& exception);
	// ���value�Ƿ�������ڵ�assignment��ͻ
	bool consistent(Position& value, std::vector<Queen*>& assignment);
	// ��¼variables��״̬������inferenceʱvariables��domian��position�ᱻ���ģ�����backtrackingSearch�㷨�У���¼�ݹ�ǰ�����һ��״̬
	void record(std::vector<Position>& lastPositions, std::vector<std::vector<Position>>& lastDomains);
	// �ָ�variables��״̬�����ڵݹ�ʧ�ܺ���ݵ�ʧ��֮ǰ�������״̬
	void recover(std::vector<Position>& lastPositions, std::vector<std::vector<Position>>& lastDomains);
	// ������variables�����ֵ
	void randomAssign();
};
