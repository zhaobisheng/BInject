
// bInjectDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"



// CbInjectDlg 对话框
class CbInjectDlg : public CDialogEx
{
// 构造
public:
	CbInjectDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	//CComboBox m_process_combo;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void ChooseProcess(WPARAM wParam, LPARAM lParam);
	void InitTabControl();
	int m_CurSelTab;
	CInjectDllDlg m_page1;
	CInjectCodeDlg m_page2;
	CDialog* pDialog[2];  //用来保存对话框对象指针
};


