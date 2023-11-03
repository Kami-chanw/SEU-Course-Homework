/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_OutputToXML.cpp
//��  �ܣ�		����滮��XML�ļ�
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��21��
//��  �£�		
//��  �ȣ�		70��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "Planning.h"								//Ӧ�ó�����ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//�����ⲿ����
extern CPlanningApp theApp;


//��  �ƣ�		OutputToXML()
//��  �ܣ�		����滮��XML�ļ�
//��  ����		��
//����ֵ��		��
void CSearchPlan::OutputToXML()
{
	//����XML�ĵ�
	TiXmlDocument doc;
	//�����ĵ�����
	TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "GBK", "");
	//����������Ϣ���ĵ�
	doc.LinkEndChild(pDecl);

	//������Ԫ��
	TiXmlElement *pRootElement = new TiXmlElement("Plan");
	//���Ӹ�Ԫ��
	doc.LinkEndChild(pRootElement);


	//�������в�ѯ���
	for (unsigned int i = 0; i < m_Plan.size(); i++)
	{
		//������������
		TiXmlElement* pAction = new TiXmlElement("Action");

		//���ö�������
		pAction->SetAttribute("NAME",MapCStringToString(m_Plan[i]).c_str());


		//���Ӷ����������б�
		pRootElement->LinkEndChild(pAction);
	}

	//��ȡ��ǰ����·��
	CString sFileName = theApp.m_sWorkPath;
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\Output.xml");

	//����XML�ļ�
	doc.SaveFile((MapCStringToString(sFileName)).c_str());

	//�ͷ��ڴ�
	doc.Clear();

	//�Զ����ļ�
	ShellExecute(NULL, _T("open"), sFileName, NULL, NULL, SW_SHOWNORMAL);
}