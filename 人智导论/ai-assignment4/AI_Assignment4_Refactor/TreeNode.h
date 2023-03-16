#ifndef treenode_h
#define treenode_h
#include <string>
#include <vector>

using namespace std;

/**
 * 解析树的节点
 * */
class TreeNode
{
public:
    TreeNode(string name, string token, vector<TreeNode*> children);
    string name;    // 用于保存节点类型，例如：”NP“，”S“...
    string token;   // 保存单词信息，注意：只有叶节点该变量才有值
    vector<TreeNode*> children; // 保存该节点的子节点的指针
};

#endif