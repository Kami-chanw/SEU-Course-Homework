/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		PlanningDlg.cpp
//��  �ܣ�		�������Ի�����
//�����ߣ�		��־ǿ
//��  �ڣ�		2022��10��05��
//��  �£�		
//��  �ȣ�		50��
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//�������Ի���CPlanningDlg
class CPlanningDlg : public CDialogEx
{
//���캯��
public:
	CPlanningDlg(CWnd* pParent = NULL);					//��׼���캯��

//�Ի�������
	enum { IDD = IDD_PLANNING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	//DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	afx_msg void OnBnClickedButtonForwardSearch();		//����ǰ�������ķ���
	afx_msg void OnBnClickedButtonBackwardSearch();		//���ڷ��������ķ���
	DECLARE_MESSAGE_MAP()
public:
};
