/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_BackTrack.cpp
//��  �ܣ�		���ݵõ��滮
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��09��
//��  �£�		2022��10��12��
//��  �£�		2022��10��15��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		BackTrack()
//��  �ܣ�		���ݵõ��滮
//��  ����		unsigned int
//				map<unsigned int, EDGE> Tr;
//����ֵ��		��
void CSearchPlan::BackTrack(unsigned int nEndNodeID,map<unsigned int, EDGE>& Tr)
{
	//������ʱ���ݽṹ
	vector <CString> Plan;
	//���ҵ�ǰ��
	map<unsigned int, EDGE>::iterator it = Tr.find(nEndNodeID);
	while (it != Tr.end())
	{
		//��������ջ
		Plan.push_back(it->second.sActionName);

		//��������
		it = Tr.find(it->second.nStartID);
	}

	//���򿽱��ض����б�
	for (int i = Plan.size()-1; i >=0; i--)
	{
		//���뵽ȫ�ֱ���
		m_Plan.push_back(Plan[i]);
	}
}