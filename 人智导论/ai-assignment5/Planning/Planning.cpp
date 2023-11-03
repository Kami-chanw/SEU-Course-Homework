/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		Planning.cpp
//��  �ܣ�		ʵ��Ӧ�ó����ࡣ��ȡ����·������ʾ���Ի���
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��05��
//��  �£�		
//��  �ȣ�		110��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"					//MFC��׼ͷ�ļ�
#include "Planning.h"				//Ӧ�ó�����ͷ�ļ�
#include "PlanningDlg.h"			//���Ի�����ͷ�ļ�
#include <direct.h>					//Ŀ¼��������ͷ�ļ�


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//��ʼ��Ϣӳ��
BEGIN_MESSAGE_MAP(CPlanningApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
//������Ϣӳ��


//��  �ƣ�		CPlanningApp()
//��  �ܣ�		���캯��
//��  ����		��
//����ֵ��		��
CPlanningApp::CPlanningApp()
{
	//֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}


//����Ψһ��һ��CPlanningApp����
CPlanningApp theApp;


//��  �ƣ�		InitInstancep()
//��  �ܣ�		��ʼ��Ӧ�ó���ʵ��
//��  ����		��
//����ֵ��		BOOL
BOOL CPlanningApp::InitInstance()
{
	//���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	//ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	//��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	//�����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	
	//���û����ȱʡ����
	CWinApp::InitInstance();

	
	//ʹ�ܿؼ�����
	AfxEnableControlContainer();

	
	//���� shell ���������Է��Ի������
	//�κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	
	//���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	//��׼��ʼ��
	//���δʹ����Щ���ܲ�ϣ����С
	//���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	//����Ҫ���ض���ʼ������
	SetRegistryKey(_T("�Զ��滮����ҵ"));

	
	/////////////////////////////////////////////////////////////////
	//��ȡȫ�ֹ���·��
	//��ȡ��ǰ����·��
	char buffer[_MAX_PATH];					//��ʱ����
	//��ȡ��ǰ����·��
	if (_getcwd(buffer, _MAX_PATH) == NULL)
	{
		//��ʾ����
		AfxMessageBox(_T("_getcwd error"));

		//�˳�
		return false;
	}
	else
	{
		//��ȡ��ǰ�ļ�·��
		m_sWorkPath = buffer;
	}

	//�������Ի������
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
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	//ȱʡ���ؼ�
	return FALSE;
}

