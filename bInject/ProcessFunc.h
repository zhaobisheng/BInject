#pragma once
#include<map>
#include <windows.h>
using namespace std;

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define STATUS_SUCCESS              ((NTSTATUS) 0x00000000)
#define SystemProcessesAndThreadsInformation    5 // 功能号
#define NTAPI    __stdcall

typedef   struct ProcessInfo
{
	DWORD PID;
	CString FileName;
	CString WindowsTitle;
	CString ImagePath;
};

typedef struct ModuleInfo
{
	CString ModuleName;
	CString ImageBaseAddress;
	CString ImageBaseSize;
};

// 线程状态的枚举常量
typedef enum _THREAD_STATE
{
	StateInitialized, // 初始化状态
	StateReady, // 准备状态
	StateRunning, // 运行状态
	StateStandby, // 
	StateTerminated,//关闭
	StateWait, // 等待
	StateTransition, // 切换???
	StateUnknown
}THREAD_STATE;

// 线程处于等待的原因的枚举常量
typedef enum _KWAIT_REASON
{
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVirtualMemory,
	WrPageOut,
	WrRendezvous,
	Spare2,
	Spare3,
	Spare4,
	Spare5,
	Spare6,
	WrKernel,
	MaximumWaitReason
}KWAIT_REASON;




typedef LONG   NTSTATUS;
typedef LONG    KPRIORITY;

typedef struct _CLIENT_ID
{
	DWORD        UniqueProcess;
	DWORD        UniqueThread;
} CLIENT_ID, *PCLIENT_ID;


typedef struct _VM_COUNTERS
{
	SIZE_T        PeakVirtualSize;
	SIZE_T        VirtualSize;
	ULONG         PageFaultCount;
	SIZE_T        PeakWorkingSetSize;
	SIZE_T        WorkingSetSize;
	SIZE_T        QuotaPeakPagedPoolUsage;
	SIZE_T        QuotaPagedPoolUsage;
	SIZE_T        QuotaPeakNonPagedPoolUsage;
	SIZE_T        QuotaNonPagedPoolUsage;
	SIZE_T        PagefileUsage;
	SIZE_T        PeakPagefileUsage;
} VM_COUNTERS;

// 线程信息结构体
typedef struct _SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER   KernelTime;
	LARGE_INTEGER   UserTime;
	LARGE_INTEGER   CreateTime;
	ULONG           WaitTime;
	PVOID           StartAddress;
	CLIENT_ID       ClientId;
	KPRIORITY       Priority;
	KPRIORITY       BasePriority;
	ULONG           ContextSwitchCount;
	LONG            State;// 状态,是THREAD_STATE枚举类型中的一个值
	LONG            WaitReason;//等待原因, KWAIT_REASON中的一个值
} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;


typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

// 进程信息结构体
typedef struct _SYSTEM_PROCESS_INFORMATION
{
	ULONG            NextEntryDelta; // 指向下一个结构体的指针
	ULONG            ThreadCount; // 本进程的总线程数
	ULONG            Reserved1[6]; // 保留
	LARGE_INTEGER    CreateTime; // 进程的创建时间
	LARGE_INTEGER    UserTime; // 在用户层的使用时间
	LARGE_INTEGER    KernelTime; // 在内核层的使用时间
	UNICODE_STRING   ProcessName; // 进程名
	KPRIORITY        BasePriority; // 
	ULONG            ProcessId; // 进程ID
	ULONG            InheritedFromProcessId;
	ULONG            HandleCount; // 进程的句柄总数
	ULONG            Reserved2[2]; // 保留
	VM_COUNTERS      VmCounters;
	IO_COUNTERS      IoCounters;
	SYSTEM_THREAD_INFORMATION Threads[5]; // 子线程信息数组
}SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

map<DWORD, ProcessInfo> GetProcessList();
map<DWORD, ModuleInfo> GetProcessModule(DWORD dwPid);
int SuspendProcess(DWORD dwProcessID, BOOL fSuspend);
typedef DWORD(WINAPI* PQUERYSYSTEM)(UINT, PVOID, DWORD, PDWORD);
int IsThreadSuspend(DWORD dwProcessID, DWORD dwThreadID);