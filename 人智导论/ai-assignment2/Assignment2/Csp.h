#pragma once

#include <vector>
#include <queue>
#include "Queen.h"
#include "relation.h"

// 对应ac-3和backtrackingSearch算法伪代码中的csp
class Csp {
public:
	Csp();
	// 用给定的variables初始化CSP，初始化过程中定义variables、arcs、constrains。
	Csp(std::vector<Queen*> variables);
	~Csp();

	std::vector<Queen*> variables;
	// variables如果有关系，那么他们会被定义为一个pair
	std::queue<std::pair<Queen*, Queen*>> arcs;
	// 定义约束函数
	relation::relationFunc constraints;

	// 获取current Queen除了expection Queen之外的相关Queen，在ac-3算法中使用
	std::vector<Queen*> getNeighboursExcept(Queen& current, Queen& exception);
	// 检查value是否会与现在的assignment冲突
	bool consistent(Position& value, std::vector<Queen*>& assignment);
	// 记录variables的状态，在做inference时variables的domian和position会被更改，用在backtrackingSearch算法中，记录递归前的最后一个状态
	void record(std::vector<Position>& lastPositions, std::vector<std::vector<Position>>& lastDomains);
	// 恢复variables的状态，用于递归失败后回溯到失败之前的最近的状态
	void recover(std::vector<Position>& lastPositions, std::vector<std::vector<Position>>& lastDomains);
	// 给所有variables随机赋值
	void randomAssign();
};
