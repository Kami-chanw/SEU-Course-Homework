/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		CSearchPlan_RunForward.cpp
// ��  �ܣ�		����ǰ�������Ĺ滮
// �����ߣ�		��־ǿ
// ��  �ڣ�		2022��10��09��
// ��  �£�		2022��10��12��
// ��  �£�		2022��10��21��
// ��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"       //MFC��׼ͷ�ļ�
#include "CSearchPlan.h"  //���������Ĺ滮��ͷ�ļ�
#include <algorithm>
#include <ranges>

// ��  �ƣ�		RunForward()
// ��  �ܣ�		����ǰ�������Ĺ滮
// ��  ����		��
// ����ֵ��		��
void CSearchPlan::RunForward() {
    // ����OPEN�б�Ԫ��Ϊ��㡣���ù������
    vector<NODE> OPEN;
    // ����CLOSEDӳ�䡣Ԫ��Ϊ״̬�����ַ���-���ID
    map<CString, unsigned int> CLOSED;  // �����ж��Ƿ��ظ����
    // ���������Tr��Ԫ��Ϊ�ս��ID-��
    map<unsigned int, EDGE> Tr;

    ////////////////////////////////////////////////////////////////////////
    // ����1����ʼ��
    // ��ʼ��OPEN����
    // �����ʼ״̬/���
    NODE node;
    node.state  = m_InitState;  // ��ʼ״̬
    node.nDepth = 0;            // ������Ϊ0
    node.nID    = m_nStateID;   // ���IDΪ0
    OPEN.push_back(node);       // ��ӵ�OPEN�б�

    ///////////////////////////////////////////////////////////////////////
    // ����2����ʼ������ֱ��OPEN�б�Ϊ��
    // while (OPEN.size() != 0)
    //{
    /**
    1	OPEN�б��һ��Ԫ�س��ӣ������б���ɾ��
    2	�����ǰ״̬��Ŀ��״̬��ʹ��CSearchPlan::IsStateSmall:
    3		���ݵõ��滮��ʹ��CSearchPlan::BackTrack����return
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
        if (IsStateSmall(m_GoalState, curr.state)) {
            BackTrack(curr.nID, Tr);
            return;
        }
        else {
            CString idx = GetStateIndex(curr.state);
            if (CLOSED.find(idx) == CLOSED.end()) {
                CLOSED.emplace(idx, m_nStateID);
                if (m_nStateID == 0)
                    m_nStateID++;
                Expand(curr, OPEN, CLOSED, Tr);
            }
        }
    }
}