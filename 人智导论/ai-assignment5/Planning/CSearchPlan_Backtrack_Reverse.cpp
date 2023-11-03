/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_BackTrack_Reverse.cpp
//功  能：		回溯得到规划。用于逆向搜索
//开发者：		高志强
//日  期：		2022年10月09日
//更  新：		
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		BackTrack_Reverse()
//功  能：		回溯得到规划，用于逆向搜索
//参  数：		unsigned int
//				map<unsigned int, EDGE> Tr;
//返回值：		无
void CSearchPlan::BackTrack_Reverse(unsigned int nEndNodeID, map<unsigned int, EDGE>& Tr)
{
	//查找当前边
	map<unsigned int, EDGE>::iterator it = Tr.find(nEndNodeID);
	while (it != Tr.end())
	{
		//将动作添加到规划
		m_Plan.push_back(it->second.sActionName);

		//继续查回溯
		it = Tr.find(it->second.nStartID);
	}
}