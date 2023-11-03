/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_Read_Task_Action.cpp
//��  �ܣ�		��ȡ����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��08��
//��  �£�		2022��10��09��
//��  �£�		2022��10��15��
//��  �£�		2022��10��21��
//��  �ȣ�		80��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "Planning.h"								//Ӧ�ó�����ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		Read_Task_Action()
//��  �ܣ�		��ȡ����
//��  ����		��
//����ֵ��		��
void CSearchPlan::Read_Task_Action(TiXmlElement* pAction, ACTION& action)
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	//����1����ȡ��������
	//01 ��������
	string sTemp = GetAttribute(pAction, "NAME");
	action.sName = CString(sTemp.c_str());

	
	///////////////////////////////////////////////////////////////////////////////////////////////
	//����2����ȡǰ������
	TiXmlElement* pPreCondition = pAction->FirstChildElement();
	//���ǰ�������Ƿ�Ϊ��
	if (pPreCondition != NULL)
	{
		//��ȡ����
		TiXmlElement* pLiteral = pPreCondition->FirstChildElement();

		//������������
		while (pLiteral != NULL)
		{
			//��������
			LITERAL literal;
			//��ȡ����
			Read_Task_Literal(pLiteral, literal);
			//��ӵ�ǰ�������������б�
			action.precondition.push_back(literal);

			//��������ָ��
			pLiteral = pLiteral->NextSiblingElement();
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����3����ȡ����Ч��
	TiXmlElement* pEffect = pPreCondition->NextSiblingElement();
	//��鶯��Ч��ָ���Ƿ�Ϊ��
	if (pEffect != NULL)
	{
		//��ȡ����
		TiXmlElement* pLiteral = pEffect->FirstChildElement();

		//������������
		while (pLiteral != NULL)
		{
			//��������
			LITERAL literal;
			//��ȡ����
			Read_Task_Literal(pLiteral, literal);
			//��ӵ�����Ч���������б�
			action.effect.push_back(literal);

			//��������ָ��
			pLiteral = pLiteral->NextSiblingElement();
		}
	}
}