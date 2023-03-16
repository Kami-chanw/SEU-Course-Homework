#pragma once
#include <vector>
#include <string>

using namespace std;

class Example
{
public:
    Example();
	Example(vector<string> data, bool label);
	vector<string> data;	// 用于保存数据属性
	bool label;	// 用于保存数据标签
};