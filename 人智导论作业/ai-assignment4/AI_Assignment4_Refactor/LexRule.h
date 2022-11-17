#ifndef lex_rule_h
#define lex_rule_h
#include <string>

using namespace std;

/**
 * This class represents the lexical rule.
 * Reference: Figure 23.1, the lexicon for E0
 * e.g. "Artile": "the".
 * */
class LexRule
{
public:
    LexRule(string pos, string lexicon, double prob);
    string pos; // the part of speech tag of this rule. e.g. "Article".
    string lexicon; // the lexicon of this tag. e.g. "the"
    double prob;
};

#endif