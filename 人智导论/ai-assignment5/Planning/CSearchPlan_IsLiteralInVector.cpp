/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_IsLiteralInVector.cpp
//功  能：		检查文字是否在向量
//开发者：		高志强
//日  期：		2022年10月16日
//更  新：		
//长  度：		90行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		IsLiteralInVector()
//功  能：		检查文字是否在向量中
//参  数：		LITERAL&,set<LITERAL>&
//返回值：		bool
bool CSearchPlan::IsLiteralInVector(LITERAL& literal,
	vector<LITERAL>& Literals)
{
	//遍历所有文字进行查找
	for (unsigned int i = 0; i < Literals.size(); i++)
	{
		//检查两个文字是否相等
		if (literal.sName == Literals[i].sName && literal.bPolarity == Literals[i].bPolarity)
		{
			//返回真
			return true;
		}
	}
	
	//缺省返回假
	return false;
}