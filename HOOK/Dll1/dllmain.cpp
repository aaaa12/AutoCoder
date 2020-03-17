﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include<windows.h>
#include <stdio.h>
#include <fstream>
#include <cassert>
#include <bits/stdc++.h>
using namespace std;

HHOOK g_hMouse = NULL;
HHOOK g_hKeyBoard = NULL;
HINSTANCE g_hInst = NULL;
bool retModel = false;//retrun model
clock_t rtm;//retModel time
clock_t ct_old;//time 
clock_t ct_new;//time 
string sCmdClear = "Clear";
string sCmdSearch = "Search";

//长度不超过8,这样，在其他线程中也能调用这个窗口
#pragma data_seg("MySec")
HWND g_hWnd = NULL;//一定要初始化化
#pragma data_seg()
#pragma comment(linker,"/section:MySec,RWS") //read write share

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	g_hInst = hModule;
	//或者 GetModuleHandle

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


LRESULT CALLBACK MouseProc(
int nCode,WPARAM wParam,LPARAM lParam) 
{
	if (wParam == WM_LBUTTONDOWN&& retModel)
	{
		retModel = false;
		PostMessage(g_hWnd, WM_USER + 1, wParam, (LPARAM)&sCmdClear);

	}

	return CallNextHookEx(g_hMouse, nCode, wParam, lParam);;
}

void SetMsgToDlg(string str)
{
	HDC hdc = GetDC(g_hWnd);
	char buf[20];
	memset(buf, 0, 20);
	TextOut(hdc, 0, 0, str.c_str(), strlen(str.c_str()));
	ReleaseDC(g_hWnd, hdc);
}


LRESULT CALLBACK KeyBoardProc(
	int nCode, WPARAM wParam, LPARAM lParam)
{
	//do nothing if is alt or shfit
	if (18 == wParam&& retModel)
	{
		return 1;
	}
		
	if (VK_SHIFT == wParam)
	{
		return  CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
	}

	//time check
	ct_new = clock();
	if (ct_new - ct_old < 200)
	{
		if (retModel)
			return 1;
		else
		    return  CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
	}
	ct_old = clock();

	//if (clock() - rtm>30000) //time out
	//{
	//	retModel = false;
	//	SendMessage(g_hWnd, WM_USER + 1, wParam, (LPARAM) &sCmdClear);
	//	return  CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
	//}

	if (VK_F2 == wParam)//F2关闭
	{
		SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		UnhookWindowsHookEx(g_hKeyBoard);
		return 1;
	}
	else if (1 == (lParam >> 29 & 1) && ('1' == wParam))//alt+1
	{
		rtm= clock();
		SetMsgToDlg("读取命令");
		retModel = true;
		return  1;
	}
	else if (retModel &&1 == (lParam >> 29 & 1) && ('2' == wParam))//alt+2
	{
		retModel = false;
		SetMsgToDlg("执行命令");
		PostMessage(g_hWnd, WM_USER + 1, wParam, (LPARAM)&sCmdSearch);
		return 1;
	}
	else if (retModel)//非alt
	{
		PostMessage(g_hWnd, WM_USER + 1, wParam, 0);//错误记录：本来是sendMessage的，会导致编辑器一直等待输入而卡死。
		return  1;
	}
	else
	{
		return  CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
	}

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