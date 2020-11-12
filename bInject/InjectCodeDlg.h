#pragma once
#include"XEDParse.h"
#include "afxwin.h"

/*
#ifdef _WIN64 //64位
//XEDParse 汇编引擎x64
#include "lib/XEDParse_dll/XEDParse.h"
#pragma comment(lib,"ib/XEDParse_dllXEDParse_x64.lib")
#define Platform true;

#else //32位
//XEDParse 汇编引擎x86
#include "lib/XEDParse_dll/XEDParse.h"
#include "afxwin.h"
#pragma comment(lib,"lib/XEDParse_dll/XEDParse_x86.lib")
#define Platform false;
#endif


#define XEDPARSE_CALL //calling convention
#define XEDPARSE_MAXBUFSIZE 256
#define XEDPARSE_MAXASMSIZE 16


//typedefs
typedef bool (XEDPARSE_CALL* CBXEDPARSE_UNKNOWN)(const char* text, ULONGLONG* value);

//XEDParse enums
enum XEDPARSE_STATUS
{
	XEDPARSE_ERROR = 0,
	XEDPARSE_OK = 1
};

//XEDParse structs
#pragma pack(push,8)
struct XEDPARSE
{
	bool x64; // use 64-bit instructions
	ULONGLONG cip; //instruction pointer (for relative addressing)
	unsigned int dest_size; //destination size (returned by XEDParse)
	CBXEDPARSE_UNKNOWN cbUnknown; //unknown operand callback
	unsigned char dest[XEDPARSE_MAXASMSIZE]; //destination buffer
	char instr[XEDPARSE_MAXBUFSIZE]; //instruction text
	char error[XEDPARSE_MAXBUFSIZE]; //error text (in case of an error)
};
#pragma pack(pop)

typedef XEDPARSE_STATUS XEDPARSE_CALL XEDParseAssemble(XEDPARSE* XEDParse);*/


typedef   XEDPARSE_STATUS XEDPARSE_CALL(*myXEDParseAssembleFunc)(XEDPARSE* XEDParse) ;

 struct XED_STRUCT  {
	CString AsmCode;
	CString ShellCode;
	int ShellcodeLen;
	CString OPCodeStr;
	CStringArray AsmCodeArray;
};

// CInjectCodeDlg 对话框

class CInjectCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInjectCodeDlg)

public:
	CInjectCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInjectCodeDlg();
	virtual BOOL OnInitDialog();
	CEdit m_edit_process;
	CEdit m_edit_process_id;
	BOOL SupportASM=false;
	afx_msg void OnBnClickedButtonOpenProcessDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INJECT_CODE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_edit_code;
	CEdit m_edit_opcode;
	afx_msg void OnBnClickedGenerateCode();
	BOOL CInjectCodeDlg::GenerateOPcode(XED_STRUCT* my_xed);
	int  CInjectCodeDlg::GetInjectShellCode(char* shellcode);

};
