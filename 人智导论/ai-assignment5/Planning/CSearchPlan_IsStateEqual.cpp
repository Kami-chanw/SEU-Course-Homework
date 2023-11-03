/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_IsStateEqual.cpp
//��  �ܣ�		���2��״̬�Ƿ����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��09��
//��  �£�		2022��10��12��
//��  �£�		2022��10��15��
//��  �£�		2022��10��21��
//��  �ȣ�		50��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		IsStateEqual()
//��  �ܣ�		�������״̬�Ƿ����
//��  ����		STATE&,STATE&
//����ֵ��		bool
bool CSearchPlan::IsStateEqual(STATE& s1, STATE& s2)
{
	//����״̬1����������
	for (map<CString, LITERAL>::iterator it = s1.begin(); it != s1.end(); it++)
	{
		//��״̬2�н��в���
		if (!IsLiteralContained(it->second, s2))
		{
			//���ؼ�
			return false;
		}
	}

	//����״̬2����������
	for (map<CString, LITERAL>::iterator it = s2.begin(); it != s2.end(); it++)
	{
		//��״̬1�н��в���
		if (!IsLiteralContained(it->second, s1))
		{
			//���ؼ�
			return false;
		}
	}
	
	//ȱʡ������
	return true;
}