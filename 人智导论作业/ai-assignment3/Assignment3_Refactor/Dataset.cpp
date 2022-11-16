#include "Dataset.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Dataset::Dataset(string filename)
{
    ifstream inFile(filename);
    if (inFile.fail())
    {
        cout << "cannot read file: " + filename << endl;
    }

    string line;
    int row = 0;
    while (getline(inFile, line))
    {
        if (row == 0) this->attributes = get_values(line);  // 数据文件的第一行为属性标签
        else
        {
            this->raw_values.push_back(get_values(line));
            vector<string> values = get_values(line);
            this->examples.push_back(build_example(values));
        }
        row++;
    }
    if (this->attributes.size() == 11) this->attributes.pop_back();
}

Dataset::Dataset(vector<vector<string>> row)
{
    this->attributes = util::get_attributes();
    this->raw_values = row;
    for (vector<string> values : row)
    {
        this->examples.push_back(build_example(values));
    }
}

vector<string> Dataset::get_values(string line)
{
    stringstream ss(line);
    string value;
    vector<string> values;
    while (getline(ss, value, ','))
    {
        values.push_back(value);
    }
    return values;
}

bool Dataset::get_label(string value)
{
    if (value == "1") return true;
    return false;
}

Example Dataset::build_example(vector<string> values)
{
    if (values.size() == 11)
    {
        bool label = get_label(values.back());
        values.pop_back();
        return Example(values, label);
    }
    if (values.size() == 10)
    {
        return Example(values, false);
    }

    return Example({}, false);
}
