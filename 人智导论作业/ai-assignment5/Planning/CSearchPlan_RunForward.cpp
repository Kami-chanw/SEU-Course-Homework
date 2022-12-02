/////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		CSearchPlan_RunForward.cpp
// 功  能：		基于前向搜索的规划
// 开发者：		高志强
// 日  期：		2022年10月09日
// 更  新：		2022年10月12日
// 更  新：		2022年10月21日
// 长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "CSearchPlan.h"  //基于搜索的规划类头文件
#include "stdafx.h"       //MFC标准头文件
#include <ranges>
#include <algorithm>
namespace rng = std::ranges;

// 名  称：		RunForward()
// 功  能：		基于前向搜索的规划
// 参  数：		无
// 返回值：		无
void CSearchPlan::RunForward() {
    // 定义OPEN列表，元素为结点。采用广度优先
    vector<NODE> OPEN;
    // 定义CLOSED映射。元素为状态索引字符串-结点ID
    map<CString, unsigned int> CLOSED;  // 用于判断是否重复结点
    // 定义回溯树Tr。元素为终结点ID-边
    map<unsigned int, EDGE> Tr;

    ////////////////////////////////////////////////////////////////////////
    // 步骤1：初始化
    // 初始化OPEN队列
    // 定义初始状态/结点
    NODE node;
    node.state  = m_InitState;  // 初始状态
    node.nDepth = 0;            // 结点深度为0
    node.nID    = m_nStateID;   // 结点ID为0
    OPEN.push_back(node);       // 添加到OPEN列表

    ///////////////////////////////////////////////////////////////////////
    // 步骤2：开始搜索，直到OPEN列表为空
    // while (OPEN.size() != 0)
    //{
    /**
    1	OPEN列表第一个元素出队，并从列表中删除
    2	如果当前状态是目标状态：使用CSearchPlan::IsStateSmall:
    3		回溯得到规划：使用CSearchPlan::BackTrack，并return
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
         if (IsStateSmall(curr.state, m_GoalState)) {
             BackTrack(curr.nID, Tr);
         }
         else {
             if (rng::all_of(curr.state, [&](const STATE::value_type& st) { return CLOSED.count(st.first) == 0; })) {
             }
         }
     }
}