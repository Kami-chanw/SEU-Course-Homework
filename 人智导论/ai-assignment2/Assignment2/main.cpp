// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <ctime>
#include "Queen.h"
#include "Csp.h"
#include <fstream>
#include "search.h"
#include <format>
using namespace std;
Csp initCsp(int size)
{
	std::vector<Queen*> variables;
	for (int i = 0; i < size; i++)
	{
		Queen* queen = new Queen(i + 1, size);
		variables.push_back(queen);
	}

	return Csp(variables);
}

// backtrackExperiment和minConflictExperiment只是样例实验，同学可以根据需求自己实现实验函数。
void backtrackExperiment(int maxPuzzleSize)
{
	std::cout << "Backtrack Search Experiment Begin." << std::endl;
	std::ofstream fs("backtrack_data.csv", ios::trunc | ios::out);

	fs << "puzzle size,time\n";
	for (int size = 4; size <= maxPuzzleSize; size++)
	{
		clock_t sum = 0;
		for (int i = 0; i < 100; ++i) {
			std::cout << "Current Size: " << size << std::endl;
			Csp csp = initCsp(size);
			clock_t start = clock();
			auto solution = search::backtrackingSearch(csp);
			sum += clock() - start;
		}
		fs << format("{},{}\n", size, sum / 100.0);
	}
}
// 由于该算法存在随机性，偶尔存在无法在maxStep范围内找到结果的情况属于正常现象
void minConflictExperiment(int maxPuzzleSize, int iteration, int maxStep)
{
	std::cout << "MinConflict Search Experiment Begin." << std::endl;
	std::ofstream fs("min_conflicts_data.csv", ios::trunc | ios::out);

	fs << "puzzle size,time,success rate,average steps\n";
	for (int size = 4; size <= maxPuzzleSize; ++size) {
		clock_t sum = 0;
		size_t fail_n = 0, step_sum = 0;
		for (int it = 0; it < iteration; it++)
		{
			Csp csp = initCsp(size);
			csp.randomAssign();

			clock_t start = clock();
			auto [step, solution] = search::minConflict(csp, maxStep);
			clock_t end = clock();

			if (solution[0] == NULL || !search::isSolution(csp, solution))
				fail_n++;
			else {
				sum += end - start;
				step_sum += step;
			}
		}
		decltype((iteration - fail_n) * 100.0 / iteration) k;
		fs << format("{},{},{},{}\n", size, sum / static_cast<double>(iteration), (iteration - fail_n) * 100.0 / iteration, (double)step_sum / (iteration - fail_n));
	}

}
// 搜索函数结果最后通过该函数测试
void searchTest(search::searchFunc searchFunc)
{
	std::cout << "Search Test Begin." << std::endl;
	for (int i = 0; i < 100; i++)
	{
		Csp csp = initCsp(8);
		std::vector<Queen*> solution = searchFunc(csp);
		if (search::failed(solution))
		{
			std::cout << "Iteration " << i << ": failed!" << std::endl;
		}
		else
		{
			std::cout << "Iteration " << i << ": " << search::isSolution(csp, solution) << std::endl;
		}
	}
}

int main(int argc, const char* argv[])
{
	std::srand((unsigned int)std::time(NULL)); // Generate a random seed

	//backtrackExperiment(15);
	minConflictExperiment(15, 100, 200);
	//searchTest(search::backtrackingSearch);
	//searchTest(search::minConflictWrapper);

	return 0;
}
