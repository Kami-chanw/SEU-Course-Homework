#pragma once
#include <string>
#include <set>
#include <map>

using namespace std;

class TreeNode
{
public:
    TreeNode(bool value);   // 用于构建叶节点，叶节点的attribute值一定为"WILLWAIT"，value为叶节点的取值，也就是决策树走到该节点时的预测结果
    TreeNode(string attribute); // 用于构建非叶节点
    string attribute;
    bool value;
    map<string, TreeNode*> options; // 以map形式保存子节点，key为string也就是attribute属性的可取值，value为节点的指针
};

