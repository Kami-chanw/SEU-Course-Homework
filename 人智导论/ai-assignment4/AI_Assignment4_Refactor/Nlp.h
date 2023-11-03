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
     * �ʵ�����ݽṹΪmap
     * keyΪ���ԣ����磺Noun��Verb��
     * valueΪLexRule�����LexRule��
     * */
    map< string, vector<LexRule> > dictionary;
    /**
     * keyΪnonterminal�����磺"NP"
     * valueΪCFGRule�б�, ���CFGRule��
     * */
    map< string, vector<CFGRule> > grammar;

    /**
     * �Ծ��ӽ��зִʣ���' 'Ϊ�ָ���
     * */
    vector<string> tokenize(string sentence);
    /**
     * ��ȡһ���ʶ�Ӧ�Ĵʵ����
     * */
    vector<LexRule> get_lex_rules_by(string token);
    /**
     * ��ȡһ���ʵĴ���
     * */
    string get_token_pos(string token);
    /**
     * �ȶ�������зִʣ�Ȼ���ȡ���ԣ����ش����б�
     * */
    vector<string> get_sentence_pos(string sentence);
    // the three functions below belongs to bottom-up seach
    /**
     * �ҵ�������node�б�ƥ�����Ĺ���ƥ�����ӵ�һ��node��ʼ��
     * �����е�leaf_node�ĳ�ʼ���������£�
        vector<TreeNode*> leaf_nodes;
        for (string token : nlp.tokenize(s1))
        {
            string pos = nlp.get_token_pos(token);
            leaf_nodes.push_back(new TreeNode(pos, token, {}));
        }
     * ���ӣ�
        TreeNode* greedy_result = nlp.greedy_match(leaf_nodes);
        util::print_tree(greedy_result, 0);
     * ����̨���:
     NP
        Article: the
        Noun: wumpus
     *
     * */
    TreeNode* greedy_match(vector<TreeNode*> nodes);
    /**
     * ��ȡһ���ڵ��б�����п��ܵ�chunkings
     * ���ӣ�
        vector< vector<TreeNode*> > chunkings = nlp.get_possible_chunkings(leaf_nodes);
        for (vector<TreeNode*> chunking : chunkings) util::print_nodes(chunking);
     * ����̨�����
        {NP, Verb, Adjective}
        {Article, NP, Verb, Adjective}
        {Article, Noun, VP, Adjective}
        {Article, Noun, Verb, Adjs}
     * */
    vector< vector<TreeNode*> > get_possible_chunkings(vector<TreeNode*> nodes);
    /**
     * ����һ�����ӣ�����������ĸ��ڵ�ָ�롣
     * �������̣�
     * 1. �ִ�
     * 2. ���ݷִʽ���ʹ��Գ�ʼ��Ҷ�ڵ��б�
     * 3. ���ý����ݹ麯��parse_rec
     * */
    TreeNode* parse(string sentence);
private:
    /**
     * ��ȡ�ڵ��б��е�һ���֣�beginΪ��ʼλ�ã�endΪ����λ��
     * */
    vector<TreeNode*> get_sub_nodes(vector<TreeNode*> source, int begin, int end);
    /**
     * ����dfs�������ܵĽ��������������nodesֻ����һ���ڵ��ҽڵ�ΪnameΪ��S��ʱ��������
     * */
    TreeNode* parse_rec(vector<TreeNode*> nodes);
};

#endif