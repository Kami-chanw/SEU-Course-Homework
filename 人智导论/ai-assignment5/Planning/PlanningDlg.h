/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		PlanningDlg.cpp
//功  能：		定义主对话框类
//开发者：		高志强
//日  期：		2022年10月05日
//更  新：		
//长  度：		50行
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//定义主对话框CPlanningDlg
class CPlanningDlg : public CDialogEx
{
//构造函数
public:
	CPlanningDlg(CWnd* pParent = NULL);					//标准构造函数

//对话框数据
	enum { IDD = IDD_PLANNING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	//DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	afx_msg void OnBnClickedButtonForwardSearch();		//基于前向搜索的方法
	afx_msg void OnBnClickedButtonBackwardSearch();		//基于反向搜索的方法
	DECLARE_MESSAGE_MAP()
public:
};
