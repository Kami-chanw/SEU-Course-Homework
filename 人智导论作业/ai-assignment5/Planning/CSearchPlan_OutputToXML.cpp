/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan_OutputToXML.cpp
//功  能：		输出规划到XML文件
//开发者：		高志强
//日  期：		2022年10月21日
//更  新：		
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "Planning.h"								//应用程序类头文件
#include "CSearchPlan.h"							//基于搜索的规划类头文件


//声明外部变量
extern CPlanningApp theApp;


//名  称：		OutputToXML()
//功  能：		输出规划到XML文件
//参  数：		无
//返回值：		无
void CSearchPlan::OutputToXML()
{
	//生成XML文档
	TiXmlDocument doc;
	//设置文档属性
	TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "GBK", "");
	//连接描述信息到文档
	doc.LinkEndChild(pDecl);

	//创建根元素
	TiXmlElement *pRootElement = new TiXmlElement("Plan");
	//连接根元素
	doc.LinkEndChild(pRootElement);


	//遍历所有查询结果
	for (unsigned int i = 0; i < m_Plan.size(); i++)
	{
		//创建动作名称
		TiXmlElement* pAction = new TiXmlElement("Action");

		//设置动作名称
		pAction->SetAttribute("NAME",MapCStringToString(m_Plan[i]).c_str());


		//连接动作到动作列表
		pRootElement->LinkEndChild(pAction);
	}

	//获取当前工作路径
	CString sFileName = theApp.m_sWorkPath;
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\Output.xml");

	//保存XML文件
	doc.SaveFile((MapCStringToString(sFileName)).c_str());

	//释放内存
	doc.Clear();

	//自动打开文件
	ShellExecute(NULL, _T("open"), sFileName, NULL, NULL, SW_SHOWNORMAL);
}