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
	static string LABEL_ATTRIBUTE = "WILLWAIT";	// ָ��label���Ե�����
	vector<string> get_attributes();	// ��ȡ�������ԣ�Ӧ��Ϊ10��
	map< string, set<string> > get_attributes_options();	// ��ȡÿ���������ԵĿ�ȡֵ
	int get_attribute_index(string attribute);	// ��ȡһ�����Ե�����ֵ��get_attribute_index("ALTERNATE")Ӧ��Ϊ0
	vector<int> get_labels(vector<Example> examples);	// ��examples��labelת��Ϊvector<int>��ʽ
	double evaluate(vector<int> pred, vector<int> gold);	// ����Ԥ������predΪԤ���ǩ�б�goldΪ��׼��ǩ�б�pred�ĳ���Ӧ����gold���
	// ���º������ڿ���̨���
	void print_attributes(vector<string>& attributes);
	void print_example(Example& example);
	void print_examples(vector<Example>& examples);
	void print_raw_values(vector<vector<string>> raw_values);
	void print_options(map<string, set<string> >& options);
	void print_tree(TreeNode* root, int depth);
}