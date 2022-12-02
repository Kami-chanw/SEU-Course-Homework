/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_BackTrack.cpp
//功  能：		回溯得到规划
//开发者：		高志强
//日  期：		2022年10月09日
//更  新：		2022年10月12日
//更  新：		2022年10月15日
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		BackTrack()
//功  能：		回溯得到规划
//参  数：		unsigned int
//				map<unsigned int, EDGE> Tr;
//返回值：		无
void CSearchPlan::BackTrack(unsigned int nEndNodeID,map<unsigned int, EDGE>& Tr)
{
	//定义临时数据结构
	vector <CString> Plan;
	//查找当前边
	map<unsigned int, EDGE>::iterator it = Tr.find(nEndNodeID);
	while (it != Tr.end())
	{
		//将动作入栈
		Plan.push_back(it->second.sActionName);

		//继续查找
		it = Tr.find(it->second.nStartID);
	}

	//逆向拷贝回动作列表
	for (int i = Plan.size()-1; i >=0; i--)
	{
		//加入到全局变量
		m_Plan.push_back(Plan[i]);
	}
}