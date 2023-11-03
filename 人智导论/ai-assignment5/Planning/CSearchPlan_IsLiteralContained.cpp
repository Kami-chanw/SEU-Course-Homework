/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_IsLiteralContained.cpp
//��  �ܣ�		��������Ƿ������״̬
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��09��
//��  �£�		2022��10��12��
//��  �£�		2022��10��21��
//��  �ȣ�		50��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		IsLiteralContained()
//��  �ܣ�		��������Ƿ������״̬
//��  ����		LITERAL&,STATE&
//����ֵ��		bool
bool CSearchPlan::IsLiteralContained(LITERAL& literal, STATE& state)
{
	//��ȡν������
	CString sName = literal.sName;

	//�������-״̬ӳ���
	map<CString, LITERAL>::iterator it = state.find(sName);
	//����Ƿ��ҵ���ͬ���ֵ�����
	if (it != state.end())//�ҵ���ͬ���ֵ�����
	{
		//��鼫���Ƿ���ͬ
		if (literal.bPolarity == it->second.bPolarity)//������ͬ
		{
			//������
			return true;
		}
		else//���Բ�ͬ
		{			
			//���ؼ�
			return false;
		}	
	}
	else//û���ҵ���ͬ���ֵ�����
	{
		//���ؼ�
		return false;
	}
}