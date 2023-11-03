/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_RunBackward.cpp
//功  能：		基于逆向搜索的规划
//开发者：		高志强
//日  期：		2022年10月16日
//更  新：		2022年10月21日
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		RunBackward()
//功  能：		反向搜索
//参  数：		无
//返回值：		无
void CSearchPlan::RunBackward()
{
	//定义OPEN列表
	vector<NODE> OPEN;
	//定义CLOSED映射
	map <CString, unsigned int> CLOSED;			
	//定义回溯树Tr
	map<unsigned int, EDGE> Tr;


	////////////////////////////////////////////////////////////////////////
	//步骤1：初始化
	//初始化OPEN队列	
	NODE node;
	node.state = m_GoalState;	//结点状态
	node.nDepth = 0;			//结点深度
	node.nID = m_nStateID;		//结点ID
	OPEN.push_back(node);


	///////////////////////////////////////////////////////////////////////
	//步骤2：开始搜索，直到OPEN表为空
	//while (OPEN.size() != 0)
	//{
		/**
		1	OPEN列表第一个元素出队，并从列表中删除
		2	如果当前状态是初始状态：使用CSearchPlan::IsStateEqual:
		3		回溯得到规划：使用CSearchPlan::BackTrack_Reverse，并return
		4	否则，继续搜索:
		5		如果当前状态在CLOSED映射中不存在:
		6			将当前结点添加到CLOSED
		7			如果结点ID是0，则自增
		8			展开后继状态到OPEN表：使用CSearchPlan::Expand
		**/

		// TODO
	//}

	while (OPEN.size()) {
        NODE curr = OPEN.front();
        OPEN.erase(OPEN.begin());  // WTF?? OPEN不应为vector，否则就是依托答辩
        if (IsStateEqual(m_InitState, curr.state)) {
            BackTrack_Reverse(curr.nID, Tr);
            return;
        }
        else {
            CString idx = GetStateIndex(curr.state);
            if (CLOSED.find(idx) == CLOSED.end()) {
                CLOSED.emplace(idx, m_nStateID);
                if (m_nStateID == 0)
                    m_nStateID++;
                Expand_Reverse(curr, OPEN, CLOSED, Tr);
            }
        }
    }
}