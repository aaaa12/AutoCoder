// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "HookDll.h"



HHOOK g_hMouse = NULL;
HHOOK g_hKeyBoard = NULL;
HINSTANCE g_hInst = NULL;
clock_t rtm;//retModel time
clock_t ct_old;//time 
clock_t ct_new;//time 
string state = "off";
WPARAM wOldParam;


//长度不超过8,这样，在其他线程中也能调用这个窗口
#pragma data_seg("MySec")
HWND g_hWnd = NULL;//一定要初始化化
HWND g_hFocusWnd = NULL;
#pragma data_seg()
#pragma comment(linker,"/section:MySec,RWS") //read write share
char *logBuffer;
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	g_hInst = hModule;
	//或者 GetModuleHandle
	logBuffer = new char[1024];
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void SetMsgToDlg(string str)
{
	sprintf(logBuffer, "%s", str.c_str());
	SendMessage(g_hWnd, WM_SEND_LOG, 0, (LPARAM)logBuffer);
}

LRESULT CALLBACK MouseProc(
int nCode,WPARAM wParam,LPARAM lParam) 
{
	
	//if ("on"== state &&wParam == WM_LBUTTONDOWN)
	//{

	//	SetMsgToDlg("结束命令，原因：鼠标点击");
	//	state = "off";
	//	PostMessage(g_hWnd, WM_SEND_TEXT2WIN, wParam, HOOK_CMD_CLEAR);

	//}
	return CallNextHookEx(g_hMouse, nCode, wParam, lParam);
}



LRESULT CALLBACK KeyBoardProc(
	int nCode, WPARAM wParam, LPARAM lParam)
{
	if (lParam & 0x80000000)//31判断按下还是弹起，这里只处理按下
	{
		return CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
	}

	//每次键盘事件更新当前输入的句柄，只有在这才能正确获取当前句柄，因为调用该函数是当前进程
	g_hFocusWnd = GetFocus();


	//do nothing if is alt or shfit
	//命令获取时，屏蔽alt，避免触发当前应用的快捷菜单
	if ((18 == wParam|| VK_SHIFT == wParam)&& "on"==state)
	{
		return 1; 
	}
	
	if (VK_F2 == wParam)//F2关闭
	{
		SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		UnhookWindowsHookEx(g_hKeyBoard);
		return 1;
	}
	else if ("off"==state && 1 == (lParam >> 29 & 1) && ('1' == wParam))//alt+1
	{
		rtm= clock();
		SetMsgToDlg("接收命令...");
		state = "on";
		PostMessage(g_hWnd, WM_SEND_TEXT2WIN, wParam, HOOK_CMD_SHOW);

		return  1;
	}
	else if ("on"== state && 1 == (lParam >> 29 & 1) && ('2' == wParam))//alt+2
	{

		SetMsgToDlg("提交命令...");
		state = "off";
		PostMessage(g_hWnd, WM_SEND_TEXT2WIN, wParam, HOOK_CMD_SUBMIT);
		return 1;
	}
	else if ("on" == state && 0 == (lParam >> 29 & 1))//非alt
	{
		PostMessage(g_hWnd, WM_SEND_TEXT2WIN, wParam, 0);//错误记录：本来是sendMessage的，会导致编辑器一直等待输入而卡死。
		return  1;
	};
	
	return  CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
	

}
void SetHook(HWND hWnd, HHOOK &hKeyBoard, HHOOK &hMouse)
{
	ct_old = clock();
	g_hWnd = hWnd;

	g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, g_hInst, 0);
	g_hMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hInst, 0);
	hKeyBoard = g_hKeyBoard;//rlease when close
	hMouse = g_hMouse;
	SetMsgToDlg("挂钩成功");
	
}

void GetFocusWnd(HWND &hWnd)
{
	hWnd= g_hFocusWnd;
}

