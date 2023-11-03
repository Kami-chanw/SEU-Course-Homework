#ifndef util_h
#define util_h

#include <vector>
#include <map>
#include <string>
#include "CFGRule.h"
#include "TreeNode.h"
#include "LexRule.h"

using namespace std;

namespace util
{
    vector<LexRule> get_nouns();
    vector<LexRule> get_verbs();
    vector<LexRule> get_adjs();
    vector<LexRule> get_advs();
    vector<LexRule> get_pronouns();
    vector<LexRule> get_rel_pronous();
    vector<LexRule> get_names();
    vector<LexRule> get_articles();
    vector<LexRule> get_preps();
    vector<LexRule> get_conjs();
    vector<LexRule> get_digits();
    map< string, vector<LexRule> > get_dict();
    map< string, vector<CFGRule> > get_grammar();

    void print_dict(map< string, vector<LexRule> >& dictionary);
    void print_grammar(map< string, vector<CFGRule> >& grammar);
    void print_tokens(vector<string>& tokens);
    void print_nodes(vector<TreeNode*>& nodes);
    void print_tree(TreeNode* root, int depth);
    /**
     * 用于获取作业要求的解析树字符串格式。
     * 例：
        TreeNode* root = nlp.parse("the wumpus is dead");
        string result;
        get_tree_linear(root, 0, result);
        std::cout << result << std::endl;
     * 控制台输出：
     [S:[NP:[Article: the][Noun: wumpus]][VP:[VP:[Verb: is]][Adjective: dead]]]
     * */
    void get_tree_linear(TreeNode* root, int depth, string& result);
    void print_tree_linear(TreeNode* root, int depth);
    void print_prob_table(map< string, vector< vector<double> > > table);
}

#endif