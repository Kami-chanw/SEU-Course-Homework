/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_GetStateIndex.cpp
//��  �ܣ�		��ȡ״̬����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��15��
//��  �£�		2022��10��21��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		getStateIndex()
//��  �ܣ�		��ȡ״̬����
//��  ����		STATE&
//����ֵ��		CString
CString CSearchPlan::GetStateIndex(STATE& state)
{
	//��������
	CString sIndex;

	//������������
	for (map<CString, LITERAL>::iterator it = state.begin(); it != state.end(); it++)
	{
		//������ֵļ���
		if (it->second.bPolarity == true)
		{
			//��ȡ��ȡ������ע��ո�
			sIndex += _T(" +") + it->first;
		}
		else
		{
			//��ȡ������ע��ո�
			sIndex += _T(" -") + it->first;
		}
	}

	//��������
	return sIndex;
}