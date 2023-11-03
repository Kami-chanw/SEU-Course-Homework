#pragma once
#include <vector>

#include "Csp.h"

namespace search
{
	// 获取未赋值的变量，对应Figure 6.5算法的SELECT-UNASSIGNED-VARIABLE(csp)函数
	Queen* selectUnassignedVariable(Csp& csp);
	// 获取一个变量的domain，可以利用该函数调整尝试domian中的值的顺序，对应Figure 6.5算法的ORDER-DOMAIN-VALUES(var, assignment, csp)函数
	std::vector<Position> orderDomainValues(Queen* var, std::vector<Queen*>& assignment, Csp& csp);
	// 使用inference算法，缩小variables的domain，如果一个variable的domain被缩小到1且该variable没有赋值，那么给该variable赋值
	// 并且把该vairable添加到inference得到的结果当中
	std::vector<Queen*> makeInference(Csp& csp, Queen* var, Position value);
	// 判断inferences是否为failure，对应Figure 6.5算法的if inferences != failure和if result != failure
	bool failed(std::vector<Queen*>& inferences);
	// 判断该solution是否成立
	bool isSolution(Csp& csp, std::vector<Queen*>& solution);
	// 在递归函数回溯后，用于去除之前添加在assignment中的手动赋值或预测产生的赋值
	// 对应Figure 6.5算法的remove {var = value} and inferences from assignment
	void refresh(std::vector<Queen*>& assignment);

	typedef std::vector<Queen*>(*searchFunc)(Csp& csp);
	
	// 回溯算法，对应Figure 6.5算法
	std::vector<Queen*> backtrackingSearch(Csp& csp);
	// 回溯算法的回调函数
	std::vector<Queen*> backtrack(std::vector<Queen*> assignment, Csp& csp);

	// 最小冲突算法，对应Figure 6.8算法
	// 注意：在执行该函数前，csp需要运行过randomAssign()函数
	std::vector<Queen*> minConflict(Csp& csp, int maxSteps);
	std::vector<Queen*> minConflictWrapper(Csp& csp);
	// 得到一个position在当前棋盘上的冲突数量
	// 注意：与position在同一列的queen的冲突不应该计算
	int getConflicts(Csp& csp, Position& position);
	// 返回一个目前赋值的冲突数大于0的variable
	Queen* chooseConflictVariable(Csp& csp);
	// 返回var的domian中，可以使冲突数最小的值
	// 注意：使冲突数最小的值可能有多个，需要随机选择，如果不随机选择问题可能会陷入局部稳定点并且该稳定点不是解
	Position getMinConflictValue(Csp& csp, Queen* var);

	// 在控制台中输出搜索结果
	void printSolution(std::vector<Queen*>& solution);
}