/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Planning.cpp
//功  能：		实现应用程序类。获取工作路径、显示主对话框
//开发者：		高志强
//日  期：		2022年10月05日
//更  新：		
//长  度：		110行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"					//MFC标准头文件
#include "Planning.h"				//应用程序类头文件
#include "PlanningDlg.h"			//主对话框类头文件
#include <direct.h>					//目录操作函数头文件


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//开始消息映射
BEGIN_MESSAGE_MAP(CPlanningApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
//结束消息映射


//名  称：		CPlanningApp()
//功  能：		构造函数
//参  数：		无
//返回值：		无
CPlanningApp::CPlanningApp()
{
	//支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}


//定义唯一的一个CPlanningApp对象
CPlanningApp theApp;


//名  称：		InitInstancep()
//功  能：		初始化应用程序实例
//参  数：		无
//返回值：		BOOL
BOOL CPlanningApp::InitInstance()
{
	//如果一个运行在 Windows XP 上的应用程序清单指定要
	//使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	//将它设置为包括所有要在应用程序中使用的
	//公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	
	//调用基类的缺省函数
	CWinApp::InitInstance();

	
	//使能控件容器
	AfxEnableControlContainer();

	
	//创建 shell 管理器，以防对话框包含
	//任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	
	//激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	//标准初始化
	//如果未使用这些功能并希望减小
	//最终可执行文件的大小，则应移除下列
	//不需要的特定初始化例程
	SetRegistryKey(_T("自动规划大作业"));

	
	/////////////////////////////////////////////////////////////////
	//获取全局工作路径
	//获取当前工作路径
	char buffer[_MAX_PATH];					//临时变量
	//获取当前工作路径
	if (_getcwd(buffer, _MAX_PATH) == NULL)
	{
		//提示错误
		AfxMessageBox(_T("_getcwd error"));

		//退出
		return false;
	}
	else
	{
		//获取当前文件路径
		m_sWorkPath = buffer;
	}

	//定义主对话框对象
	CPlanningDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{	
	}
	else if (nResponse == IDCANCEL)
	{
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	//缺省返回假
	return FALSE;
}

