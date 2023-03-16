/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_Read_Task.cpp
//功  能：		读取规划任务，包括初始状态、目标状态、允许的动作列表等
//开发者：		高志强
//日  期：		2022年10月07日
//更  新：		2022年10月08日
//更  新：		2022年10月09日
//更  新：		2022年10月21日
//长  度：		120行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "Planning.h"								//应用程序类头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件



//声明外部变量
extern CPlanningApp theApp;


//名  称：		Read_Task()
//功  能：		读取规划任务。包括目标状态、动作列表等
//参  数：		无
//返回值：		无
void CSearchPlan::Read_Task()
{
	//获取当前工作路径
	CString sFileName = CString(theApp.m_sWorkPath);
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\shoe.xml");


	//搜索文件，如果文件不存在，则直接返回
	CFileFind findWenJian;								//文件名称
	BOOL bWenJian = findWenJian.FindFile(sFileName);	//搜索文件是否存在
	//如果没有发现文件，则直接返回
	if (!bWenJian)
	{
		//提示
		AfxMessageBox(_T("穿袜穿鞋任务shoe.xml不存在"));

		//返回
		return;
	}


	//打开文件
	TiXmlDocument aDoc(MapCStringToString(sFileName).c_str());

	//检查打开文件是否成功
	if (!aDoc.LoadFile())
	{
		//提示
		AfxMessageBox(_T("shoe.xml失败:"));

		//退出
		return exit(0);
	}

	//获取根结点PLANNING
	TiXmlElement *pRoot = aDoc.RootElement();


	////////////////////////////////////////////////////////////////////////////////////////////////
	//步骤01：获取目标状态。初始状态为空
	TiXmlElement* pGoal = pRoot->FirstChildElement();
	if (pGoal != NULL)
	{
		//获取文字
		TiXmlElement* pLiteral = pGoal->FirstChildElement();
		//遍历所有文字
		while (pLiteral != NULL)
		{
			//定义文字
			LITERAL literal;

			//读取文字
			Read_Task_Literal(pLiteral, literal);

			//添加到目标状态的谓词名称-文字映射
			m_GoalState.insert(make_pair(literal.sName,literal));


			//更新文字指针
			pLiteral = pLiteral->NextSiblingElement();
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////
	//步骤2：获取允许的动作列表
	TiXmlElement* pActions = pGoal->NextSiblingElement();
	if (pActions != NULL)
	{
		//获取动作指针
		TiXmlElement* pAction = pActions->FirstChildElement();

		//检查动作指针是否为空
		while (pAction != NULL)
		{
			//获取动作
			ACTION action;

			//读取动作
			Read_Task_Action(pAction, action);
			//添加到允许的动作列表
			m_Actions.push_back(action);

	
			//更新动作指针
			pAction = pAction->NextSiblingElement();
		}	
	}


	//释放文件所占内存
	aDoc.Clear();
}