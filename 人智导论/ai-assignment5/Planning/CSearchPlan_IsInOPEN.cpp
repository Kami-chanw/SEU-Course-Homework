/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_IsInOPEN.cpp
//��  �ܣ�		������Ƿ���OPEN����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��12��
//��  �£�		2022��10��21��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		IsInOPEN()
//��  �ܣ�		������Ƿ���OPEN����
//��  ����		CString&,vector<NODE>&
//����ֵ��		��
bool CSearchPlan::IsInOPEN(CString& sIndex, vector<NODE>&OPEN)
{
	//����OPEN���е����н��
	for (unsigned int i = 0; i < OPEN.size(); i++)
	{
		//��ȡ�������
		CString sNodeIndex = GetStateIndex(OPEN[i].state);

		//����Ƿ����
		if (sIndex == sNodeIndex)
		{
			//������
			return true;
		}
	}

	//ȱʡ���ؼ�
	return false;
}