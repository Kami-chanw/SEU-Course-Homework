/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_IsLiteralContained.cpp
//功  能：		检查文字是否包含于状态
//开发者：		高志强
//日  期：		2022年10月09日
//更  新：		2022年10月12日
//更  新：		2022年10月21日
//长  度：		50行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		IsLiteralContained()
//功  能：		检查文字是否包含于状态
//参  数：		LITERAL&,STATE&
//返回值：		bool
bool CSearchPlan::IsLiteralContained(LITERAL& literal, STATE& state)
{
	//获取谓词名称
	CString sName = literal.sName;

	//检查名称-状态映射表
	map<CString, LITERAL>::iterator it = state.find(sName);
	//检查是否找到相同名字的文字
	if (it != state.end())//找到相同名字的文字
	{
		//检查极性是否相同
		if (literal.bPolarity == it->second.bPolarity)//极性相同
		{
			//返回真
			return true;
		}
		else//极性不同
		{			
			//返回假
			return false;
		}	
	}
	else//没有找到相同名字的文字
	{
		//返回假
		return false;
	}
}