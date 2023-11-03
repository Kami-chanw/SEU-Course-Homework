#include "util.h"
#include <iostream>

namespace util
{
    vector<LexRule> get_nouns()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Noun", "stench", 0.05));
        result.push_back(LexRule("Noun", "breeze", 0.1));
        result.push_back(LexRule("Noun", "glitter", 0.1));
        result.push_back(LexRule("Noun", "nothing", 0.2));
        result.push_back(LexRule("Noun", "agent", 0.2));
        result.push_back(LexRule("Noun", "wumpus", 0.15));
        result.push_back(LexRule("Noun", "pit", 0.05));
        result.push_back(LexRule("Noun", "pits", 0.05));
        result.push_back(LexRule("Noun", "gold", 0.1));
        return result;
    }

    vector<LexRule> get_verbs()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Verb", "is", 0.1));
        result.push_back(LexRule("Verb", "feel", 0.1));
        result.push_back(LexRule("Verb", "see", 0.05));
        result.push_back(LexRule("Verb", "smells", 0.05));
        result.push_back(LexRule("Verb", "shoot", 0.1));
        result.push_back(LexRule("Verb", "feels", 0.1));
        result.push_back(LexRule("Verb", "stinks", 0.05));
        result.push_back(LexRule("Verb", "go", 0.1));
        result.push_back(LexRule("Verb", "grab", 0.1));
        result.push_back(LexRule("Verb", "carry", 0.05));
        result.push_back(LexRule("Verb", "kill", 0.05));
        result.push_back(LexRule("Verb", "turn", 0.05));
        return result;
    }

    vector<LexRule> get_adjs()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Adjective", "right", 0.1));
        result.push_back(LexRule("Adjective", "left", 0.1));
        result.push_back(LexRule("Adjective", "up", 0.1));
        result.push_back(LexRule("Adjective", "down", 0.1));
        result.push_back(LexRule("Adjective", "east", 0.1));
        result.push_back(LexRule("Adjective", "south", 0.1));
        result.push_back(LexRule("Adjective", "dead", 0.2));
        result.push_back(LexRule("Adjective", "back", 0.1));
        result.push_back(LexRule("Adjective", "smelly", 0.05));
        result.push_back(LexRule("Adjective", "breezy", 0.05));
        return result;
    }

    vector<LexRule> get_advs()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Adverb", "here", 0.25));
        result.push_back(LexRule("Adverb", "there", 0.25));
        result.push_back(LexRule("Adverb", "nearby", 0.25));
        result.push_back(LexRule("Adverb", "ahead", 0.25));
        return result;
    }

    vector<LexRule> get_pronouns()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Pronoun", "me", 0.25));
        result.push_back(LexRule("Pronoun", "you", 0.1));
        result.push_back(LexRule("Pronoun", "I", 0.25));
        result.push_back(LexRule("Pronoun", "it", 0.1));
        result.push_back(LexRule("Pronoun", "he", 0.1));
        result.push_back(LexRule("Pronoun", "she", 0.1));
        result.push_back(LexRule("Pronoun", "they", 0.1));
        return result;
    }

    vector<LexRule> get_rel_pronous()
    {
        vector<LexRule> result;
        result.push_back(LexRule("RelPro", "that", 0.4));
        result.push_back(LexRule("RelPro", "which", 0.15));
        result.push_back(LexRule("RelPro", "who", 0.2));
        result.push_back(LexRule("RelPro", "whom", 0.05));
        result.push_back(LexRule("RelPro", "where", 0.2));
        return result;
    }

    vector<LexRule> get_names()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Name", "John", 0.25));
        result.push_back(LexRule("Name", "Mary", 0.25));
        result.push_back(LexRule("Name", "Boston", 0.25));
        result.push_back(LexRule("Name", "Aristotle", 0.25));
        return result;
    }

    vector<LexRule> get_articles()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Article", "the", 0.4));
        result.push_back(LexRule("Article", "a", 0.3));
        result.push_back(LexRule("Article", "an", 0.1));
        result.push_back(LexRule("Article", "every", 0.2));
        return result;
    }

    vector<LexRule> get_preps()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Prep", "to", 0.4));
        result.push_back(LexRule("Prep", "in", 0.25));
        result.push_back(LexRule("Prep", "on", 0.25));
        result.push_back(LexRule("Prep", "near", 0.1));
        return result;
    }

    vector<LexRule> get_conjs()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Conj", "and", 0.6));
        result.push_back(LexRule("Conj", "or", 0.1));
        result.push_back(LexRule("Conj", "but", 0.2));
        result.push_back(LexRule("Conj", "yet", 0.1));
        return result;
    }

    vector<LexRule> get_digits()
    {
        vector<LexRule> result;
        result.push_back(LexRule("Digit", "0", 0.1));
        result.push_back(LexRule("Digit", "1", 0.1));
        result.push_back(LexRule("Digit", "2", 0.1));
        result.push_back(LexRule("Digit", "3", 0.1));
        result.push_back(LexRule("Digit", "4", 0.1));
        result.push_back(LexRule("Digit", "5", 0.1));
        result.push_back(LexRule("Digit", "6", 0.1));
        result.push_back(LexRule("Digit", "7", 0.1));
        result.push_back(LexRule("Digit", "8", 0.1));
        result.push_back(LexRule("Digit", "9", 0.1));
        return result;
    }

    map<string, vector<LexRule> > get_dict()
    {
        map<string, vector<LexRule> > result;
        result["Noun"] = get_nouns();
        result["Verb"] = get_verbs();
        result["Adjective"] = get_adjs();
        result["Adverb"] = get_advs();
        result["Pronoun"] = get_pronouns();
        result["RelPro"] = get_rel_pronous();
        result["Name"] = get_names();
        result["Article"] = get_articles();
        result["Prep"] = get_preps();
        result["Conj"] = get_conjs();
        result["Digit"] = get_digits();
        return result;
    }

    vector<CFGRule> get_sentence_rules()
    {
        vector<CFGRule> result;
        result.push_back(CFGRule("S", { "NP", "VP" }, 0.9));
        result.push_back(CFGRule("S", { "S", "Conj", "S" }, 0.1));
        return result;
    }

    vector<CFGRule> get_np_rules()
    {
        vector<CFGRule> result;
        result.push_back(CFGRule("NP", { "Pronoun" }, 0.3));
        result.push_back(CFGRule("NP", { "Name" }, 0.1));
        result.push_back(CFGRule("NP", { "Noun" }, 0.1));
        result.push_back(CFGRule("NP", { "Article", "Noun" }, 0.25));
        result.push_back(CFGRule("NP", { "Article", "Adjs", "Noun" }, 0.05));
        result.push_back(CFGRule("NP", { "Digit", "Digit" }, 0.05));
        result.push_back(CFGRule("NP", { "NP", "PP" }, 0.1));
        result.push_back(CFGRule("NP", { "NP", "RelClause" }, 0.05));
        return result;
    }

    vector<CFGRule> get_vp_rules()
    {
        vector<CFGRule> result;
        result.push_back(CFGRule("VP", { "Verb" }, 0.4));
        result.push_back(CFGRule("VP", { "VP", "NP" }, 0.35));
        result.push_back(CFGRule("VP", { "VP", "Adjective" }, 0.05));
        result.push_back(CFGRule("VP", { "VP", "PP" }, 0.1));
        result.push_back(CFGRule("VP", { "VP", "Adverb" }, 0.1));
        return result;
    }

    vector<CFGRule> get_adjs_rules()
    {
        vector<CFGRule> result;
        result.push_back(CFGRule("Adjs", { "Adjective" }, 0.8));
        result.push_back(CFGRule("Adjs", { "Adjective", "Adjs" }, 0.2));
        return result;
    }

    vector<CFGRule> get_pp_rules()
    {
        return { CFGRule("PP", {"Prep", "NP"}, 1.0) };
    }

    vector<CFGRule> get_relclause_rules()
    {
        return { CFGRule("RelClause", {"RelPro", "VP"}, 1.0) };
    }

    map< string, vector<CFGRule> > get_grammar()
    {
        map< string, vector<CFGRule> > result;
        result["S"] = get_sentence_rules();
        result["NP"] = get_np_rules();
        result["VP"] = get_vp_rules();
        result["Adjs"] = get_adjs_rules();
        result["PP"] = get_pp_rules();
        result["RelClause"] = get_relclause_rules();
        return result;
    }

    void print_dict(map<string, vector<LexRule> >& dictionary)
    {
        for (auto it = dictionary.begin(); it != dictionary.end(); it++)
        {
            std::cout << it->first << std::endl;
            for (LexRule lex : it->second)
                std::cout << '\t' << lex.lexicon << std::endl;
        }
    }

    void print_grammar(map< string, vector<CFGRule> >& grammar)
    {
        for (auto it = grammar.begin(); it != grammar.end(); it++)
        {
            for (CFGRule rule : it->second)
            {
                std::cout << rule.lhs << ": ";
                for (string ext : rule.rhs) std::cout << ext << " ";
                std::cout << std::endl;
            }
        }
    }

    void print_tokens(vector<string>& tokens)
    {
        std::cout << "{";
        for (unsigned int i = 0; i < tokens.size(); i++)
        {
            if (i == tokens.size() - 1) std::cout << tokens[i];
            else std::cout << tokens[i] << ", ";
        }
        std::cout << "}" << std::endl;
    }

    void print_nodes(vector<TreeNode*>& nodes)
    {
        vector<string> pos;
        for (TreeNode* node : nodes)
        {
            pos.push_back(node->name);
        }
        util::print_tokens(pos);
    }

    void print_spacer(int number)
    {
        for (int i = 0; i < number; i++)
        {
            std::cout << '\t';
        }
    }

    void print_tree(TreeNode* root, int depth)
    {
        if (root == nullptr)
        {
            std::cout << "parse failed" << std::endl;
            return;
        }

        if (depth == 0) std::cout << root->name << std::endl;

        for (TreeNode* child : root->children)
        {
            print_spacer(depth + 1);
            std::cout << child->name;
            if (!child->token.empty()) std::cout << ": " << child->token;
            std::cout << std::endl;
            print_tree(child, depth + 1);
        }
    }

    void get_tree_linear(TreeNode* root, int depth, string& result)
    {
        if (depth == 0) result += '[' + root->name + ':';

        for (TreeNode* child : root->children)
        {
            result += '[' + child->name + ':';
            if (!child->token.empty()) result += ' ' + child->token;
            get_tree_linear(child, depth + 1, result);
            result += ']';
        }

        if (depth == 0) result += ']';
    }

    void print_tree_linear(TreeNode* root, int depth)
    {
        if (root == nullptr)
        {
            std::cout << "parse failed" << std::endl;
            return;
        }
        string result;
        get_tree_linear(root, 0, result);
        std::cout << result << std::endl;
    }

    void print_prob_table(map< string, vector< vector<double> > > table)
    {
        for (auto it = table.begin(); it != table.end(); it++)
        {
            std::cout << it->first << ": ";
            for (unsigned int i = 0; i < it->second.size(); i++)
            {
                std::cout << '[';
                for (unsigned int j = 0; j < it->second[i].size(); j++)
                {
                    std::cout << it->second[i][j];
                    if (j != it->second[i].size() - 1) std::cout << ',';
                }
                std::cout << ']';
            }
            std::cout << std::endl;
        }
    }
}