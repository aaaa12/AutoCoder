
// GrobHookDlg.h: 头文件
//

#pragma once
#include <bits/stdc++.h>
using namespace std;

#define WM_SEND_TEXT2WIN (WM_USER+1)

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
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSendText2Win(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnHook();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
private:
	void TypeStr(string str);
	void TypeText(string path);
public:
	afx_msg void OnBnClickedButton5();
};
