/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_Read_Task.cpp
//��  �ܣ�		��ȡ�滮���񣬰�����ʼ״̬��Ŀ��״̬������Ķ����б��
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��07��
//��  �£�		2022��10��08��
//��  �£�		2022��10��09��
//��  �£�		2022��10��21��
//��  �ȣ�		120��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "Planning.h"								//Ӧ�ó�����ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�



//�����ⲿ����
extern CPlanningApp theApp;


//��  �ƣ�		Read_Task()
//��  �ܣ�		��ȡ�滮���񡣰���Ŀ��״̬�������б��
//��  ����		��
//����ֵ��		��
void CSearchPlan::Read_Task()
{
	//��ȡ��ǰ����·��
	CString sFileName = CString(theApp.m_sWorkPath);
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\shoe.xml");


	//�����ļ�������ļ������ڣ���ֱ�ӷ���
	CFileFind findWenJian;								//�ļ�����
	BOOL bWenJian = findWenJian.FindFile(sFileName);	//�����ļ��Ƿ����
	//���û�з����ļ�����ֱ�ӷ���
	if (!bWenJian)
	{
		//��ʾ
		AfxMessageBox(_T("���ഩЬ����shoe.xml������"));

		//����
		return;
	}


	//���ļ�
	TiXmlDocument aDoc(MapCStringToString(sFileName).c_str());

	//�����ļ��Ƿ�ɹ�
	if (!aDoc.LoadFile())
	{
		//��ʾ
		AfxMessageBox(_T("shoe.xmlʧ��:"));

		//�˳�
		return exit(0);
	}

	//��ȡ�����PLANNING
	TiXmlElement *pRoot = aDoc.RootElement();


	////////////////////////////////////////////////////////////////////////////////////////////////
	//����01����ȡĿ��״̬����ʼ״̬Ϊ��
	TiXmlElement* pGoal = pRoot->FirstChildElement();
	if (pGoal != NULL)
	{
		//��ȡ����
		TiXmlElement* pLiteral = pGoal->FirstChildElement();
		//������������
		while (pLiteral != NULL)
		{
			//��������
			LITERAL literal;

			//��ȡ����
			Read_Task_Literal(pLiteral, literal);

			//��ӵ�Ŀ��״̬��ν������-����ӳ��
			m_GoalState.insert(make_pair(literal.sName,literal));


			//��������ָ��
			pLiteral = pLiteral->NextSiblingElement();
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////
	//����2����ȡ����Ķ����б�
	TiXmlElement* pActions = pGoal->NextSiblingElement();
	if (pActions != NULL)
	{
		//��ȡ����ָ��
		TiXmlElement* pAction = pActions->FirstChildElement();

		//��鶯��ָ���Ƿ�Ϊ��
		while (pAction != NULL)
		{
			//��ȡ����
			ACTION action;

			//��ȡ����
			Read_Task_Action(pAction, action);
			//��ӵ�����Ķ����б�
			m_Actions.push_back(action);

	
			//���¶���ָ��
			pAction = pAction->NextSiblingElement();
		}	
	}


	//�ͷ��ļ���ռ�ڴ�
	aDoc.Clear();
}