/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CSearchPlan_RunBackward.cpp
//��  �ܣ�		�������������Ĺ滮
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��16��
//��  �£�		2022��10��21��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC��׼ͷ�ļ�
#include "CSearchPlan.h"							//���������Ĺ滮��ͷ�ļ�


//��  �ƣ�		RunBackward()
//��  �ܣ�		��������
//��  ����		��
//����ֵ��		��
void CSearchPlan::RunBackward()
{
	//����OPEN�б�
	vector<NODE> OPEN;
	//����CLOSEDӳ��
	map <CString, unsigned int> CLOSED;			
	//���������Tr
	map<unsigned int, EDGE> Tr;


	////////////////////////////////////////////////////////////////////////
	//����1����ʼ��
	//��ʼ��OPEN����	
	NODE node;
	node.state = m_GoalState;	//���״̬
	node.nDepth = 0;			//������
	node.nID = m_nStateID;		//���ID
	OPEN.push_back(node);


	///////////////////////////////////////////////////////////////////////
	//����2����ʼ������ֱ��OPEN��Ϊ��
	//while (OPEN.size() != 0)
	//{
		/**
		1	OPEN�б��һ��Ԫ�س��ӣ������б���ɾ��
		2	�����ǰ״̬�ǳ�ʼ״̬��ʹ��CSearchPlan::IsStateEqual:
		3		���ݵõ��滮��ʹ��CSearchPlan::BackTrack_Reverse����return
		4	���򣬼�������:
		5		�����ǰ״̬��CLOSEDӳ���в�����:
		6			����ǰ�����ӵ�CLOSED
		7			������ID��0��������
		8			չ�����״̬��OPEN��ʹ��CSearchPlan::Expand
		**/

		// TODO
	//}

	while (OPEN.size()) {
        NODE curr = OPEN.front();
        OPEN.erase(OPEN.begin());  // WTF?? OPEN��ӦΪvector������������д��
        if (IsStateEqual(m_InitState, curr.state)) {
            BackTrack_Reverse(curr.nID, Tr);
            return;
        }
        else {
            CString idx = GetStateIndex(curr.state);
            if (CLOSED.find(idx) == CLOSED.end()) {
                CLOSED.emplace(idx, m_nStateID);
                if (m_nStateID == 0)
                    m_nStateID++;
                Expand_Reverse(curr, OPEN, CLOSED, Tr);
            }
        }
    }
}