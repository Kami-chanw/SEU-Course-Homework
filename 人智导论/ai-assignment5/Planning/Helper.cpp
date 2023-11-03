/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Helper.cpp
//功  能：		实现辅助函数
//开发者：		高志强
//日  期：		2022年10月02日
//更  新：		2022年10月12日
//更  新：		2022年10月21日
//长  度：		50行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"						//MFC头文件
#include "Helper.h"						//辅助函数头文件
#include <comutil.h>					//常用功能函数头文件	
#pragma comment(lib, "comsuppw.lib")	//常用功能函数库文件


//名  称：		MapCStringToString()
//功  能：		将CString映射到string
//参  数：		CString&
//返回值：		string
string MapCStringToString(CString& sGivenCString)
{
	CStringA sTemp(sGivenCString.GetBuffer(0));
	sGivenCString.ReleaseBuffer();

	string sReturnString = sTemp.GetBuffer(0);
	sTemp.ReleaseBuffer();

	//返回结果
	return sReturnString;
}


// 名  称：		GetAttribute()
// 功  能：		获取属性值
// 参  数：		TiXmlElement*,const char*
// 返回值：		string
string GetAttribute(TiXmlElement *pEle, const char * name)
{
	const char * s = pEle->Attribute(name);
	if (s) return s;	// 如果const char *不为空，返回字符串
	else return "";		// 如果是空指针，返回空字符串
}