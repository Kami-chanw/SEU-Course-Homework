#pragma once
#include <vector>

/*
	��A*�㷨�ṩhCost
*/
namespace heuristic
{
	struct Position
	{
		int row;
		int col;
	};

	Position getPosition(int gridSize, int value);

	int misplace(std::vector<int>& state, std::vector<int>& goalState);
	int manhattan(std::vector<int>& state, std::vector<int>& goalState);
};

