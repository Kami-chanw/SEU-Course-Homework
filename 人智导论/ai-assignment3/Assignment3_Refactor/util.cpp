#include "util.h"
#include <iostream>

namespace util
{
    vector<string> get_attributes()
    {
        vector<string> attributes;
        attributes.push_back("ALTERNATE");
        attributes.push_back("BAR");
        attributes.push_back("FRI/SAT ");
        attributes.push_back("HUNGRY");
        attributes.push_back("PATRONS");
        attributes.push_back("PRICE");
        attributes.push_back("RAINING");
        attributes.push_back("RESERVATION");
        attributes.push_back("TYPE");
        attributes.push_back("WAITESTIMATE");
        return attributes;
    }

    map<string, set<string>> get_attributes_options()
    {
        map<string, set<string>> result;
        result["ALTERNATE"] = {"0", "1"};
        result["BAR"] = { "0", "1" };
        result["FRI/SAT "] = { "0", "1" };
        result["HUNGRY"] = { "0", "1" };
        result["PATRONS"] = { "0", "0.5", "1" };
        result["PRICE"] = { "0", "0.5", "1" };
        result["RAINING"] = { "0", "1" };
        result["RESERVATION"] = { "0", "1" };
        result["TYPE"] = { "0", "0.33", "0.67", "1" };
        result["WAITESTIMATE"] = { "0", "0.17", "0.5", "1" };
        result[LABEL_ATTRIBUTE] = { "0", "1" };
        return result;
    }

    int get_attribute_index(string attribute)
    {
        vector<string> attributes = get_attributes();
        for (unsigned int i = 0; i < attributes.size(); i++)
        {
            if (attributes[i] == attribute) return i;
        }
        return -1;
    }

    vector<int> get_labels(vector<Example> examples)
    {
        vector<int> result;
        for (Example example : examples)
        {
            if (example.label) result.push_back(1);
            else result.push_back(0);
        }
        return result;
    }

    double evaluate(vector<int> pred, vector<int> gold)
    {
        int correct = 0;
        for (unsigned int i = 0; i < pred.size(); i++)
        {
            if (pred[i] == gold[i]) correct++;
        }
        return (double)correct / (double)pred.size();
    }

    void print_attributes(vector<string>& attributes)
    {
        for (string attribute : attributes)
        {
            std::cout << attribute << " ";
        }
        std::cout << std::endl;
    }

    void print_example(Example& example)
    {
        std::cout << "data:  ";
        for (string value : example.data)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        std::cout << "label: ";
        std::cout << example.label << std::endl;
    }

    void print_examples(vector<Example>& examples)
    {
        for (Example example : examples)
        {
            util::print_example(example);
        }
    }

    void print_raw_values(vector<vector<string>> raw_values)
    {
        for (vector<string> row : raw_values)
        {
            for (string value : row)
            {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_options(map<string, set<string>>& options)
    {
        for (auto it = options.begin(); it != options.end(); ++it)
        {
            std::cout << it->first << ": ";
            for (string value : it->second)
            {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
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
        if (root->options.empty())
        {
            print_spacer(depth);
            std::cout << root->attribute << ": " << root->value << std::endl;
            return;
        }
        print_spacer(depth);
        std::cout << "Split on " << root->attribute << std::endl;
        for (auto it = root->options.begin(); it != root->options.end(); ++it)
        {
            print_spacer(depth);
            std::cout << "If " << root->attribute << " == " << it->first << std::endl;
            print_tree(it->second, depth + 1);
        }
    }


}

