/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		Helper_Type.h
// ��  �ܣ�		���帨������
// �����ߣ�		��־ǿ
// ��  �ڣ�		2022��10��07��
// ��  �£�		2022��10��09��
// ��  �£�		2022��10��12��
// ��  �ȣ�		80��
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <algorithm>  //algorithm��ͷ�ļ�
#include <list>       //listͷ�ļ�
#include <map>        //mapͷ�ļ�
#include <numeric>    //numeric��ͷ�ļ�
#include <set>        //setͷ�ļ�
#include <string>     //stringͷ�ļ�
#include <vector>     //vectorͷ�ļ�

// ���������ռ�
using namespace std;  // ʹ�ñ�׼�����ռ�

///////////////////////////////////////////////////////////////////////////////////////////////
// �������֡�״̬������

// ����
typedef struct {
    CString sName;      // 01 ν������
    bool    bPolarity;  // 02 ν�ʼ��ԡ�true��false
} LITERAL;              // ����

// ״̬
typedef map<CString, LITERAL> STATE;  // ״̬��ν�����Ƶ����ֵ�ӳ��

// ��㡣����״̬����Ⱥͽڵ�ID
typedef struct {
    STATE        state;   // 01 ״̬
    unsigned int nDepth;  // 02 �����ȡ���δʹ��
    unsigned int nID;     // 03 ���ID
} NODE;

////////////////////////////////////////////////////////////////////////////////////////////
// ���嶯��������ǰ������������Ч�����������ߣ����ڻ��ݣ���

// ǰ������
typedef vector<LITERAL> PRECONDITION;  // �����б�

// ����Ч��
typedef vector<LITERAL> EFFECT;  // �����б�

// ����
typedef struct {
    CString      sName;         // 01 ��������
    PRECONDITION precondition;  // 02 ǰ������
    EFFECT       effect;        // 03 ����Ч��
} ACTION;                       // ����

// �ߡ����ڻ���
typedef struct {
    unsigned int nStartID;     // 01 ��ʼ�ڵ�ID
    unsigned int nEndID;       // 02 �����ڵ�ID
    CString      sActionName;  // 03 ��������
} EDGE;