/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		stdafx.h
//功  能：		MFC标准头文件
//开发者：		高志强
//日  期：		2022年10月05日
//更  新：		2022年10月21日
//长  度：		50行
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN							//从Windows头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      //某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>								//MFC核心组件和标准组件
#include <afxext.h>								//MFC扩展


#include <afxdisp.h>							//MFC自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>							//MFC对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>								//MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>						//功能区和控件条的 MFC 支持


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

