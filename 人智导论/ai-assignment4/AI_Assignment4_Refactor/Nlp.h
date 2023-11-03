#ifndef nlp_h
#define nlp_h

#include <map>
#include <string>
#include <vector>
#include "CFGRule.h"
#include "TreeNode.h"
#include "LexRule.h"

using namespace std;

class Nlp
{
public:
    Nlp(map< string, vector<LexRule> > dictionary, map< string, vector<CFGRule> > grammar);
    /**
     * 词典的数据结构为map
     * key为词性，例如：Noun，Verb等
     * value为LexRule，详见LexRule类
     * */
    map< string, vector<LexRule> > dictionary;
    /**
     * key为nonterminal，例如："NP"
     * value为CFGRule列表, 详见CFGRule类
     * */
    map< string, vector<CFGRule> > grammar;

    /**
     * 对句子进行分词，以' '为分隔符
     * */
    vector<string> tokenize(string sentence);
    /**
     * 获取一个词对应的词典规则
     * */
    vector<LexRule> get_lex_rules_by(string token);
    /**
     * 获取一个词的词性
     * */
    string get_token_pos(string token);
    /**
     * 先对整句进行分词，然后获取词性，返回词性列表
     * */
    vector<string> get_sentence_pos(string sentence);
    // the three functions below belongs to bottom-up seach
    /**
     * 找到可以与node列表匹配的最长的规则，匹配必须从第一个node开始。
     * 后文中的leaf_node的初始化过程如下：
        vector<TreeNode*> leaf_nodes;
        for (string token : nlp.tokenize(s1))
        {
            string pos = nlp.get_token_pos(token);
            leaf_nodes.push_back(new TreeNode(pos, token, {}));
        }
     * 例子：
        TreeNode* greedy_result = nlp.greedy_match(leaf_nodes);
        util::print_tree(greedy_result, 0);
     * 控制台输出:
     NP
        Article: the
        Noun: wumpus
     *
     * */
    TreeNode* greedy_match(vector<TreeNode*> nodes);
    /**
     * 获取一个节点列表的所有可能的chunkings
     * 例子：
        vector< vector<TreeNode*> > chunkings = nlp.get_possible_chunkings(leaf_nodes);
        for (vector<TreeNode*> chunking : chunkings) util::print_nodes(chunking);
     * 控制台输出：
        {NP, Verb, Adjective}
        {Article, NP, Verb, Adjective}
        {Article, Noun, VP, Adjective}
        {Article, Noun, Verb, Adjs}
     * */
    vector< vector<TreeNode*> > get_possible_chunkings(vector<TreeNode*> nodes);
    /**
     * 解析一个句子，输出解析树的根节点指针。
     * 解析流程：
     * 1. 分词
     * 2. 根据分词结果和词性初始化叶节点列表
     * 3. 调用解析递归函数parse_rec
     * */
    TreeNode* parse(string sentence);
private:
    /**
     * 获取节点列表中的一部分，begin为起始位置，end为结束位置
     * */
    vector<TreeNode*> get_sub_nodes(vector<TreeNode*> source, int begin, int end);
    /**
     * 利用dfs搜索可能的解析结果，搜索到nodes只包含一个节点且节点为name为”S“时结束搜索
     * */
    TreeNode* parse_rec(vector<TreeNode*> nodes);
};

#endif