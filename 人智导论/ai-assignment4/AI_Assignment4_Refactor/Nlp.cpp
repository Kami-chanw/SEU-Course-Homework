#include "Nlp.h"
#include "util.h"
#include <sstream>
#include <algorithm>

Nlp::Nlp(map<string, vector<LexRule>> dictionary, map<string, vector<CFGRule>> grammar)
{
	this->dictionary = dictionary;
	this->grammar = grammar;
}

vector<string> Nlp::tokenize(string sentence)
{
	stringstream ss(sentence);
	string token;
	vector<string> result;
	while (getline(ss, token, ' '))
	{
		result.push_back(token);
	}
	return result;
}

vector<LexRule> Nlp::get_lex_rules_by(string token)
{
	vector<LexRule> result;
	for (auto it = this->dictionary.begin(); it != this->dictionary.end(); it++)
	{
		for (LexRule rule : it->second)
		{
			if (rule.lexicon == token) result.push_back(rule);
		}
	}
	return result;
}

string Nlp::get_token_pos(string token)
{
	vector<LexRule> rules = get_lex_rules_by(token);
	if (rules.empty()) return "unknown";
	return rules[0].pos;
}

vector<string> Nlp::get_sentence_pos(string sentence)
{
	vector<string> result;
	vector<string> tokens = tokenize(sentence);
	for (string token : tokens) result.push_back(get_token_pos(token));
	return result;
}

TreeNode* Nlp::greedy_match(vector<TreeNode*> nodes)
{
	vector<TreeNode*> longest;
	string node_name;

	for (auto it = grammar.begin(); it != grammar.end(); it++)
	{
		for (CFGRule rule : it->second)
		{
			if (nodes.size() < rule.rhs.size()) continue;
			if (nodes[0]->name != rule.rhs.front()) continue;

			vector<TreeNode*> current;
			for (unsigned int i = 0; i < rule.rhs.size(); i++)
			{
				if (rule.rhs[i] != nodes[i]->name)
				{
					current.clear();
					break;
				}
				current.push_back(nodes[i]);
			}
			if (longest.size() < current.size())
			{
				node_name = rule.lhs;
				longest = current;
			}
		}
	}
	if (longest.empty()) return nullptr;
	return new TreeNode(node_name, "", longest);
}

vector<vector<TreeNode*>> Nlp::get_possible_chunkings(vector<TreeNode*> nodes)
{
	/**
	 * TODO:
	 * function get_possible_chunkings(vector<TreeNode*> nodes) return vector<vector<TreeNode*>>
	 *  vector<vector<TreeNode*>> result;
	 *  for i in 0...nodes.size():
	 *      sub_nodes = get_sub_nodes(nodes, i, nodes.size());
	 *      chunked_node = greddy_match(subnodes);
	 *      if chunked_node is not null:
	 *          current = {nodes[0], ..., nodes[i], chunked_node, nodes[i + checked_node.children.size()], ...}
	 *          add current to result;
	 * return result
	 * */
	vector<vector<TreeNode*>> result;
	for (int i = 0; i < nodes.size(); ++i) {
		vector<TreeNode*> sub_nodes = get_sub_nodes(nodes, i, nodes.size());
		TreeNode* chunked_node = greedy_match(sub_nodes);
		if (chunked_node) {
			vector<TreeNode*> current;
			copy(nodes.begin(), nodes.begin() + i, back_inserter(current));
			current.push_back(chunked_node);
			copy(nodes.begin() + i + chunked_node->children.size(), nodes.end(), back_inserter(current));
			result.push_back(current);
		}
	}
	return result;
}

TreeNode* Nlp::parse(string sentence)
{
	/**
	 * TODO:
	 * function parse(string sentence) return the root node ptr of the parse tree:
	 *  tokens = tokenize(sentence)
	 *  vector<TreeNode*> leaf_nodes;
	 *  for token in tokens:
	 *      pos = get_token_pos(token);
	 *      leaf = new TreeNode(pos, token, {})
	 *      add leaf to leaf_nodes;
	 *  return parse_rec(leaf_nodes);
	 * */
	vector<string> tokens = tokenize(sentence);
	vector<TreeNode*> leaf_nodes;
	for (const string& token : tokens) {
		string pos = get_token_pos(token);
		TreeNode* leaf = new TreeNode(pos, token, {});
		leaf_nodes.push_back(leaf);
	}
	return parse_rec(leaf_nodes);
}

vector<TreeNode*> Nlp::get_sub_nodes(vector<TreeNode*> source, int begin, int end)
{
	if (end < begin) return {};
	if (begin >= (int)source.size()) return {};
	if (end >= (int)source.size()) end = source.size();

	return vector<TreeNode*>(source.begin() + begin, source.begin() + end);
}

TreeNode* Nlp::parse_rec(vector<TreeNode*> nodes)
{
	/**
	 * TODO:
	 * function parse_res(vector<TreeNode*> nodes) return the root node ptr
	 *  base condition = nodes.size() == 1 and nodex[0]->name == "S"
	 *  if base condition: return nodes[0]
	 *  chunkings = get_possible_chunkings(nodes);
	 *  # use depth first search to search for result
	 *  for chunking in chunkings:
	 *      result = parse_rec(chunking)
	 *      if result is not nullptr return result;
	 *  return nullptr;
	 * */
	if (nodes.size() == 1 && nodes[0]->name == "S")
		return nodes[0];
	auto chunkings = get_possible_chunkings(nodes);
	for (const auto& chunking : chunkings) {
		TreeNode* result = parse_rec(chunking);
		if (result)
			return result;
	}
	return nullptr;
}