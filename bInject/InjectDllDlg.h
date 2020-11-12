#pragma once
#include "afxwin.h"


// CInjectDllDlg 对话框

class CInjectDllDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInjectDllDlg)

public:
	CInjectDllDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInjectDllDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INJECT_DLL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnBnClickedInjectDllBtn();
	afx_msg void OnBnClickedButtonOpenProcessDlg();
	CEdit m_edit_process;
	CEdit m_edit_process_id;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CEdit m_edit_dll_path;
};
