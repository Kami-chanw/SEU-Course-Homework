#pragma once
#include <vector>
#include <string>

using namespace std;

class Example
{
public:
    Example();
	Example(vector<string> data, bool label);
	vector<string> data;	// ���ڱ�����������
	bool label;	// ���ڱ������ݱ�ǩ
};