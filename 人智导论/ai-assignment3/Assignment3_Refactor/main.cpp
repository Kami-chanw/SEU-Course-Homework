#include <iostream>
#include <cmath>
#include "DecisionTree.h"
#include "Dataset.h"
#include "util.h"

using namespace std;

int main()
{
    Dataset trainset("./dataset/train.csv");
    Dataset testset("./dataset/test.csv");
    DecisionTree dt(trainset);

    util::print_examples(trainset.examples);
    util::print_tree(dt.root, 0);
    
    vector<int> classify_res = dt.classify(testset.raw_values);
    vector<int> gold_res = util::get_labels(testset.examples);
    std::cout << "Precision: " << util::evaluate(classify_res, gold_res) << std::endl;

}
