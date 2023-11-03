/////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		CSearchPlan_Expand.cpp
// 功  能：		展开后继结点
// 开发者：		高志强
// 日  期：		2022年10月09日
// 更  新：		2022年10月12日
// 更  新：		2022年10月15日
// 更  新：		2022年10月21日
// 长  度：		90行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"       //MFC标准头文件
#include "CSearchPlan.h"  //基于搜索的规划类头文件

// 名  称：		Expand()
// 功  能：		展开后继状态
// 参  数：		无
// 返回值：		无
void CSearchPlan::Expand(NODE& curr_node, vector<NODE>& OPEN, map<CString, unsigned int>& CLOSED, map<unsigned int, EDGE>& Tr) {
    /**
    1	遍历(a)所有的可能动作，检查动作是否可行。对于每个当前动作:
    2		定义变量bPreCondSatisfied：保存前提条件是否满足
    3		遍历(b)所有前提条件:
    4			检查文字是否包含于当前状态：使用CSPSearchPlan::IsLiteralContained:
    5				如果发现不包含，更新变量bPreCondSatisfied值，并退出遍历(b)
    6		如果当前动作的前提条件满足：
    7			定义后继状态
    8			添加动作效果
    9			获取后继状态索引
    10			检查索引是否已经出现在CLOSED表
    11			如果状态没有出现过，且不在OPEN表中：
    12				设置后继节点
    13				更新状态/节点ID计数
    14				将后继结点加入OPEN列表
    15				定义边并添加边到回溯树
    **/
    for (ACTION& action : m_Actions) {
        if (ranges::all_of(action.precondition, [&](LITERAL& cond) { return IsLiteralContained(cond, curr_node.state); })) {
            STATE suc_state = curr_node.state;
            ranges::copy(action.effect | views::transform([](const LITERAL& li) { return pair(li.sName, li); }),
                         insert_iterator(suc_state, suc_state.end()));
            CString idx = GetStateIndex(suc_state);
            if (CLOSED.find(idx) == CLOSED.end() && !IsInOPEN(idx, OPEN)) {
                OPEN.emplace_back(suc_state, curr_node.nDepth + 1, m_nStateID);
                /*Tr.emplace(
                    m_nStateID, EDGE { // invalid in c++, due to CWG : P0329R4
                        .nEndID      = m_nStateID,
                        .nStartID    = curr_node.nID,
                        .sActionName = action.sName
                    });*/
                Tr.emplace(m_nStateID, EDGE{ curr_node.nID, m_nStateID, action.sName });
                m_nStateID++;
            }
        }
    }
}