/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Planning.h
//功  能：		应用程序类头文件
//开发者：		高志强
//日  期：		2022年10月05日
//更  新：		
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"				//主符号


//定义应用程序类CPlanningApp: 
class CPlanningApp : public CWinApp
{
public:
	CPlanningApp();					//构造函数

//重写
public:
	virtual BOOL InitInstance();	//初始化应用程序实例

//实现
	DECLARE_MESSAGE_MAP()			//声明消息映射

	// 公有数据成员
public:
	CString m_sWorkPath;								//工作路径
};

extern CPlanningApp theApp;			//声明唯一的应用程序对象