/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_IsStateContainedIn.cpp
//功  能：		检查第1个状态是否包含于第2个状态
//开发者：		高志强
//日  期：		2022年10月09日
//更  新：		
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		IsStateContained()
//功  能：		检查第1个状态是否包含于第2个状态
//参  数：		STATE&,STATE&
//返回值：		bool
bool CSearchPlan::IsStateContainedIn(STATE& s1, STATE& s2)
{
	//步骤1：异常情况处理。状态1的谓词映射表为空
	if (s1.Name2Predicates.size() == 0)
	{
		//返回真
		return true;
	}


	//步骤2：遍历状态1的谓词，并在状态2中检查是否兼容
	for (map<CString, PREDICATE>::iterator it = s1.Name2Predicates.begin(); it != s1.Name2Predicates.end(); it++)
	{
		//获取当前谓词
		PREDICATE predicate = it->second;

		//检查当前谓词是否和状态2相容
		if (!IsCompatible(predicate, s2))
		{
			//返回假
			return false;
		}
	}

	//缺省返回真
	return true;
}