/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_IsStateEqual.cpp
//功  能：		检查2个状态是否相等
//开发者：		高志强
//日  期：		2022年10月09日
//更  新：		2022年10月12日
//更  新：		2022年10月15日
//更  新：		2022年10月21日
//长  度：		50行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		IsStateEqual()
//功  能：		检查两个状态是否相等
//参  数：		STATE&,STATE&
//返回值：		bool
bool CSearchPlan::IsStateEqual(STATE& s1, STATE& s2)
{
	//遍历状态1的所有文字
	for (map<CString, LITERAL>::iterator it = s1.begin(); it != s1.end(); it++)
	{
		//在状态2中进行查找
		if (!IsLiteralContained(it->second, s2))
		{
			//返回假
			return false;
		}
	}

	//遍历状态2的所有文字
	for (map<CString, LITERAL>::iterator it = s2.begin(); it != s2.end(); it++)
	{
		//在状态1中进行查找
		if (!IsLiteralContained(it->second, s1))
		{
			//返回假
			return false;
		}
	}
	
	//缺省返回真
	return true;
}