/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_IsLiteralInVector.cpp
//��  �ܣ�		��������Ƿ�������
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��16��
//��  �£�		
//��  �ȣ�		90��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		IsLiteralInVector()
//��  �ܣ�		��������Ƿ���������
//��  ����		LITERAL&,set<LITERAL>&
//����ֵ��		bool
bool CSearchPlan::IsLiteralInVector(LITERAL& literal,
	vector<LITERAL>& Literals)
{
	//�����������ֽ��в���
	for (unsigned int i = 0; i < Literals.size(); i++)
	{
		//������������Ƿ����
		if (literal.sName == Literals[i].sName && literal.bPolarity == Literals[i].bPolarity)
		{
			//������
			return true;
		}
	}
	
	//ȱʡ���ؼ�
	return false;
}