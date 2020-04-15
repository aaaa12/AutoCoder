
// GrobHookDlg.h: 头文件
//

#pragma once
#include "MsgDlg.h"
#include "HookDll.h"

// CGrobHookDlg 对话框
class CGrobHookDlg : public CDialogEx
{
// 构造
public:
	CGrobHookDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROBHOOK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
private:
private:
	// 输出文件方式
	int iOldMod;//m_iMod的旧值，防止重复触发点击事件
	int m_iMod;//0.hide cmd windows; 1.simulate type;2 WM_CAHR Message  ;3 use ClipBoard
	int iRetMod;//when iTypeMod==2 how to change line. 1.simulate type enter key 2.post char value
	int WaitTime = 50;//interval time between two pype
	string cmd;
	string value;
	const char *withShift = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<> ?	";
	const char   *noShift = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,. /	";
	const char *AwithShift = "QWERTYUIOPASDFGHJKLZXCVBNM";
	const char   *AnoShift = "qwertyuiopasdfghjklzxcvbnm";
	const int offset = 'A' - 'a';
	char SwitchShift(char c);//to uppercase
	char SwitchNoShift(char c);
	bool CheckNoShift(char c, int &key);//to key code
	bool CheckShift(char c, int &key);
	string TrimStr(string s);
	void SpaceAndTabNum(string str, int& sNum, int &tNum);//count space and tab
	void TypeKeyBackSpace(int times);
	void TypeKeyEnter(string proLine, int &sNum, int &tNum);//proLine 上一行
	CMsgDlg* m_pMsgDlg;
	HHOOK g_hKeyBoard;
	HHOOK g_hMouse;

	//out put
	void TypeStr(CString str);
	bool TypeTextFile(CString path);
	bool OutPutFile(CString path);
	bool FileToClip(CString path);

	CString GetClipBoradText();
	void SaveStrToFile(CString fileName, CString str);
	CString ReadStrFromFile(CString path);

	CString sLog[10];
	int iLogPos = 0;
	void ShowLog(CString string);

	void MakeDir(CString path);
	CString GetFilePath(CString fileName);

	void AnysSleep(int iClock);
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSendText2Win(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSendLog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnHook();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
