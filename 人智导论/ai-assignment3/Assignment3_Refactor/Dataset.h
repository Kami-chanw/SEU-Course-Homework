#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
#include "Example.h"

using namespace std;

/*
* 用于储存数据集
*/
class Dataset
{
public:
	Dataset(string filename);	// 使用文件进行初始化
	Dataset(vector<vector<string>> row);	// 使用数据列表进行初始化

	vector<string> attributes;	// 数据集的特征列表
	vector<vector<string>> raw_values;	// 数据列表，每条数据包含11个特征，最后一个特征为标签
	vector<Example> examples;	// 把数据保存为Example的形式
	vector<string> get_values(string line);	// 读取一行数据，line中包括11个特征，以逗号分隔
	bool get_label(string value);	// 获取标签的bool值，"1"为true
	Example build_example(vector<string> values);	// 把vector<string>的数据格式转化为Example
};

