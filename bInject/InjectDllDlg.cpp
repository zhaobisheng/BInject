// InjectDllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bInject.h"
#include "InjectDllDlg.h"
#include "afxdialogex.h"
#include "ProcessFunc.h"
#include "ProcessListDlg.h"
#include "Utils.h"

// CInjectDllDlg 对话框

IMPLEMENT_DYNAMIC(CInjectDllDlg, CDialogEx)

CInjectDllDlg::CInjectDllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INJECT_DLL_DIALOG, pParent)
{
	
}

CInjectDllDlg::~CInjectDllDlg()
{
}

void CInjectDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_PROCESS1, m_process_combo);
	DDX_Control(pDX, IDC_EDIT_DLG1_PROCESS, m_edit_process);
	DDX_Control(pDX, IDC_EDIT_DLG1_PROCESS_ID, m_edit_process_id);
	DDX_Control(pDX, IDC_EDIT_DLL_PATH, m_edit_dll_path);
}


BEGIN_MESSAGE_MAP(CInjectDllDlg, CDialogEx)
	ON_BN_CLICKED(ID_INJECT_DLL_BTN, &CInjectDllDlg::OnBnClickedInjectDllBtn)
	ON_BN_CLICKED(ID_BUTTON_OPEN_PROCESS_DLG, &CInjectDllDlg::OnBnClickedButtonOpenProcessDlg)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BOOL CInjectDllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	return TRUE;
}


// CInjectDllDlg 消息处理程序





void CInjectDllDlg::OnBnClickedInjectDllBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDllPath, strProcessID;
	m_edit_dll_path.GetWindowTextA(strDllPath);
	m_edit_process_id.GetWindowTextA(strProcessID);
	DWORD processID = _ttoi(strProcessID);
	if (strProcessID == ""||processID<=0) {
		MessageBox("请选择要注入的进程!", _T("温馨提示"));
		return;
	}
	if( strDllPath != ""){
	
		if (injectDll(processID, strDllPath.GetBuffer())){
			MessageBox(strDllPath+"注入成功!");
		}	
	}
	else {
		MessageBox("请拖入需要注入的DLL文件!", _T("温馨提示"));
	}
}


void CInjectDllDlg::OnBnClickedButtonOpenProcessDlg()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND myHwnd=this->GetSafeHwnd();
	CProcessListDlg *processDlg = new  CProcessListDlg;
	processDlg->parentHwnd = myHwnd;
	processDlg->Create(IDD_PROCESS_DIALOG, this);
	processDlg->ShowWindow(SW_SHOW);
}




void CInjectDllDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nCount;
	TCHAR szPath[MAX_PATH];
	CString dropFile;
	nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (nCount)
	{
		for (UINT nIndex = 0; nIndex < nCount; ++nIndex)
		{
			DragQueryFile(hDropInfo, nIndex, szPath, _countof(szPath));
			dropFile = szPath;
			if (dropFile.Find(".dll") > 0) {
			m_edit_dll_path.SetWindowTextA(dropFile);
			}
			else {
				MessageBox("只支持DLL后缀,请拖入需要注入的DLL文件!", _T("WM_DROPFILES"));
			}
		}
	}

	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}
