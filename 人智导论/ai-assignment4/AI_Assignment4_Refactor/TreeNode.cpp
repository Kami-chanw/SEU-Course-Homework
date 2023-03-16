#include "TreeNode.h"

TreeNode::TreeNode(string name, string token, vector<TreeNode*> children)
{
    this->name = name;
    this->token = token;
    this->children = children;
}