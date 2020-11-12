
// bInjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bInject.h"
#include "bInjectDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//声明函数


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



// CbInjectDlg 对话框



CbInjectDlg::CbInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BINJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tab);

}

BEGIN_MESSAGE_MAP(CbInjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CbInjectDlg::OnTcnSelchangeTabMain)
	ON_MESSAGE(WM_CHOOSEPROCESS, (LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM))ChooseProcess)
END_MESSAGE_MAP()


// CbInjectDlg 消息处理程序

BOOL CbInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码

	this->InitTabControl();
	if (PathFileExistsA("SkinH.dll") && PathFileExistsA("skinh.she")) {
		typedef int(*DLL_SkinH_Attach)();
		HINSTANCE m_myDllHInst = LoadLibrary("SkinH.dll");
		if (m_myDllHInst == NULL)
		{
			FreeLibrary(m_myDllHInst);
			return TRUE;
		}
		DLL_SkinH_Attach ddl_SkinH_Attach = (DLL_SkinH_Attach)GetProcAddress(m_myDllHInst, "SkinH_Attach");
		if (ddl_SkinH_Attach == NULL)
		{
			FreeLibrary(m_myDllHInst);
			return TRUE;
		}
		ddl_SkinH_Attach();
		//FreeLibrary(m_myDllHInst);
		//SkinH_Attach();
		//::SkinH_AttachEx("skinh.she", NULL);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CbInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CbInjectDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码

	//把当前的页面隐藏起来
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = m_tab.GetCurSel();
	//把新的页面显示出来
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

void  CbInjectDlg::InitTabControl() 
{
	m_tab.InsertItem(0, _T("注入DLL"));
	m_tab.InsertItem(1, _T("注入汇编"));
	//创建两个对话框
	m_page1.Create(IDD_INJECT_DLL_DIALOG, &m_tab);
	m_page2.Create(IDD_INJECT_CODE_DIALOG, &m_tab);
	//设定在Tab内显示的范围
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	
	m_page1.MoveWindow(&rc);
	m_page2.MoveWindow(&rc);

	//把对话框对象指针保存起来
	pDialog[0] = &m_page1;
	pDialog[1] = &m_page2;
	//显示初始页面
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	//保存当前选择
	m_CurSelTab = 0;
}

void CbInjectDlg::ChooseProcess(WPARAM wParam, LPARAM lParam)
{
	CString msgName,msgID;
	//msg = (char*)wParam;
	msgName.Format("%s",wParam);
	msgID.Format("%d", (LPARAM)lParam);
	m_page1.m_edit_process.SetWindowText(msgName);
	m_page1.m_edit_process_id.SetWindowText(msgID);
	m_page2.m_edit_process.SetWindowText(msgName);
	m_page2.m_edit_process_id.SetWindowText(msgID);
	pDialog[0]->UpdateWindow();
}