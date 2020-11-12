#pragma once
#include <windows.h>
#include <olectl.h>
#include <vector>
#pragma comment(lib, "oleaut32.lib")
using namespace std;

HRESULT SaveIcon(HICON hIcon, const char* path);
HICON fileIcon(CString extention);
int injectDll(DWORD processID, char* dll);
bool InjectShellCode(DWORD PID, char *ShellCode, int ShellSize);
typedef ULONG(WINAPI *PFNNtUnmapViewOfSection)(IN HANDLE ProcessHandle, IN PVOID BaseAddress);
BOOL UnmapViewOfModule(DWORD dwProcessId, LPVOID lpBaseAddr);
BOOL UnInjectDll(DWORD dwProcessId, LPVOID lpBaseAddr);
void SplitString(CString srcStr, CString subStr, vector<CString>* vecStr);
void SplitString(CString srcStr, CString subStr, CStringArray* strArr);
int htoi(char *s);