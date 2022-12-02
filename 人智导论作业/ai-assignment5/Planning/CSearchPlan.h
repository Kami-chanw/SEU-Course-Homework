/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CSearchPlan.h
//功  能：		定义基于搜索的规划类。包括前向搜索、逆向搜索
//开发者：		高志强
//日  期：		2022年10月09日
//更  新：		2022年10月12日
//更  新：		2022年10月21日
//长  度：		80行
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <map>							//map头文件
#include <set>							//set头文件
#include <string>						//string头文件
#include <list>							//list头文件
#include <vector>						//vector头文件
#include <queue>						//队列头文件
#include <stack>						//栈头文件
#include <algorithm>					//algorithm类头文件
#include <numeric>						//numeric类头文件
#include <sstream>						//sstream类头文件
#include <fstream>						//fstream类头文件
#include <time.h>						//时间函数类头文件
#include "math.h"						//数学函数头文件
#include "Helper.h"						//辅助函数头文件


//声明命名空间
using namespace std;									//使用标准命名空间


//定义基于搜索的规划类
class CSearchPlan
{
	//构造函数与析构函数
public:
	CSearchPlan();										//构造函数
	~CSearchPlan();										//析构函数

	//公有方法
public:
	//初始化
	void Init();										//初始化
	
	//运行
	void RunForward();									//基于前向搜索的规划
	void RunBackward();									//基于逆向搜索的规划

	//输出规划
	void OutputToXML();									//输出概率分布到XML文件

	//私有方法
private:
	////////////////////////////////////////////////////////////////////////////////////////////
	//读取规划任务
	void Read_Task();									//读取规划任务
	void Read_Task_Literal(TiXmlElement*, LITERAL&);	//读取规划任务中的文字
	void Read_Task_Action(TiXmlElement*, ACTION&);		//读取规划任务中的动作列表

	
	////////////////////////////////////////////////////////////////////////////
	//辅助函数
	//检查状态是否相等
	bool IsStateSmall(STATE&, STATE&);						//检查一个状态是否小于另一个
	bool IsStateEqual(STATE&, STATE&);						//检查2个状态是否相等？
	bool IsLiteralContained(LITERAL&, STATE&);				//检查文字是否包含于状态？	
	CString GetStateIndex(STATE&);							//获取状态索引
	bool IsInOPEN(CString&, vector<NODE>&);					//检查索引是否在OPEN表
	bool IsLiteralInVector(LITERAL&, vector<LITERAL>&);//检查文字是否在文字集合中
	

	///////////////////////////////////////////////////////////////////////////
	//前向搜索
	//展开后继节点
	void Expand(NODE&, vector<NODE>&, map<CString,unsigned int>&, map<unsigned int, EDGE>&);	//展开后继节点
	//回溯得到规划
	void BackTrack(unsigned int, map<unsigned int, EDGE>&);	//回溯得到规划

	
	///////////////////////////////////////////////////////////////////////////
	//逆向搜索
	void Expand_Reverse(NODE&, vector<NODE>&, map<CString, unsigned int>&, map<unsigned int, EDGE>&);	//反向展开后继节点
	//回溯得到规划
	void BackTrack_Reverse(unsigned int, map<unsigned int, EDGE>&);	//回溯得到规划
	

//私有数据成员
private:
	//规划任务定义
	STATE m_InitState;									//规划任务的初始状态
	STATE m_GoalState;									//规划任务的目标状态
	vector <ACTION> m_Actions;							//规划任务允许的动作列表
	
	vector<CString> m_Plan;								//搜索得到的规划，即动作名称序列						
	
	unsigned int m_nStateID;							//当前状态ID/节点ID
};