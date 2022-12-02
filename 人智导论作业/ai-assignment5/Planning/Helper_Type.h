/////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		Helper_Type.h
// 功  能：		定义辅助类型
// 开发者：		高志强
// 日  期：		2022年10月07日
// 更  新：		2022年10月09日
// 更  新：		2022年10月12日
// 长  度：		80行
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <algorithm>  //algorithm类头文件
#include <list>       //list头文件
#include <map>        //map头文件
#include <numeric>    //numeric类头文件
#include <set>        //set头文件
#include <string>     //string头文件
#include <vector>     //vector头文件

// 声明命名空间
using namespace std;  // 使用标准命名空间

///////////////////////////////////////////////////////////////////////////////////////////////
// 定义文字、状态、结点等

// 文字
typedef struct {
    CString sName;      // 01 谓词名称
    bool    bPolarity;  // 02 谓词极性。true或false
} LITERAL;              // 文字

// 状态
typedef map<CString, LITERAL> STATE;  // 状态。谓词名称到文字的映射

// 结点。包括状态、深度和节点ID
typedef struct {
    STATE        state;   // 01 状态
    unsigned int nDepth;  // 02 结点深度。暂未使用
    unsigned int nID;     // 03 结点ID
} NODE;

////////////////////////////////////////////////////////////////////////////////////////////
// 定义动作，包括前提条件、动作效果、动作、边（用于回溯）等

// 前提条件
typedef vector<LITERAL> PRECONDITION;  // 文字列表

// 动作效果
typedef vector<LITERAL> EFFECT;  // 文字列表

// 动作
typedef struct {
    CString      sName;         // 01 动作名称
    PRECONDITION precondition;  // 02 前提条件
    EFFECT       effect;        // 03 动作效果
} ACTION;                       // 动作

// 边。用于回溯
typedef struct {
    unsigned int nStartID;     // 01 开始节点ID
    unsigned int nEndID;       // 02 结束节点ID
    CString      sActionName;  // 03 动作名称
} EDGE;