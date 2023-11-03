/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_IsStateContainedIn.cpp
//��  �ܣ�		����1��״̬�Ƿ�����ڵ�2��״̬
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��09��
//��  �£�		
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		IsStateContained()
//��  �ܣ�		����1��״̬�Ƿ�����ڵ�2��״̬
//��  ����		STATE&,STATE&
//����ֵ��		bool
bool CSearchPlan::IsStateContainedIn(STATE& s1, STATE& s2)
{
	//����1���쳣�������״̬1��ν��ӳ���Ϊ��
	if (s1.Name2Predicates.size() == 0)
	{
		//������
		return true;
	}


	//����2������״̬1��ν�ʣ�����״̬2�м���Ƿ����
	for (map<CString, PREDICATE>::iterator it = s1.Name2Predicates.begin(); it != s1.Name2Predicates.end(); it++)
	{
		//��ȡ��ǰν��
		PREDICATE predicate = it->second;

		//��鵱ǰν���Ƿ��״̬2����
		if (!IsCompatible(predicate, s2))
		{
			//���ؼ�
			return false;
		}
	}

	//ȱʡ������
	return true;
}