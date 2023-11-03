/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		Helper.cpp
//��  �ܣ�		ʵ�ָ�������
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��02��
//��  �£�		2022��10��12��
//��  �£�		2022��10��21��
//��  �ȣ�		50��
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"						//MFCͷ�ļ�
#include "Helper.h"						//��������ͷ�ļ�
#include <comutil.h>					//���ù��ܺ���ͷ�ļ�	
#pragma comment(lib, "comsuppw.lib")	//���ù��ܺ������ļ�


//��  �ƣ�		MapCStringToString()
//��  �ܣ�		��CStringӳ�䵽string
//��  ����		CString&
//����ֵ��		string
string MapCStringToString(CString& sGivenCString)
{
	CStringA sTemp(sGivenCString.GetBuffer(0));
	sGivenCString.ReleaseBuffer();

	string sReturnString = sTemp.GetBuffer(0);
	sTemp.ReleaseBuffer();

	//���ؽ��
	return sReturnString;
}


// ��  �ƣ�		GetAttribute()
// ��  �ܣ�		��ȡ����ֵ
// ��  ����		TiXmlElement*,const char*
// ����ֵ��		string
string GetAttribute(TiXmlElement *pEle, const char * name)
{
	const char * s = pEle->Attribute(name);
	if (s) return s;	// ���const char *��Ϊ�գ������ַ���
	else return "";		// ����ǿ�ָ�룬���ؿ��ַ���
}