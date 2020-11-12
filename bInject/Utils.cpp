#include  "stdafx.h"
#include "Utils.h"
#include <iostream>



HRESULT SaveIcon(HICON hIcon, const char* path)
	{
		// Create the IPicture intrface
		PICTDESC desc = { sizeof(PICTDESC) };
		desc.picType = PICTYPE_ICON;
		desc.icon.hicon = hIcon;
		IPicture* pPicture = 0;
		HRESULT hr = OleCreatePictureIndirect(&desc, IID_IPicture, FALSE, (void**)&pPicture);
		if (FAILED(hr)) return hr;

		// Create a stream and save the image
		IStream* pStream = 0;
		CreateStreamOnHGlobal(0, TRUE, &pStream);
		LONG cbSize = 0;
		hr = pPicture->SaveAsFile(pStream, TRUE, &cbSize);

		// Write the stream content to the file
		if (!FAILED(hr)) {
			HGLOBAL hBuf = 0;
			GetHGlobalFromStream(pStream, &hBuf);
			void* buffer = GlobalLock(hBuf);
			HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (!hFile) hr = HRESULT_FROM_WIN32(GetLastError());
			else {
				DWORD written = 0;
				WriteFile(hFile, buffer, cbSize, &written, 0);
				CloseHandle(hFile);
			}
			GlobalUnlock(buffer);
		}
		// Cleanup
		pStream->Release();
		pPicture->Release();
		return hr;

	}

	// 获取文件图标
HICON fileIcon(CString extention)
	{
		HICON icon = NULL;
		if (extention.GetLength() > 0)
		{
			LPCSTR name = extention.GetBuffer(); //extention.c_str();
			SHFILEINFOA info;
			if (SHGetFileInfoA(name,
				FILE_ATTRIBUTE_NORMAL,
				&info,
				sizeof(info),
				SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
			{
				icon = info.hIcon;
			}
		}

		return icon;
	}



int htoi(char *s)
{
	int i;
	int n = 0;
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
		i = 2;
	}
	else
	{
		i = 0;
	}
	for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i)
	{
		if (tolower(s[i]) > '9')
		{
			n = 16 * n + (10 + tolower(s[i]) - 'a');
		}
		else
		{
			n = 16 * n + (tolower(s[i]) - '0');
		}
	}
	return n;
}


bool InjectShellCode(DWORD PID,char *ShellCode,int ShellSize) {
	HANDLE Process;
	LPVOID pThread;
	Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	pThread = VirtualAllocEx(Process, NULL, ShellSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(Process, pThread, ShellCode, ShellSize, NULL);
	HANDLE hThread = CreateRemoteThread(Process, 0, 0, (LPTHREAD_START_ROUTINE)pThread, NULL, 0, NULL);
	if (!hThread) {
		//printf("创建远程线程失败!");
		return false;
	}
	//等待远程线程结束
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	VirtualFreeEx(Process, pThread, 0, MEM_RELEASE);
	VirtualFreeEx(Process, pThread, 0, MEM_RELEASE);
	CloseHandle(Process);
	return true;
}

int injectDll(DWORD processID, char* dll) {
	if (processID != 0 && dll != NULL) {
		// Get handle of process
		HANDLE procHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, processID);

		// Get address of LoadLibraryA
		HANDLE loadLibraryAddress = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
		int cb = (lstrlenA(dll) + 1) * sizeof(char);
		// Allocate memory on target process
		HANDLE allocateMemoryAdress = VirtualAllocEx(procHandle, NULL, cb, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		// Write name of dll in process
		SIZE_T bytesWritten;
		WriteProcessMemory(procHandle, allocateMemoryAdress, (PVOID)dll, cb, &bytesWritten);

		// Create thread to call LoadLibraryA
		CreateRemoteThread(procHandle, NULL, 0, (PTHREAD_START_ROUTINE)loadLibraryAddress, allocateMemoryAdress, 0, NULL);
		return 1;
	}
	return 0;
}

BOOL UnmapViewOfModule(DWORD dwProcessId, LPVOID lpBaseAddr)
{
	    HMODULE hModule = GetModuleHandle("ntdll.dll");
	   if (hModule == NULL)
		        hModule = LoadLibrary("ntdll.dll");
	
		    PFNNtUnmapViewOfSection pfnNtUnmapViewOfSection = (PFNNtUnmapViewOfSection)GetProcAddress(hModule, "NtUnmapViewOfSection");
	
		    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessId);
	    ULONG    ret = pfnNtUnmapViewOfSection(hProcess, lpBaseAddr);
	    CloseHandle(hProcess);
	    return ret ? FALSE : TRUE;
}

BOOL UnInjectDll( DWORD dwProcessId, LPVOID lpBaseAddr)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessId);
	LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "FreeLibrary");
	if (NULL == lpThreadFun)
	{
		::CloseHandle(hProcess);
		return false;
	}
	// 创建远程线程调用 FreeLibrary   
	HANDLE  hThread = ::CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, lpBaseAddr, 0, NULL);
	if (NULL == hThread)
	{
		::CloseHandle(hProcess);
		return false;
	}
	// 等待远程线程结束   
	::WaitForSingleObject(hThread, INFINITE);
	// 清理   
	::CloseHandle(hThread);
	::CloseHandle(hProcess);
	return true;
}


void SplitString(CString srcStr, CString subStr, vector<CString>* vecStr) {
	int index = srcStr.Find(subStr);
	CString tempStr= srcStr,arrStr;
	int subLen = subStr.GetLength();
	//for(int index = -1; index >= 0; index= srcStr.Find(subStr)) {
	while(true){
		if (index >= 0) {
			arrStr = tempStr.Left(index);
			vecStr->insert(vecStr->begin()+vecStr->size(), arrStr);
			tempStr = tempStr.Right(tempStr.GetLength()-index- subLen);
			index = tempStr.Find(subStr);
		}
		else if(vecStr->size()>0&& tempStr.GetLength()>0){
			vecStr->insert(vecStr->begin() + vecStr->size(), tempStr);
			tempStr.Empty();
		}
		else {
			break;
		}
	}
}

void SplitString(CString srcStr, CString subStr, CStringArray* strArr) {
	int index = srcStr.Find(subStr);
	CString tempStr = srcStr, arrStr;
	int subLen = subStr.GetLength();
	//for(int index = -1; index >= 0; index= srcStr.Find(subStr)) {
	while (true) {
		if (index >= 0) {
			arrStr = tempStr.Left(index);
			strArr->Add(arrStr); //insert(vecStr->begin() + vecStr->size(), arrStr);
			tempStr = tempStr.Right(tempStr.GetLength() - index - subLen);
			index = tempStr.Find(subStr);
		}
		else if ( tempStr.GetLength()>0) { //strArr->GetSize()>0 &&
			//vecStr->insert(vecStr->begin() + vecStr->size(), tempStr);
			strArr->Add(tempStr);
			tempStr.Empty();
		}
		else {
			break;
		}
	}

}