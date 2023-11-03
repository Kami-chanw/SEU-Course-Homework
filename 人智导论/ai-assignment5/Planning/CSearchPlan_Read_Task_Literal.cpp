/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_Read_Task_Literal.cpp
//��  �ܣ�		��ȡ����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��08��
//��  �£�		2022��10��09��
//��  �£�		2022��10��12��
//��  �£�		2022��10��21��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "Planning.h"								//Ӧ�ó�����ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		Read_Task_Literal()
//��  �ܣ�		��ȡ����
//��  ����		TiXmlElement*,LITERAL&
//����ֵ��		��
void CSearchPlan::Read_Task_Literal(TiXmlElement* pLiteral, LITERAL& literal)
{
	//01 ��������
	string sTemp = GetAttribute(pLiteral, "NAME");
	literal.sName = CString(sTemp.c_str());

	//02 ���ּ���
	sTemp = GetAttribute(pLiteral, "POLARITY");
	if (sTemp == "true")
	{
		//����Ϊ�棬ת��Ϊbool����
		literal.bPolarity = true;
	}
	else
	{
		//����Ϊ�٣�ת��Ϊbool����
		literal.bPolarity = false;
	}
}