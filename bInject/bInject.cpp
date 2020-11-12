
// bInject.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "bInject.h"
#include "bInjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CbInjectApp

BEGIN_MESSAGE_MAP(CbInjectApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CbInjectApp 构造

CbInjectApp::CbInjectApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CbInjectApp 对象

CbInjectApp theApp;


// CbInjectApp 初始化

BOOL CbInjectApp::InitInstance()
{
	WNDCLASS wc;
	// 获取窗口类信息。MFC默认的所有对话框的窗口类名为 #32770
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	// 改变窗口类名
	wc.lpszClassName = _T("bInjectHelper");
	// 注册新窗口类，使程序能使用它
	AfxRegisterClass(&wc);

	CWinApp::InitInstance();

	//AfxRegisterClass(&wc);
	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CbInjectDlg dlg;
	m_pMainWnd = &dlg;
	//m_pMainWnd->DragAcceptFiles(TRUE);
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


