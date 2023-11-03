#pragma once
#include <vector>

#include "Csp.h"

namespace search
{
	// ��ȡδ��ֵ�ı�������ӦFigure 6.5�㷨��SELECT-UNASSIGNED-VARIABLE(csp)����
	Queen* selectUnassignedVariable(Csp& csp);
	// ��ȡһ��������domain���������øú�����������domian�е�ֵ��˳�򣬶�ӦFigure 6.5�㷨��ORDER-DOMAIN-VALUES(var, assignment, csp)����
	std::vector<Position> orderDomainValues(Queen* var, std::vector<Queen*>& assignment, Csp& csp);
	// ʹ��inference�㷨����Сvariables��domain�����һ��variable��domain����С��1�Ҹ�variableû�и�ֵ����ô����variable��ֵ
	// ���ҰѸ�vairable��ӵ�inference�õ��Ľ������
	std::vector<Queen*> makeInference(Csp& csp, Queen* var, Position value);
	// �ж�inferences�Ƿ�Ϊfailure����ӦFigure 6.5�㷨��if inferences != failure��if result != failure
	bool failed(std::vector<Queen*>& inferences);
	// �жϸ�solution�Ƿ����
	bool isSolution(Csp& csp, std::vector<Queen*>& solution);
	// �ڵݹ麯�����ݺ�����ȥ��֮ǰ�����assignment�е��ֶ���ֵ��Ԥ������ĸ�ֵ
	// ��ӦFigure 6.5�㷨��remove {var = value} and inferences from assignment
	void refresh(std::vector<Queen*>& assignment);

	typedef std::vector<Queen*>(*searchFunc)(Csp& csp);
	
	// �����㷨����ӦFigure 6.5�㷨
	std::vector<Queen*> backtrackingSearch(Csp& csp);
	// �����㷨�Ļص�����
	std::vector<Queen*> backtrack(std::vector<Queen*> assignment, Csp& csp);

	// ��С��ͻ�㷨����ӦFigure 6.8�㷨
	// ע�⣺��ִ�иú���ǰ��csp��Ҫ���й�randomAssign()����
	std::vector<Queen*> minConflict(Csp& csp, int maxSteps);
	std::vector<Queen*> minConflictWrapper(Csp& csp);
	// �õ�һ��position�ڵ�ǰ�����ϵĳ�ͻ����
	// ע�⣺��position��ͬһ�е�queen�ĳ�ͻ��Ӧ�ü���
	int getConflicts(Csp& csp, Position& position);
	// ����һ��Ŀǰ��ֵ�ĳ�ͻ������0��variable
	Queen* chooseConflictVariable(Csp& csp);
	// ����var��domian�У�����ʹ��ͻ����С��ֵ
	// ע�⣺ʹ��ͻ����С��ֵ�����ж������Ҫ���ѡ����������ѡ��������ܻ�����ֲ��ȶ��㲢�Ҹ��ȶ��㲻�ǽ�
	Position getMinConflictValue(Csp& csp, Queen* var);

	// �ڿ���̨������������
	void printSolution(std::vector<Queen*>& solution);
}