#pragma once
#include <string>
#include <set>
#include <map>

using namespace std;

class TreeNode
{
public:
    TreeNode(bool value);   // ���ڹ���Ҷ�ڵ㣬Ҷ�ڵ��attributeֵһ��Ϊ"WILLWAIT"��valueΪҶ�ڵ��ȡֵ��Ҳ���Ǿ������ߵ��ýڵ�ʱ��Ԥ����
    TreeNode(string attribute); // ���ڹ�����Ҷ�ڵ�
    string attribute;
    bool value;
    map<string, TreeNode*> options; // ��map��ʽ�����ӽڵ㣬keyΪstringҲ����attribute���ԵĿ�ȡֵ��valueΪ�ڵ��ָ��
};

