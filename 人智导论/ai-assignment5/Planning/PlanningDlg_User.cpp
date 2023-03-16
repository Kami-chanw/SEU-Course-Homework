/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		PlanningDlg_User.cpp
//功  能：		响应用户命令
//开发者：		高志强
//日  期：		2022年10月05日
//更  新：		2022年10月09日
//更  新：		2022年10月21日
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"				//MFC标准头文件	
#include "Planning.h"			//应用程序类头文件
#include "PlanningDlg.h"		//主对话框类头文件
#include "afxdialogex.h"		//MFC扩展对话框类头文件
#include "CSearchPlan.h"		//基于搜索的规划方法头文件


//名  称：		OnBnClickedButtonForwardSearch()
//功  能：		响应用户命令、基于前向搜索的规划方法
//参  数：		无
//返回值：		无
void CPlanningDlg::OnBnClickedButtonForwardSearch()
{
	//定义基于搜索规划类的对象
	CSearchPlan theSearchPlan;

	//初始化
	theSearchPlan.Init();

	//运行前向搜索的规划
	theSearchPlan.RunForward();
	
	//输出规划
	theSearchPlan.OutputToXML();
}


//名  称：		OnBnClickedButtonBackwardSearch()
//功  能：		响应用户命令、基于逆向搜索的规划方法
//参  数：		无
//返回值：		无
void CPlanningDlg::OnBnClickedButtonBackwardSearch()
{
	//定义基于搜索规划类的对象
	CSearchPlan theSearchPlan;

	//初始化
	theSearchPlan.Init();

	//运行逆向搜索的规划
	theSearchPlan.RunBackward();

	//输出规划
	theSearchPlan.OutputToXML();
}
