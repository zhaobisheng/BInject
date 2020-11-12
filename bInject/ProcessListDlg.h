#pragma once
#include "afxcmn.h"



// CProcessListDlg 对话框

class CProcessListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessListDlg)

public:
	CProcessListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	HWND parentHwnd;
	CListCtrl m_list_process;
	CListCtrl m_list_module;
	DWORD selectProcessID=0;
	WPARAM selectProcessWPARAM;
	CString strProcessName;
	//afx_msg void OnLvnItemchangedListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	void InitModalIndirect(DWORD PID);
	void InitListItem();
	afx_msg void OnBnClickedProcessSelect();
	afx_msg void OnBnClickedProcessRefresh();
	afx_msg void OnBnClickedButtonUninstallDll();
	afx_msg void OnEnChangeEditProcessTips();
	afx_msg void OnBnClickedButtonSuspendThreat();
	afx_msg void OnBnClickedButtonResumeThreat();
};
