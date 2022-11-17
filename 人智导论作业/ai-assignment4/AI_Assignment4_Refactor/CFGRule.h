#ifndef cfg_rule_h
#define cfg_rule_h

#include <vector>
#include <string>

using namespace std;

/**
 * This class represents CFG(Context Free Grammar).
 * Reference: Figure 23.2, the grammar for E0.
 * To use CYK-parsing, we need to convert CFG to CNF(Chomsky Normal Form).
 * The process of converting is described in Exercise 23.8.
 * */
class CFGRule
{
public:
    CFGRule(string lhs, vector<string> rhs, double prob);
    string lhs; // The left hand side of this rule. e.g. "NP"
    vector<string> rhs; // the right hnd side of this rule. e.g. {"Article", "Noun"}
    double prob; // for implementing CYK-parse
};

#endif