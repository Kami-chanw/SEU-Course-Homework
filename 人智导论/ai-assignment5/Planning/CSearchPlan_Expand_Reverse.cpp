/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		CSearchPlan_Expand_Reverse.cpp
// ��  �ܣ�		չ����̽�㡣������������
// �����ߣ�		��־ǿ
// ��  �ڣ�		2022��10��16��
// ��  �£�		2022��10��21��
// ��  �ȣ�		110��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"       //MFC��׼ͷ�ļ�
#include "CSearchPlan.h"  //���������Ĺ滮��ͷ�ļ�

// ��  �ƣ�		Expand_Reverse()
// ��  �ܣ�		����չ�����״̬
// ��  ����		��
// ����ֵ��		��
void CSearchPlan::Expand_Reverse(NODE& curr_node, vector<NODE>& OPEN, map<CString, unsigned int>& CLOSED,
                                 map<unsigned int, EDGE>& Tr) {
    /**
    1	����(a)���еĿ��ܶ�������鶯���Ƿ���С�����ÿ����ǰ����:
    2		�������bSatisfied�����涯��Ч���Ƿ�����Ҫ��
    3		����ȥ����Ч�����ּ���vector<LITERAL> Effects;
    4		����(b)���еĶ���Ч��:
    5			���ν���Ƿ����㵱ǰ״̬��ʹ��CSPSearchPlan::IsLiteralContained:
    6				������㣬���±���bSatisfiedֵ�������Ч�����ֵ�Ч�����ּ�
    7		�������Ч������Ҫ��
    8			����ǰ��״̬��STATE prev_state;
    9			����(c)��ǰ�ڵ��ÿ��״̬:
    10				���״̬�������Ƿ���Ч�����ּ��У�ʹ��CSearchPlan::IsLiteralInVector:
    11					������ڣ�����ӵ�ǰ��״̬
    12			����(d)��ǰ������ǰ��:
    13				��ȡǰ����������ƣ�����ӵ�ǰ��״̬
    14			��ȡ������ʹ��CSearchPlan::GetStateIndex
    15			��������Ƿ��Ѿ�������CLOSED��
    16			���״̬û�г��ֹ����Ҳ���OPEN���У�
    17				����ǰ���ڵ�
    18				����״̬/�ڵ�ID����
    19				����̽�����OPEN�б�
    20				����߲���ӱߵ�������
    **/

    for (ACTION& action : m_Actions) {
        vector<LITERAL> effects;
        ranges::copy_if(action.effect, back_inserter(effects),
                        [&](LITERAL& e) { return IsLiteralContained(e, curr_node.state); });
        if (effects.size()) {
            STATE prev_state;
            ranges::copy_if(curr_node.state | views::transform([](map<CString, LITERAL>::value_type& p) {
                                return pair(p.second.sName, p.second);
                            }),
                            insert_iterator(prev_state, prev_state.end()), [&](const pair<CString, LITERAL>& p) {
                                return !IsLiteralInVector(const_cast<pair<CString, LITERAL>&>(p).second, effects);
                            });  // �����׹�ˣ�������޸��ܲ��ܸ�̧���ָ�������������const���η�
            ranges::copy(action.precondition | views::transform([](LITERAL& li) { return pair(li.sName, li); }),
                         insert_iterator(prev_state, prev_state.end()));

            CString idx = GetStateIndex(prev_state);
            if (CLOSED.find(idx) == CLOSED.end() && !IsInOPEN(idx, OPEN)) {
                OPEN.emplace_back(prev_state, curr_node.nDepth + 1, m_nStateID);
                Tr.emplace(m_nStateID, EDGE{ curr_node.nID, m_nStateID, action.sName });
                m_nStateID++;
            }
        }
    }
}