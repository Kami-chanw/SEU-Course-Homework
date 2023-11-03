/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		CSearchPlan.h
// ��  �ܣ�		������������Ĺ滮�ࡣ����ǰ����������������
// �����ߣ�		��־ǿ
// ��  �ڣ�		2022��10��09��
// ��  �£�		2022��10��12��
// ��  �£�		2022��10��21��
// ��  �ȣ�		80��
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Helper.h"   //��������ͷ�ļ�
#include "math.h"     //��ѧ����ͷ�ļ�
#include <algorithm>  //algorithm��ͷ�ļ�
#include <fstream>    //fstream��ͷ�ļ�
#include <list>       //listͷ�ļ�
#include <map>        //mapͷ�ļ�
#include <numeric>    //numeric��ͷ�ļ�
#include <queue>      //����ͷ�ļ�
#include <set>        //setͷ�ļ�
#include <sstream>    //sstream��ͷ�ļ�
#include <stack>      //ջͷ�ļ�
#include <string>     //stringͷ�ļ�
#include <time.h>     //ʱ�亯����ͷ�ļ�
#include <vector>     //vectorͷ�ļ�

// ���������ռ�
using namespace std;  // ʹ�ñ�׼�����ռ�

// ������������Ĺ滮��
class CSearchPlan {
    // ���캯������������
public:
    CSearchPlan();   // ���캯��
    ~CSearchPlan();  // ��������

    // ���з���
public:
    // ��ʼ��
    void Init();  // ��ʼ��

    // ����
    void RunForward();   // ����ǰ�������Ĺ滮
    void RunBackward();  // �������������Ĺ滮

    // ����滮
    void OutputToXML();  // ������ʷֲ���XML�ļ�

    // ˽�з���
private:
    ////////////////////////////////////////////////////////////////////////////////////////////
    // ��ȡ�滮����
    void Read_Task();                                 // ��ȡ�滮����
    void Read_Task_Literal(TiXmlElement*, LITERAL&);  // ��ȡ�滮�����е�����
    void Read_Task_Action(TiXmlElement*, ACTION&);    // ��ȡ�滮�����еĶ����б�

    ////////////////////////////////////////////////////////////////////////////
    // ��������
    // ���״̬�Ƿ����
    bool    IsStateSmall(STATE&, STATE&);                   // ���һ��״̬�Ƿ�С����һ��
    bool    IsStateEqual(STATE&, STATE&);                   // ���2��״̬�Ƿ���ȣ�
    bool    IsLiteralContained(LITERAL&, STATE&);           // ��������Ƿ������״̬��
    CString GetStateIndex(STATE&);                          // ��ȡ״̬����
    bool    IsInOPEN(CString&, vector<NODE>&);              // ��������Ƿ���OPEN��
    bool    IsLiteralInVector(LITERAL&, vector<LITERAL>&);  // ��������Ƿ������ּ�����

    ///////////////////////////////////////////////////////////////////////////
    // ǰ������
    // չ����̽ڵ�
    void Expand(NODE&, vector<NODE>&, map<CString, unsigned int>&, map<unsigned int, EDGE>&);  // չ����̽ڵ�
    // ���ݵõ��滮
    void BackTrack(unsigned int, map<unsigned int, EDGE>&);  // ���ݵõ��滮

    ///////////////////////////////////////////////////////////////////////////
    // ��������
    void Expand_Reverse(NODE&, vector<NODE>&, map<CString, unsigned int>&, map<unsigned int, EDGE>&);  // ����չ����̽ڵ�
    // ���ݵõ��滮
    void BackTrack_Reverse(unsigned int, map<unsigned int, EDGE>&);  // ���ݵõ��滮

    // ˽�����ݳ�Ա
private:
    // �滮������
    STATE          m_InitState;  // �滮����ĳ�ʼ״̬
    STATE          m_GoalState;  // �滮�����Ŀ��״̬
    vector<ACTION> m_Actions;    // �滮��������Ķ����б�

    vector<CString> m_Plan;  // �����õ��Ĺ滮����������������

    unsigned int m_nStateID = 0;  // ��ǰ״̬ID/�ڵ�ID
};