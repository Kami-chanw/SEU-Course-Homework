/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		Planning.h
//��  �ܣ�		Ӧ�ó�����ͷ�ļ�
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��05��
//��  �£�		
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"				//������


//����Ӧ�ó�����CPlanningApp: 
class CPlanningApp : public CWinApp
{
public:
	CPlanningApp();					//���캯��

//��д
public:
	virtual BOOL InitInstance();	//��ʼ��Ӧ�ó���ʵ��

//ʵ��
	DECLARE_MESSAGE_MAP()			//������Ϣӳ��

	// �������ݳ�Ա
public:
	CString m_sWorkPath;								//����·��
};

extern CPlanningApp theApp;			//����Ψһ��Ӧ�ó������