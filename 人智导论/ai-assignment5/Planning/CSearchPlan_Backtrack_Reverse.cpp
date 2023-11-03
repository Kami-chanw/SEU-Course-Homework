/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_BackTrack_Reverse.cpp
//��  �ܣ�		���ݵõ��滮��������������
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��09��
//��  �£�		
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		BackTrack_Reverse()
//��  �ܣ�		���ݵõ��滮��������������
//��  ����		unsigned int
//				map<unsigned int, EDGE> Tr;
//����ֵ��		��
void CSearchPlan::BackTrack_Reverse(unsigned int nEndNodeID, map<unsigned int, EDGE>& Tr)
{
	//���ҵ�ǰ��
	map<unsigned int, EDGE>::iterator it = Tr.find(nEndNodeID);
	while (it != Tr.end())
	{
		//��������ӵ��滮
		m_Plan.push_back(it->second.sActionName);

		//���������
		it = Tr.find(it->second.nStartID);
	}
}