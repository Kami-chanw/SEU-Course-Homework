#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
#include "Example.h"

using namespace std;

/*
* ���ڴ������ݼ�
*/
class Dataset
{
public:
	Dataset(string filename);	// ʹ���ļ����г�ʼ��
	Dataset(vector<vector<string>> row);	// ʹ�������б���г�ʼ��

	vector<string> attributes;	// ���ݼ��������б�
	vector<vector<string>> raw_values;	// �����б�ÿ�����ݰ���11�����������һ������Ϊ��ǩ
	vector<Example> examples;	// �����ݱ���ΪExample����ʽ
	vector<string> get_values(string line);	// ��ȡһ�����ݣ�line�а���11���������Զ��ŷָ�
	bool get_label(string value);	// ��ȡ��ǩ��boolֵ��"1"Ϊtrue
	Example build_example(vector<string> values);	// ��vector<string>�����ݸ�ʽת��ΪExample
};

