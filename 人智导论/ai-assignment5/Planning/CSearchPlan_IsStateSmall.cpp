/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_IsStateEqual.cpp
//��  �ܣ�		���һ��״̬�Ƿ����������һ��״̬
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��09��
//��  �£�		2022��10��12��
//��  �£�		2022��10��15��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		IsStateSmall()
//��  �ܣ�		���һ��״̬�Ƿ����������һ��״̬
//��  ����		STATE&,STATE&
//����ֵ��		bool
bool CSearchPlan::IsStateSmall(STATE& s1, STATE& s2)
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

	//ȱʡ������
	return true;
}