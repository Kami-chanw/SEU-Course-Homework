/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		PlanningDlg_User.cpp
//��  �ܣ�		��Ӧ�û�����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��05��
//��  �£�		2022��10��09��
//��  �£�		2022��10��21��
//��  �ȣ�		70��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"				//MFC��׼ͷ�ļ�	
#include "Planning.h"			//Ӧ�ó�����ͷ�ļ�
#include "PlanningDlg.h"		//���Ի�����ͷ�ļ�
#include "afxdialogex.h"		//MFC��չ�Ի�����ͷ�ļ�
#include "CSearchPlan.h"		//���������Ĺ滮����ͷ�ļ�


//��  �ƣ�		OnBnClickedButtonForwardSearch()
//��  �ܣ�		��Ӧ�û��������ǰ�������Ĺ滮����
//��  ����		��
//����ֵ��		��
void CPlanningDlg::OnBnClickedButtonForwardSearch()
{
	//������������滮��Ķ���
	CSearchPlan theSearchPlan;

	//��ʼ��
	theSearchPlan.Init();

	//����ǰ�������Ĺ滮
	theSearchPlan.RunForward();
	
	//����滮
	theSearchPlan.OutputToXML();
}


//��  �ƣ�		OnBnClickedButtonBackwardSearch()
//��  �ܣ�		��Ӧ�û�����������������Ĺ滮����
//��  ����		��
//����ֵ��		��
void CPlanningDlg::OnBnClickedButtonBackwardSearch()
{
	//������������滮��Ķ���
	CSearchPlan theSearchPlan;

	//��ʼ��
	theSearchPlan.Init();

	//�������������Ĺ滮
	theSearchPlan.RunBackward();

	//����滮
	theSearchPlan.OutputToXML();
}
