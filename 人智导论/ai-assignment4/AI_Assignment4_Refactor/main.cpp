#include <iostream>
#include "util.h"
#include "CFGRule.h"
#include "TreeNode.h"
#include "Nlp.h"
#include "LexRule.h"

using namespace std;

void parsing_test(Nlp& nlp, string sentence)
{
    TreeNode* root = nlp.parse(sentence);
    util::print_tree(root, 0);
    util::print_tree_linear(root, 0);
}

int main()
{
    map< string, vector<LexRule> > dictionary = util::get_dict();
    map< string, vector<CFGRule> > grammar = util::get_grammar();
    Nlp nlp(dictionary, grammar);
    string s1 = "the wumpus is dead";
    string s2 = "the wumpus who smells is dead";
    string s3 = "the agent feels the breeze on 0 8";
    parsing_test(nlp, s1);
    parsing_test(nlp, s2);
    parsing_test(nlp, s3);

    return EXIT_SUCCESS;
}