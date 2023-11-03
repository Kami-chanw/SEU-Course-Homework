/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		CSearchPlan_Expand.cpp
// ��  �ܣ�		չ����̽��
// �����ߣ�		��־ǿ
// ��  �ڣ�		2022��10��09��
// ��  �£�		2022��10��12��
// ��  �£�		2022��10��15��
// ��  �£�		2022��10��21��
// ��  �ȣ�		90��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"       //MFC��׼ͷ�ļ�
#include "CSearchPlan.h"  //���������Ĺ滮��ͷ�ļ�

// ��  �ƣ�		Expand()
// ��  �ܣ�		չ�����״̬
// ��  ����		��
// ����ֵ��		��
void CSearchPlan::Expand(NODE& curr_node, vector<NODE>& OPEN, map<CString, unsigned int>& CLOSED, map<unsigned int, EDGE>& Tr) {
    /**
    1	����(a)���еĿ��ܶ�������鶯���Ƿ���С�����ÿ����ǰ����:
    2		�������bPreCondSatisfied������ǰ�������Ƿ�����
    3		����(b)����ǰ������:
    4			��������Ƿ�����ڵ�ǰ״̬��ʹ��CSPSearchPlan::IsLiteralContained:
    5				������ֲ����������±���bPreCondSatisfiedֵ�����˳�����(b)
    6		�����ǰ������ǰ���������㣺
    7			������״̬
    8			��Ӷ���Ч��
    9			��ȡ���״̬����
    10			��������Ƿ��Ѿ�������CLOSED��
    11			���״̬û�г��ֹ����Ҳ���OPEN���У�
    12				���ú�̽ڵ�
    13				����״̬/�ڵ�ID����
    14				����̽�����OPEN�б�
    15				����߲���ӱߵ�������
    **/
    for (ACTION& action : m_Actions) {
        if (ranges::all_of(action.precondition, [&](LITERAL& cond) { return IsLiteralContained(cond, curr_node.state); })) {
            STATE suc_state = curr_node.state;
            ranges::copy(action.effect | views::transform([](const LITERAL& li) { return pair(li.sName, li); }),
                         insert_iterator(suc_state, suc_state.end()));
            CString idx = GetStateIndex(suc_state);
            if (CLOSED.find(idx) == CLOSED.end() && !IsInOPEN(idx, OPEN)) {
                OPEN.emplace_back(suc_state, curr_node.nDepth + 1, m_nStateID);
                /*Tr.emplace(
                    m_nStateID, EDGE { // invalid in c++, due to CWG : P0329R4
                        .nEndID      = m_nStateID,
                        .nStartID    = curr_node.nID,
                        .sActionName = action.sName
                    });*/
                Tr.emplace(m_nStateID, EDGE{ curr_node.nID, m_nStateID, action.sName });
                m_nStateID++;
            }
        }
    }
}