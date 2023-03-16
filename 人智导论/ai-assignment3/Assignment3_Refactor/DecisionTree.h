#pragma once
#include <string>
#include <vector>
#include "Example.h"
#include "TreeNode.h"
#include "Dataset.h"

using namespace std;

class DecisionTree
{
public:
	DecisionTree(Dataset trainset);	// 传入训练数据集，用于初始化决策树，初始化过程中会训练得到一棵决策树
	
	TreeNode* root;	// 保存训练得到的决策树

	// 以下四个函数用于计算信息增益
	double entropy(double p1, double p2);
	double entropy_binary(double p);
	double entropy_remain(string attribute, vector<Example> examples);
	double info_gain(const string& attribute, const vector<Example>& examples);
	// 利用信息增益选择attribute
	string importance(const vector<string>& attributes, const vector<Example>& examples);
	// 对应Figure 18.5中描述的伪代码：DECISION-TREE-LEARNING
	TreeNode* learn(const vector<Example>& examples, const vector<string>& attributes, const vector<Example>& parent_examples);
	// 以下两个函数用于预测
	int classify_rec(const Example& example, TreeNode* root);
	vector<int> classify(const vector<vector<string>>& test_raw_values);
private:
	TreeNode* plurality_value(vector<Example> examples);	// 计算examples中的多数标签取值，返回的TreeNode为决策树的叶节点，对应伪代码中的PLURALITY-VALUE(parent examples)函数
	vector<Example> get_examples(const vector<Example>& examples, const string& attr, const string& option);	// 获取examples中，attr值为option样例
	int get_positive_count(const vector<Example>& examples);	// 获取examples中正例的数量
	bool have_same_class(const vector<Example>& examples);	// 判断examples的标签是否都相同
	vector<string> remove_attribute(const vector<string>& attributes, const string& attribute); // 从attributes列表中移除attribute
};

