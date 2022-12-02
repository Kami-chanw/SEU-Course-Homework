/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_Read_Task_Literal.cpp
//功  能：		读取文字
//开发者：		高志强
//日  期：		2022年10月08日
//更  新：		2022年10月09日
//更  新：		2022年10月12日
//更  新：		2022年10月21日
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "Planning.h"								//应用程序类头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//名  称：		Read_Task_Literal()
//功  能：		读取文字
//参  数：		TiXmlElement*,LITERAL&
//返回值：		无
void CSearchPlan::Read_Task_Literal(TiXmlElement* pLiteral, LITERAL& literal)
{
	//01 文字名称
	string sTemp = GetAttribute(pLiteral, "NAME");
	literal.sName = CString(sTemp.c_str());

	//02 文字极性
	sTemp = GetAttribute(pLiteral, "POLARITY");
	if (sTemp == "true")
	{
		//极性为真，转换为bool类型
		literal.bPolarity = true;
	}
	else
	{
		//极性为假，转换为bool类型
		literal.bPolarity = false;
	}
}