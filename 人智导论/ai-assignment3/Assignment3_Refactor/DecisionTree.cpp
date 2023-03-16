#include "DecisionTree.h"
#include "Dataset.h"
#include "util.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
namespace rng = std::ranges;
DecisionTree::DecisionTree(Dataset trainset) {
    std::cout << "decision tree learning start..." << std::endl;
    this->root = learn(trainset.examples, trainset.attributes, {});
    std::cout << "decision tree learning finished." << std::endl;
}

TreeNode* DecisionTree::plurality_value(vector<Example> examples) {
    int positive = get_positive_count(examples);
    return new TreeNode(positive > examples.size() - positive);
}

vector<Example> DecisionTree::get_examples(const vector<Example>& examples, const string& attr, const string& option) {
    vector<Example> result;
    int             attr_index = util::get_attribute_index(attr);
    rng::copy_if(examples, back_inserter(result), [&](const Example& e) { return e.data[attr_index] == option; });
    return result;
}

int DecisionTree::get_positive_count(const vector<Example>& examples) {
    return rng::count_if(examples, [&](const Example& example) { return example.label == true; });
}

bool DecisionTree::have_same_class(const vector<Example>& examples) {
    return rng::all_of(examples, [&](const Example& e) { return e.label == examples[0].label; });
}

double DecisionTree::entropy(double p1, double p2) {
    if (p1 == 0 || p2 == 0)
        return 0;
    return -(p1 * std::log2(p1) + p2 * std::log2(p2));
}

double DecisionTree::entropy_binary(double p) { return entropy(p, 1 - p); }

double DecisionTree::entropy_remain(string attribute, vector<Example> examples) {
    return [&](auto&& options) {
        return accumulate(options.begin(), options.end(), 0.0, [&](double res, const string& option) {
            vector<Example> exs = get_examples(examples, attribute, option);
            return res + exs.size() * 1.0 / examples.size() * entropy_binary(get_positive_count(exs) * 1.0 / exs.size());
        });
    }(util::get_attributes_options()[attribute]);
}

double DecisionTree::info_gain(const string& attribute, const vector<Example>& examples) {
    /*
     * TODO:
     * 根据18.3.4 Choosing attribute tests中的信息增益公式计算attribute在examples上的信息增益
     * 公式：
     * Gain(attribute) = entropy_binary(p / (p + n)) - entropy_remain(attribute, examples)
     */
    using size_type = set<string>::size_type;
    auto [p, n]     = [&](const set<string>& options) -> pair<size_type, size_type> {
        return accumulate(options.begin(), options.end(), make_pair(0, 0),
                              [&](const pair<size_type, size_type>& res, const string& option) {
                              vector<Example> exs = get_examples(examples, attribute, option);
                              size_type       pos = get_positive_count(exs);
                              size_type       neg = exs.size() - pos;
                              return make_pair(res.first + pos, res.second + neg);
                          });
    }(util::get_attributes_options()[attribute]);

    return entropy_binary(p * 1.0 / (p + n)) - entropy_remain(attribute, examples);
}

string DecisionTree::importance(const vector<string>& attributes, const vector<Example>& examples) {
    /*
     * TODO:
     * 对于attributes中的每一个attribute，计算在examples上的信息增益，返回信息增益最大的attribute.
     * 例：
     * Dataset trainset("./dataset/train.csv")
     * string attr = importance(trainset.attributes, trainset.examples)
     * 此时attr应该为"PATRONS"
     * Note: attributes.size() > 0
     */
    map<double, string, greater<>> gains;
    rng::transform(attributes, insert_iterator(gains, gains.end()),
                   [&](const string& attr) { return make_pair(info_gain(attr, examples), attr); });
    return gains.begin()->second;
}

vector<string> DecisionTree::remove_attribute(const vector<string>& attributes, const string& attribute) {
    return rng::to<vector<string>>(views::filter(attributes, [&](const string& attr) { return attr != attribute; }));
}

TreeNode* DecisionTree::learn(const vector<Example>& examples, const vector<string>& attributes,
                              const vector<Example>& parent_examples) {
    /*
    * TODO:
    * 实现Figure 18.5中描述的伪代码：DECISION-TREE-LEARNING
    * function DECISION-TREE-LEARNING(examples, attributes, parent_examples) returns a tree
        if examples is empty then return PLURALITY-VALUE(parent examples)
        else if all examples have the same classification then return the classification
        else if attributes is empty then return PLURALITY-VALUE(examples)
        else
            let A = the A important attribute # use importance(attributes, examples)
            let tree = a new decision tree with tree test A
            for each value v_k of A do
                let exs = {e : e in examples and e.A = v_k}
                let subtree = DECISION-TREE-LEARNING(exs, {attributes − A}, examples)
                add a branch to tree with label (A = v_k) and subtree subtree # tree->options[v_k] = subtree
        return tree
    * NOTE: 如果信息增益函数正确，通过训练集得到的决策树的第一个分类属性应该为PATRONS
    */
    if (examples.empty())
        return plurality_value(parent_examples);
    if (have_same_class(examples))
        return new TreeNode(examples[0].label);
    if (attributes.empty())
        return plurality_value(examples);
    string    A = importance(attributes, examples);
    TreeNode* tree = new TreeNode(A);
    rng::for_each(util::get_attributes_options()[A], [&](const string& v_k) {
        vector<Example> exs     = get_examples(examples, A, v_k);
        TreeNode*       subtree = learn(exs, remove_attribute(attributes, A), examples);
        tree->options[v_k]      = subtree;
    });
    return tree;
}

int DecisionTree::classify_rec(const Example& example, TreeNode* root) {
    /*
     * TODO:
     * 实现决策树的递归预测函数
     * Algorithm:
     * function classify_rec(example, tree) return an integer which indicates the label of the given example
     *   if tree->attribute == util::LABEL_ATTRIBUTE then return tree->value;
     *
     *   let index = the index of tree->attribute    # use util::get_attribute_index(attribute)
     *   let v_k = example.data[index]
     *   let child = tree->options[v_k]
     *   return classify_rec(example, child)
     */
    if (root->attribute == util::LABEL_ATTRIBUTE)
        return root->value;
    return classify_rec(example, root->options[example.data[util::get_attribute_index(root->attribute)]]);
}

vector<int> DecisionTree::classify(const vector<vector<string>>& test_raw_values) {
    vector<int> result;
    rng::transform(Dataset(test_raw_values).examples, back_inserter(result),
                   [&](const Example& e) { return classify_rec(e, root); });
    return result;
}