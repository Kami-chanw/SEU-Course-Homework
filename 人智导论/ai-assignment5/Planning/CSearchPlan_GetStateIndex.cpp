/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_GetStateIndex.cpp
//功  能：		获取状态索引
//开发者：		高志强
//日  期：		2022年10月15日
//更  新：		2022年10月21日
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		getStateIndex()
//功  能：		获取状态索引
//参  数：		STATE&
//返回值：		CString
CString CSearchPlan::GetStateIndex(STATE& state)
{
	//定义索引
	CString sIndex;

	//遍历所有文字
	for (map<CString, LITERAL>::iterator it = state.begin(); it != state.end(); it++)
	{
		//检查文字的极性
		if (it->second.bPolarity == true)
		{
			//获取获取索引。注意空格
			sIndex += _T(" +") + it->first;
		}
		else
		{
			//获取索引。注意空格
			sIndex += _T(" -") + it->first;
		}
	}

	//返回索引
	return sIndex;
}