#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include "Example.h"
#include "TreeNode.h"

using namespace std;

namespace util
{
	static string LABEL_ATTRIBUTE = "WILLWAIT";	// 指定label属性的名称
	vector<string> get_attributes();	// 获取数据属性，应该为10个
	map< string, set<string> > get_attributes_options();	// 获取每个数据属性的可取值
	int get_attribute_index(string attribute);	// 获取一个属性的索引值，get_attribute_index("ALTERNATE")应该为0
	vector<int> get_labels(vector<Example> examples);	// 把examples的label转换为vector<int>格式
	double evaluate(vector<int> pred, vector<int> gold);	// 评估预测结果，pred为预测标签列表，gold为标准标签列表，pred的长度应该与gold相等
	// 以下函数用于控制台输出
	void print_attributes(vector<string>& attributes);
	void print_example(Example& example);
	void print_examples(vector<Example>& examples);
	void print_raw_values(vector<vector<string>> raw_values);
	void print_options(map<string, set<string> >& options);
	void print_tree(TreeNode* root, int depth);
}