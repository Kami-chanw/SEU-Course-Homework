/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_Read_Task_Action.cpp
//功  能：		读取动作
//开发者：		高志强
//日  期：		2022年10月08日
//更  新：		2022年10月09日
//更  新：		2022年10月15日
//更  新：		2022年10月21日
//长  度：		80行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "Planning.h"								//应用程序类头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		Read_Task_Action()
//功  能：		读取动作
//参  数：		无
//返回值：		无
void CSearchPlan::Read_Task_Action(TiXmlElement* pAction, ACTION& action)
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	//步骤1：读取动作本身
	//01 动作名称
	string sTemp = GetAttribute(pAction, "NAME");
	action.sName = CString(sTemp.c_str());

	
	///////////////////////////////////////////////////////////////////////////////////////////////
	//步骤2：读取前提条件
	TiXmlElement* pPreCondition = pAction->FirstChildElement();
	//检查前提条件是否为空
	if (pPreCondition != NULL)
	{
		//获取文字
		TiXmlElement* pLiteral = pPreCondition->FirstChildElement();

		//遍历所有文字
		while (pLiteral != NULL)
		{
			//定义文字
			LITERAL literal;
			//读取文字
			Read_Task_Literal(pLiteral, literal);
			//添加到前提条件的文字列表
			action.precondition.push_back(literal);

			//更新文字指针
			pLiteral = pLiteral->NextSiblingElement();
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//步骤3：读取动作效果
	TiXmlElement* pEffect = pPreCondition->NextSiblingElement();
	//检查动作效果指针是否为空
	if (pEffect != NULL)
	{
		//获取文字
		TiXmlElement* pLiteral = pEffect->FirstChildElement();

		//遍历所有文字
		while (pLiteral != NULL)
		{
			//定义文字
			LITERAL literal;
			//读取文字
			Read_Task_Literal(pLiteral, literal);
			//添加到动作效果的文字列表
			action.effect.push_back(literal);

			//更新文字指针
			pLiteral = pLiteral->NextSiblingElement();
		}
	}
}