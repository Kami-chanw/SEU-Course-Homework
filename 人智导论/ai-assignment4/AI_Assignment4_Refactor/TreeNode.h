#ifndef treenode_h
#define treenode_h
#include <string>
#include <vector>

using namespace std;

/**
 * �������Ľڵ�
 * */
class TreeNode
{
public:
    TreeNode(string name, string token, vector<TreeNode*> children);
    string name;    // ���ڱ���ڵ����ͣ����磺��NP������S��...
    string token;   // ���浥����Ϣ��ע�⣺ֻ��Ҷ�ڵ�ñ�������ֵ
    vector<TreeNode*> children; // ����ýڵ���ӽڵ��ָ��
};

#endif