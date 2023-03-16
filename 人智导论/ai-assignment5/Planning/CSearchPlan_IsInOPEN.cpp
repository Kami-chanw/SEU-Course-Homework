/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_IsInOPEN.cpp
//功  能：		检查结点是否在OPEN表中
//开发者：		高志强
//日  期：		2022年10月12日
//更  新：		2022年10月21日
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		IsInOPEN()
//功  能：		检查结点是否在OPEN表中
//参  数：		CString&,vector<NODE>&
//返回值：		无
bool CSearchPlan::IsInOPEN(CString& sIndex, vector<NODE>&OPEN)
{
	//遍历OPEN表中的所有结点
	for (unsigned int i = 0; i < OPEN.size(); i++)
	{
		//获取结点索引
		CString sNodeIndex = GetStateIndex(OPEN[i].state);

		//检查是否相等
		if (sIndex == sNodeIndex)
		{
			//返回真
			return true;
		}
	}

	//缺省返回假
	return false;
}