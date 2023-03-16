/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Helper.h
//功  能：		定义辅助函数
//开发者：		高志强
//日  期：		2022年10月07日
//更  新：		2022年10月12日
//更  新：		2022年10月21日
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>								//map头文件
#include <set>								//set头文件
#include <string>							//string头文件
#include <list>								//list头文件
#include <vector>							//vector头文件
#include <algorithm>						//algorithm类头文件
#include <numeric>							//numeric类头文件
#include <sstream>							//sstream类头文件
#include <fstream>							//fstream类头文件
#include <iostream>							//iostream类头文件
#include <time.h>							//时间函数类头文件
#include "math.h"							//数学函数头文件
#include "TinyXML.h"						//TinyXML头文件
#include "Helper_Type.h"					//辅助类型头文件


//声明命名空间
using namespace std;						//使用标准命名空间


//将CString映射到string
string MapCStringToString(CString&);

//获取XML文件中属性的值
string GetAttribute(TiXmlElement *pEle, const char * name);