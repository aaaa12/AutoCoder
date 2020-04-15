#pragma once
#include<windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <time.h>
#include <string>
#include <vector>
using namespace std;

#define WM_SEND_TEXT2WIN (WM_USER+1)
#define WM_SEND_LOG (WM_USER+2)
#define HOOK_CMD_CLEAR 0x0001
#define HOOK_CMD_SUBMIT 0x0002
#define HOOK_CMD_SHOW 0x0003

#ifdef DLLEXAM_EXPORTS
	_declspec(dllimport)  void SetHook(HWND hd, HHOOK &hKeyBoard, HHOOK &hMouse);
	_declspec(dllimport)  void GetFocusWnd(HWND &hWnd);

#endif

