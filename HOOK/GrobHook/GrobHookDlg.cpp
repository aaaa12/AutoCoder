
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
	ON_BN_CLICKED(IDC_BUTTON2, &CGrobHookDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGrobHookDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CGrobHookDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CGrobHookDlg::OnBnClickedButton5)
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
_declspec(dllimport)  void SetHook(HWND hd,HHOOK &hKeyBoard);

HHOOK g_hKeyBoard;
void CGrobHookDlg::OnBnClickedBtnHook()
{
	// TODO: 在此添加控件通知处理程序代码
	SetHook(m_hWnd, g_hKeyBoard);
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
	else 
	{
		cmd = *((string*)lParam);
		if ("Clear" == cmd)
		{
			value = "";
			return 0;
		}
		else if("Search" == cmd)
		{
			//MessageBox(value.c_str());
			TypeText(value+".txt");
			return 0;
		}
	}
		
	 
	//MessageBox(stmp);
	//Sleep(1000);
	/*if (GetKeyState(VK_SHIFT))
	{
		keybd_event(VK_LSHIFT, 0, 0, 0);
		keybd_event(wParam, 0, 0, 0);
		keybd_event(wParam, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
	}
	else
	{
		keybd_event(wParam, 0, 0, 0);
		keybd_event(wParam, 0, KEYEVENTF_KEYUP, 0);
	}*/
		
	return 0;
}

/*
` 0xc0 ~

; 0xba :
= 0xbb +
, 0xbc <
- 0xbd _
. 0xbe >
/ 0xbf ?


[ 0xdb {
\ 0xdc |
] 0xdd }
' 0xde "
*/

bool CheckShift(char c, int &key)
{
	//number line
	string s = "!@#$%^&*()";
	for (int i = 0; i < 9; i++)
	{
		if (c == s[i])
		{
			key = i + '1';
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

bool CheckNoShift(char c, int &key)
{

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

void CGrobHookDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	
	ifstream myfile("A.txt");
	string temp;	
	int offset = 'A' - 'a';
	int key;
	while(getline(myfile,temp)) //按行读取字符串 	
	{	
		Sleep(2000);
		for (int i = 0; i < temp.size(); i++)
		{
			Sleep(1000);
			/*CString str;
			str.Format("%x", temp[i]);
			MessageBox(str);*/
			if ('a' <= temp[i] && temp[i] <= 'z')
			{
				key = temp[i] + offset;
				keybd_event(key, 0, 0, 0);
				keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
			}
			else if ('A' <= temp[i] && temp[i] <= 'Z')
			{
				keybd_event(VK_LSHIFT, 0, 0, 0);
				keybd_event(temp[i], 0, 0, 0);
				keybd_event(temp[i], 0, KEYEVENTF_KEYUP, 0);
				keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
			}
			else if (CheckShift(temp[i], key))
			{
				keybd_event(VK_LSHIFT, 0, 0, 0);
				keybd_event(key, 0, 0, 0);
				keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
			}
			else if (CheckNoShift(temp[i], key))
			{

				keybd_event(key, 0, 0, 0);
				keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
			}
			else
			{
				keybd_event(temp[i], 0, 0, 0);
				keybd_event(temp[i], 0, KEYEVENTF_KEYUP, 0);
			}
		}
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	} 	
	myfile.close();
	
	//ifstream infile;
	//string file = "A.txt";
	//infile.open(file.data());   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	//char c;infile >> noskipws;
	//while (!infile.eof())
	//{
	//	Sleep(1000);
	//	infile >> c;
	//	CString str;
	//	str.Format("%x",c);
	//	MessageBox(str);

	    //ASCII
		//a 0x61
		//A Ox41
		//回车 无
	    // tab 0x9
		// 0 0x30
	
		/*key
	    //VK_SHIFT(10)
			//SHIFT key
		VK_SPACE(20)
			SPACEBAR
		 0 (30)
			
		 A  (41)
			* /

		/*VK_TAB(09)
			TAB key*/
		//keybd_event(c, 0, 0, 0);
		//keybd_event(c, 0, KEYEVENTF_KEYUP, 0);

		//keybd_event('W', 0, 0, 0);
		//keybd_event('W', 0, KEYEVENTF_KEYUP, 0);

		//keybd_event('c', 0, 0, 0);
		//keybd_event('c', 0, KEYEVENTF_KEYUP, 0);



		
			
	//}
	//infile.close();             //关闭文件输入流 

}





void CGrobHookDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	clock_t var1, var2;
	var1 = clock();
	CString str;
	str.Format("%ld", var1);
	MessageBox(str);
}


void CGrobHookDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	char a=MapVirtualKey('A',MAPVK_VK_TO_CHAR);

	
	CString str;
	str.Format("%c", a);
	MessageBox(str);
}

void CGrobHookDlg::TypeStr(string str)
{
	//close caps lock 
	//BYTE btKeyState[256];
	//GetKeyboardState(btKeyState);
	//if (btKeyState[VK_CAPITAL] & 1)
	//{
	//	keybd_event(VK_CAPITAL, 0, 0, 0);
	//	keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	//}

	int offset = 'A' - 'a';
	int key;
	for (int i = 0; i < str.size(); i++)
	{
		Sleep(1000);
		/*CString str;
		str.Format("%x", temp[i]);
		MessageBox(str);*/
		if ('a' <= str[i] && str[i] <= 'z')
		{
			key = str[i] + offset;
			keybd_event(key, 0, 0, 0);
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
		}
		else if ('A' <= str[i] && str[i] <= 'Z')
		{
			keybd_event(VK_LSHIFT, 0, 0, 0);
			keybd_event(str[i], 0, 0, 0);
			keybd_event(str[i], 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
		}
		else if (CheckShift(str[i], key))
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
		else
		{
			keybd_event(str[i], 0, 0, 0);
			keybd_event(str[i], 0, KEYEVENTF_KEYUP, 0);
		}
	}
}
void CGrobHookDlg::TypeText(string path)
{

	TypeStr("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

	ifstream myfile(path);

	if (!myfile.good())
	{
		TypeStr("no file");
		myfile.close();
		return;
	}

	string temp;
	while (getline(myfile, temp)) //按行读取字符串 	
	{
		Sleep(1000);
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	}
	myfile.close();
}

void CGrobHookDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	Sleep(5000);
	
	TypeStr("ABC123");
}
