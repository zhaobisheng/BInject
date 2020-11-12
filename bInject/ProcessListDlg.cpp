// ProcessList.cpp : 实现文件
//

#include "stdafx.h"
#include "bInject.h"
#include "ProcessListDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include <windows.h>
#include "ProcessFunc.h"
#include "Utils.h"
using namespace std;




// CProcessListDlg 对话框

IMPLEMENT_DYNAMIC(CProcessListDlg, CDialogEx)

CProcessListDlg::CProcessListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESS_DIALOG, pParent)
{

}

CProcessListDlg::~CProcessListDlg()
{
}

void CProcessListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_list_process);
	DDX_Control(pDX, IDC_LIST_MODULE, m_list_module);
	
}


BEGIN_MESSAGE_MAP(CProcessListDlg, CDialogEx)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROCESS, &CProcessListDlg::OnLvnItemchangedListProcess)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROCESS, &CProcessListDlg::OnNMClickListProcess)
	ON_BN_CLICKED(IDBTN_PROCESS_SELECT, &CProcessListDlg::OnBnClickedProcessSelect)
	ON_BN_CLICKED(IDBTN_PROCESS_REFRESH, &CProcessListDlg::OnBnClickedProcessRefresh)
	ON_BN_CLICKED(IDC_BUTTON_UNINSTALL_DLL, &CProcessListDlg::OnBnClickedButtonUninstallDll)
	ON_EN_CHANGE(IDC_EDIT_PROCESS_TIPS, &CProcessListDlg::OnEnChangeEditProcessTips)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND_THREAT, &CProcessListDlg::OnBnClickedButtonSuspendThreat)
	ON_BN_CLICKED(IDC_BUTTON_RESUME_THREAT, &CProcessListDlg::OnBnClickedButtonResumeThreat)
END_MESSAGE_MAP()


// CProcessListDlg 消息处理程序
BOOL CProcessListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;

	// 进程列表获取编程语言列表视图控件的位置和大小   
	m_list_process.GetClientRect(&rect);
	// 为进程列表视图控件添加全行选中和栅格风格   
	m_list_process.SetExtendedStyle(m_list_process.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_SHOWSELALWAYS);
	CString itemArray[] = { CString("进程ID"),CString("文件名"),CString("标题"),CString("路径") };
	// 为进程列表视图控件添加列 
	int arrayLen = sizeof(itemArray) / sizeof(CString);
	for (int index = 0; index <arrayLen; index++) {
		if (index==0){
		m_list_process.InsertColumn(index, itemArray[index], LVCFMT_LEFT, (rect.Width() / arrayLen)*0.5,index);
		}else if (index == 3) {
			m_list_process.InsertColumn(index, itemArray[index], LVCFMT_LEFT, (rect.Width() / arrayLen)*1.5, index);
		}
		else {
			m_list_process.InsertColumn(index, itemArray[index], LVCFMT_LEFT, (rect.Width() / arrayLen), index);
		}
	}


	// 模块列表获取编程语言列表视图控件的位置和大小   
	m_list_module.GetClientRect(&rect);
	// 为模块列表视图控件添加全行选中和栅格风格   
	m_list_module.SetExtendedStyle(m_list_module.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_SHOWSELALWAYS);
	CString moduleItemArray[] = { CString("模块"),CString("映像基址"),CString("映像大小") };
	// 为模块列表视图控件添加三列 
	 arrayLen = sizeof(moduleItemArray) / sizeof(CString);
	for (int index = 0; index <arrayLen; index++) {
		if (index == 0) {
			m_list_module.InsertColumn(index, moduleItemArray[index], LVCFMT_LEFT, (rect.Width() / arrayLen + 1)*1.6, index);
		}
		else {
			m_list_module.InsertColumn(index, moduleItemArray[index], LVCFMT_LEFT, (rect.Width() / arrayLen + 1)*0.7, index);
		}
	}
	//获取进程列表数据
	this->InitListItem();
	return TRUE;
}

void CProcessListDlg::InitListItem() {
	map<DWORD, ProcessInfo> processMap = GetProcessList();
	map<DWORD, ProcessInfo>::iterator it;
	int index = 0, imageIndex = 0;
	CImageList *m_ImageList = new CImageList();
	m_ImageList->Create(16, 16, ILC_COLOR32, 0, 1);
	for (it = processMap.begin(); it != processMap.end(); it++) {
		CString PID;
		PID.Format("%d", (it->first));
		if (it->second.ImagePath != "") {
			HICON icon = fileIcon(it->second.ImagePath);
			m_ImageList->Add(icon);
			m_list_process.SetImageList(m_ImageList, LVSIL_SMALL);
			LVITEM lvItem;
			lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvItem.iImage = imageIndex;
			lvItem.iItem = index;
			lvItem.iSubItem = 0;
			lvItem.pszText = PID.GetBuffer();
			m_list_process.InsertItem(&lvItem);
			imageIndex++;

		}
		else {
			m_list_process.InsertItem(index, PID);
		}
		m_list_process.SetItemText(index, 1, it->second.FileName);
		m_list_process.SetItemText(index, 2, it->second.WindowsTitle);
		m_list_process.SetItemText(index, 3, it->second.ImagePath);
		index++;
	}
}



void CProcessListDlg::OnNMClickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString strProcessID;    // 选择的进程 strProcessName,  
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	*pResult = 0;
	//m_list_process
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项的PID和ProcessName 
		strProcessID = m_list_process.GetItemText(pNMListView->iItem, 0);
		strProcessName = m_list_process.GetItemText(pNMListView->iItem, 1);
		//DWORD pid= _ttoi(strProcessID);
		selectProcessID= _ttoi(strProcessID);
		//SetDlgItemText(IDC_EDIT_PROCESS_TIPS, strProcessName);
		selectProcessWPARAM = (WPARAM)strProcessName.GetString();
		InitModalIndirect(selectProcessID);
	}
}


void CProcessListDlg::InitModalIndirect(DWORD PID) 
{
	m_list_module.DeleteAllItems();
	map<DWORD, ModuleInfo> moduleMap = GetProcessModule(PID);
	map<DWORD, ModuleInfo>::iterator it;
	int index = 0;
	for (it = moduleMap.begin(); it != moduleMap.end(); it++) {
		m_list_module.InsertItem(index,it->second.ModuleName);
		m_list_module.SetItemText(index, 1, it->second.ImageBaseAddress);
		m_list_module.SetItemText(index, 2, it->second.ImageBaseSize);
		index++;
	}
	


}




void CProcessListDlg::OnBnClickedProcessSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (selectProcessID > 0&& selectProcessWPARAM!=NULL) {
	HWND DLG1Hwnd = ::FindWindow("bInjectHelper", NULL);
	::SendMessage(DLG1Hwnd, WM_CHOOSEPROCESS, selectProcessWPARAM, (LPARAM)selectProcessID);
	this->DestroyWindow();
	this->OnClose();
	}
	else {
		MessageBox("没有选中进程!");
	}
}


void CProcessListDlg::OnBnClickedProcessRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_list_process.DeleteAllItems();
	m_list_module.DeleteAllItems();
	this->InitListItem();
}


void CProcessListDlg::OnBnClickedButtonUninstallDll()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString str,str1;
	BOOL uninstallFlag = true;
	POSITION pos = m_list_module.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL|| selectProcessID<=0)
		AfxMessageBox("你没有选中模块DLL!");
	else
	{
		while (pos)   //如果你选择多行
		{
			int nIdx = -1;
			nIdx = m_list_module.GetNextSelectedItem(pos);
			if (nIdx >= 0 && nIdx<m_list_module.GetItemCount())
			{
				
				str = m_list_module.GetItemText(nIdx, 1);//获取指定列，为字符串形式
				DWORD baseAddress;
				sscanf(str.GetBuffer(), "%x", &baseAddress);//strtol(str.GetString(), &charBaseAddress, 16);
				uninstallFlag= UnInjectDll(selectProcessID, (LPVOID)baseAddress);

			}
		}
		if (uninstallFlag) {
			MessageBox("已卸载所有选中的DLL!");
		}
		else {
			MessageBox("已卸载部分选中的DLL!");
		}
	}
}


void CProcessListDlg::OnEnChangeEditProcessTips()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	int itemCount=m_list_process.GetItemCount();
	CString searchKey;
	GetDlgItemTextA(IDC_EDIT_PROCESS_TIPS, searchKey);
	if (searchKey != "") {		
	for (int index = 0; index < itemCount; index++) {
		m_list_process.SetItemState(index, 0, -1);
		CString itemValue = m_list_process.GetItemText(index, 1);
		if (itemValue.Find(searchKey)>=0) {
			m_list_process.SetItemState(index, LVIS_SELECTED| LVNI_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_list_process.EnsureVisible(index, FALSE);
			}
		}
	}
}


void CProcessListDlg::OnBnClickedButtonSuspendThreat()
{
	// TODO: 在此添加控件通知处理程序代码	
	if (SuspendProcess(selectProcessID, TRUE) == 1) {
	MessageBox("目标进程已挂起!");
	}
	else {
		MessageBox("目标进程已是挂起状态!");
	}
}


void CProcessListDlg::OnBnClickedButtonResumeThreat()
{
	// TODO: 在此添加控件通知处理程序代码
	if (SuspendProcess(selectProcessID, FALSE) == 1) {
	MessageBox("目标进程已恢复!");
	}
	else {
		MessageBox("目标进程已是正常状态!");
	}
}
