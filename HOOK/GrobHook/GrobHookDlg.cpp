﻿
// GrobHookDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GrobHook.h"
#include "GrobHookDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrobHookDlg 对话框



CGrobHookDlg::CGrobHookDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GROBHOOK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGrobHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGrobHookDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SEND_TEXT2WIN, OnSendText2Win)
	ON_BN_CLICKED(IDC_BTN_HOOK, &CGrobHookDlg::OnBnClickedBtnHook)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CGrobHookDlg 消息处理程序

BOOL CGrobHookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (NULL == m_pMsgDlg)
	{
		// 创建非模态对话框实例  WS_EX_NOACTIVATE 
		m_pMsgDlg = new CMsgDlg();
		m_pMsgDlg->Create(IDD_MSG, this);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGrobHookDlg::OnPaint()
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
HCURSOR CGrobHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//原名引用
_declspec(dllimport)  void SetHook(HWND hd,HHOOK &hKeyBoard, HHOOK &hMouse);

void CGrobHookDlg::OnBnClickedBtnHook()
{
	// TODO: 在此添加控件通知处理程序代码
	SetHook(m_hWnd, g_hKeyBoard,g_hMouse);
}

//const char *noShift = "`1234567890-=";
//const char * withShit=
string cmd;
string value;
const char *withShift = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<> ?	";
const char   *noShift = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,. /	";

const char *AwithShift = "QWERTYUIOPASDFGHJKLZXCVBNM";
const char   *AnoShift = "qwertyuiopasdfghjklzxcvbnm";
char SwitchShift(char c)
{
	int p = 0;
	while (0 != noShift[p])
	{
		if (c == noShift[p])
			return withShift[p];
		p++;
	}
	return 0;
}
char SwitchNoShift(char c)
{
	int p = 0;
	while (0 != AwithShift[p])
	{
		if (c == AwithShift[p])
			return AnoShift[p];
		p++;
	}
	return 0;
}



LRESULT CGrobHookDlg::OnSendText2Win(WPARAM wParam, LPARAM lParam)
{

	if (lParam == 0)
	{
		if (VK_BACK == wParam)//key code of Backspace
		{
			if(value.size() > 0)
				value = value.substr(0, value.size() - 1);
		}
		else
		{
			char c = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
			if ('A' <= c && c <= 'Z')
			{
				if (!GetAsyncKeyState(VK_SHIFT))
					c = SwitchNoShift(c);
			}
			else {
				if (GetAsyncKeyState(VK_SHIFT))
				{
					c = SwitchShift(c);
				}
			}

			value = value + c;

		}
		if(!m_pMsgDlg->IsWindowVisible())
			m_pMsgDlg->ShowWindow(SW_SHOWNOACTIVATE);
		m_pMsgDlg->SetTxt(value.c_str());
	}
	else    


	{
		cmd = *((string*)lParam);
		if ("Clear" == cmd)
		{
			value = "";
			if(m_pMsgDlg->IsWindowVisible())
				m_pMsgDlg->ShowWindow(SW_HIDE);
			m_pMsgDlg->SetTxt("");
			return 0;
		}
		else if("Search" == cmd)
		{
			if (m_pMsgDlg->IsWindowVisible())
				m_pMsgDlg->ShowWindow(SW_HIDE);
			m_pMsgDlg->SetTxt("");
			value = value + ".txt";

			while (GetAsyncKeyState(18)) {};//wait for alt up


			TypeTextFile(value);

			value = "";
			return 0;
		}
	}
		
	return 0;
}

//no shift type value
bool CheckShift(char c, int &key)
{
	if ('A' <= c && c <= 'Z')
	{
		key = c;
		return true;
	}

	//number line  1234567890
	string s =    ")!@#$%^&*(";
	for (int i = 0; i < 10; i++)
	{
		if (c == s[i])
		{
			key = i + '0';
			return true;
		}
	}
	//~
	if ('~' == c)
	{
		key = 0xc0;
		return true;
	}

	string s1 = ":+<_>?";
	for (int i = 0; i < s1.size(); i++)
	{
		if (c == s1[i])
		{
			key = i + 0xba;
			return true;
		}
	}
	string s2 = "{|}\"";
	for (int i = 0; i < s2.size(); i++)	{
		if (c == s2[i])
		{
			key = i + 0xdb;
			return true;
		}
	}

	return false;
}
const int offset = 'A' - 'a';
bool CheckNoShift(char c, int &key)
{
	if ('a' <= c && c <= 'z')
	{
		key = c + offset;
		return true;
	}

	if ('0' <= c && c <= '9')
	{
		key = c;
		return true;
	}

	if (' '==c)
	{
		key = c;
		return true;
	}

	if ('`' == c)
	{
		key = 0xc0;
		return true;
	}

	string s1 = ";=,-./";
	for (int i = 0; i < s1.size(); i++)
	{
		if (c == s1[i])
		{
			key = i + 0xba;
			return true;
		}
	}
	string s2 = "[\\]'";
	for (int i = 0; i < s2.size(); i++)
	{
		if (c == s2[i])
		{
			key = i + 0xdb;
			return true;
		}
	}
	return false;

}
int WaitTime = 16;

void CGrobHookDlg::TypeStr(string str)
{
	//close caps lock 
	BYTE btKeyState[256];
	GetKeyboardState(btKeyState);
	if (btKeyState[VK_CAPITAL] & 1)
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}

	int key;
	for (int i = 0; i < str.size(); i++)
	{

		Sleep(WaitTime);
		if (CheckShift(str[i], key))
		{
			keybd_event(VK_LSHIFT, 0, 0, 0);
			keybd_event(key, 0, 0, 0);
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
		}
		else if (CheckNoShift(str[i], key))
		{
			keybd_event(key, 0, 0, 0);
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
		}
		else//should be no other
		{
			keybd_event(str[i], 0, 0, 0);
			keybd_event(str[i], 0, KEYEVENTF_KEYUP, 0);
		}
	}
}



void trim(string &s)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
	}
}

void  SpaceAndTabNum(string str,int& sNum,int &tNum)
{
	int p=0;
	sNum = 0;//space count
	tNum = 0;//tab count
	while (' ' == str[p] || '	' == str[p])
	{
		if (' ' == str[p])
			sNum++;
		if ('	' == str[p])
			tNum++;
		p++;
	}
}
void TypeBackSpace(int times)
{
	for (int i = 0; i < times; i++)
	{
		Sleep(WaitTime);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
}
void CGrobHookDlg::TypeTextFile(string path)
{
	ifstream myfile(path);

	if (!myfile.good())
	{
		TypeStr("no file");
		myfile.close();
		return;
	}

	string temp;
	int sNum=0,tNum=0;
	while (getline(myfile, temp)) //line by line call TypeStr
	{
		Sleep(100);

		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
		TypeBackSpace(sNum+ tNum);//delete space and tab int prior line begining
		SpaceAndTabNum(temp, sNum, tNum);
	
		TypeStr(temp);
		
	}
	myfile.close();
}

void CGrobHookDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (NULL != m_pMsgDlg)
	{
		// 创建非模态对话框实例   
		m_pMsgDlg->DestroyWindow();
		delete m_pMsgDlg;
		m_pMsgDlg = NULL;
	}
	UnhookWindowsHookEx(g_hKeyBoard);
	UnhookWindowsHookEx(g_hMouse);
}