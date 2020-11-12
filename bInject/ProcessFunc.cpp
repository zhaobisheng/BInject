#include   "stdafx.h"
#include "ProcessFunc.h"
#include "Process.h"
#include <tlhelp32.h>
#include <Psapi.h>
#pragma comment(lib,"psapi.lib")



CString GetImageFullPath(DWORD dwPid)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL)
		return "";
	char path[MAX_PATH] = { 0 };
	GetModuleFileNameEx(hProcess, NULL, path, MAX_PATH);
	CloseHandle(hProcess);
	CString imagePath(path);
	return imagePath;
}


map<DWORD, ProcessInfo> GetProcessList()
{
	map<DWORD, ProcessInfo> processMap;
	HANDLE hwnd;
	PROCESSENTRY32 pro;
	
	pro.dwSize = sizeof(PROCESSENTRY32);
	BOOL flag;
	hwnd = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hwnd != 0) {		
	flag = Process32First(hwnd, &pro);
	while (flag) {
		ProcessInfo myProcessInfo;
		//mp.insert(pair<DWORD, WCHAR>(pro.th32ProcessID, pro.szExeFile));
		CString tempName(pro.szExeFile);
		//strcmp(tempName, pro.szExeFile);
		//StrCpyW(tempName, pro.szExeFile);
		myProcessInfo.FileName = tempName;
		myProcessInfo.PID = pro.th32ProcessID;
		myProcessInfo.ImagePath=GetImageFullPath(pro.th32ProcessID);
		processMap[pro.th32ProcessID] = myProcessInfo;
		flag = Process32Next(hwnd, &pro);
	}
	CloseHandle(hwnd);
	}

	return processMap;
}

map<DWORD, ModuleInfo> GetProcessModule(DWORD dwPid)
{
	map<DWORD, ModuleInfo> moduleMap;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (INVALID_HANDLE_VALUE != hSnapshot)
	{	
	MODULEENTRY32 mi;
	mi.dwSize = sizeof(MODULEENTRY32); //第一次使用必须初始化成员
	BOOL  flag = Module32First(hSnapshot, &mi);
	DWORD mapIndex = 0;
	while (flag)
	{		
		ModuleInfo myModuleInfo;
		myModuleInfo.ModuleName = CString(mi.szExePath);
		CString tempImageBaseAddress, tempImageBaseSize;
		tempImageBaseAddress.Format("0x%x", mi.modBaseAddr);
		myModuleInfo.ImageBaseAddress = tempImageBaseAddress;
		tempImageBaseSize.Format("0x%x", mi.modBaseSize);
		myModuleInfo.ImageBaseSize = tempImageBaseSize;
		moduleMap[mapIndex] = myModuleInfo;
		mapIndex++;
		flag = Module32Next(hSnapshot, &mi);
	}
	CloseHandle(hSnapshot);
	}
	return moduleMap;
}


int SuspendProcess(DWORD dwProcessID, BOOL fSuspend)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPTHREAD, dwProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{

		THREADENTRY32 te = { sizeof(te) };
		BOOL fOk = Thread32First(hSnapshot, &te);
		for (; fOk; fOk = Thread32Next(hSnapshot, &te))
		{
			if (te.th32OwnerProcessID == dwProcessID)
			{		
				
					HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME,
						FALSE, te.th32ThreadID);
					if (hThread != NULL){
					if (fSuspend)
					{
						if (IsThreadSuspend(dwProcessID, te.th32ThreadID) == 0) {
							SuspendThread(hThread);
						}
						else {
							return 0;
						}
					}
					else
					{
						if (IsThreadSuspend(dwProcessID, te.th32ThreadID) == 1) {
							ResumeThread(hThread);
						}
						else {
							return 0;
						}
					}
					}
					CloseHandle(hThread);
				}			
			}
		
	}
	CloseHandle(hSnapshot);
	return 1;
}





/*
※※* 函数名 : IsThreadSuspend
※※* 功  能 : 获取线程是被是否被挂起
※※* 返回值 : int 1： 表示线程被挂起 , 0： 表示线程正常。 -1 未知状态
※※* 形  参 : DWORD dwThreadID 要验证的线程的ID
※※* 形  参 : wchar_t * szProcessName 表示线程所在的进程名
*/
int IsThreadSuspend(DWORD dwProcessID, DWORD dwThreadID)
{
	int nRet = 0;
	NTSTATUS Status = 0;


	PQUERYSYSTEM NtQuerySystemInformation = NULL;
	PSYSTEM_PROCESS_INFORMATION pInfo = { 0 };

	// 获取函数地址
	NtQuerySystemInformation = (PQUERYSYSTEM)
		GetProcAddress(LoadLibrary("ntdll.dll"), "NtQuerySystemInformation");


	DWORD   dwSize = 0;
	// 获取信息所需的缓冲区大小
	Status = NtQuerySystemInformation(SystemProcessesAndThreadsInformation,//要获取的信息的类型
		NULL, // 用于接收信息的缓冲区
		0,  // 缓冲区大小
		&dwSize
		);
	// 申请缓冲区
	char* pBuff = new char[dwSize];
	pInfo = (PSYSTEM_PROCESS_INFORMATION)pBuff;
	if (pInfo == NULL)
		return -1;
	// 再次调用函数, 获取信息
	Status = NtQuerySystemInformation(SystemProcessesAndThreadsInformation,//要获取的信息的类型
		pInfo, // 用于接收信息的缓冲区
		dwSize,  // 缓冲区大小
		&dwSize
		);
	if (!NT_SUCCESS(Status)) {/*如果函数执行失败*/
		printf("失败\n");
		delete[] pInfo;
		return -1;
	}

	// 遍历结构体,找到对应的进程
	while (1) {
		// 判断是否还有下一个进程
		if (pInfo->NextEntryDelta == 0)
			break;

		// 判断是否找到了ID
		if (pInfo->ProcessId == dwProcessID) {

			// 找到该进程下的对应的线程,也就是遍历所有线程
			for (DWORD i = 0; i < pInfo->ThreadCount; i++) {
				if (pInfo->Threads[i].ClientId.UniqueThread == dwThreadID) { //找到线程 
																			 //如果线程被挂起
					if (pInfo->Threads[i].State == StateWait
						&& pInfo->Threads[i].WaitReason == Suspended) {
						nRet = 1;
						break;
					}
				}
			}
			break;
		}
		// 迭代到下一个节点
		pInfo = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);
	}

	delete[] pBuff;
	return nRet;
}