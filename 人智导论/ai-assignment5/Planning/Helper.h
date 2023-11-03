/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		Helper.h
//��  �ܣ�		���帨������
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��07��
//��  �£�		2022��10��12��
//��  �£�		2022��10��21��
//��  �ȣ�		40��
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>								//mapͷ�ļ�
#include <set>								//setͷ�ļ�
#include <string>							//stringͷ�ļ�
#include <list>								//listͷ�ļ�
#include <vector>							//vectorͷ�ļ�
#include <algorithm>						//algorithm��ͷ�ļ�
#include <numeric>							//numeric��ͷ�ļ�
#include <sstream>							//sstream��ͷ�ļ�
#include <fstream>							//fstream��ͷ�ļ�
#include <iostream>							//iostream��ͷ�ļ�
#include <time.h>							//ʱ�亯����ͷ�ļ�
#include "math.h"							//��ѧ����ͷ�ļ�
#include "TinyXML.h"						//TinyXMLͷ�ļ�
#include "Helper_Type.h"					//��������ͷ�ļ�


//���������ռ�
using namespace std;						//ʹ�ñ�׼�����ռ�


//��CStringӳ�䵽string
string MapCStringToString(CString&);

//��ȡXML�ļ������Ե�ֵ
string GetAttribute(TiXmlElement *pEle, const char * name);