/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_Expand_Reverse.cpp
//功  能：		展开后继结点。用于逆向搜索
//开发者：		高志强
//日  期：		2022年10月16日
//更  新：		2022年10月21日
//长  度：		110行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		Expand_Reverse()
//功  能：		反向展开后继状态
//参  数：		无
//返回值：		无
void CSearchPlan::Expand_Reverse(NODE& curr_node,
	vector<NODE>& OPEN,
	map<CString, unsigned int>& CLOSED,
	map<unsigned int, EDGE>&Tr)
{
	/**
	1	遍历(a)所有的可能动作，检查动作是否可行。对于每个当前动作:
	2		定义变量bSatisfied：保存动作效果是否满足要求
	3		定义去除的效果文字集：vector<LITERAL> Effects;
	4		遍历(b)所有的动作效果:
	5			检查谓词是否满足当前状态：使用CSPSearchPlan::IsLiteralContained:
	6				如果满足，更新变量bSatisfied值，并添加效果文字到效果文字集
	7		如果动作效果满足要求：
	8			定义前驱状态：STATE prev_state;
	9			遍历(c)当前节点的每个状态:
	10				检查状态的文字是否在效果文字集中：使用CSearchPlan::IsLiteralInVector:
	11					如果不在，则添加到前驱状态
	12			遍历(d)当前动作的前提:
	13				获取前提的文字名称，并添加到前驱状态
	14			获取索引：使用CSearchPlan::GetStateIndex
	15			检查索引是否已经出现在CLOSED表
	16			如果状态已经出现过，且不在OPEN表中：
	17				设置前驱节点
	18				更新状态/节点ID计数
	19				将后继结点加入OPEN列表
	20				定义边并添加边到回溯树
	**/

	// TODO
}