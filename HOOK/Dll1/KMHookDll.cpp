// #include <windows.h>
// #include <vector>
// #include "../Common/Definition.h"
// using namespace std;
#include "pch.h"
#include "KMHookDll.h"

CKMHookDll* CKMHookDll::m_pInstance = NULL;

//BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved )
//{
//	CKMHookDll::GetInstance()->SetGlobalInstance(hinstDLL);
//	return TRUE;
//}

LRESULT CALLBACK RecHook(int code,WPARAM wParam,LPARAM lParam)
{
	int a;
	a = 2;
	static int recOK = 1;
	if(code<0)
	{
		return CallNextHookEx(CKMHookDll::GetInstance()->GetRecHook(),code,wParam,lParam);
	}
	else if(code==HC_SYSMODALON)
	{
		recOK = 0;
	}
	else if(code==HC_SYSMODALOFF)
	{
		recOK = 1;
	}
	else if(recOK && (code==HC_ACTION))
	{
		EVENTMSG action = *(EVENTMSG*)lParam;
		//此处根据条件过滤
// 		if (action.message == WM_LBUTTONDOWN || action.message == WM_LBUTTONUP)
// 		{
// 			return 0;
// 		}
		CKMHookDll::GetInstance()->AddEvent(action);
		//m_vecAction.push_back(action);
		//recordedEvent++;
	}
	return 0;
}


LRESULT CALLBACK PlayHook(int code,WPARAM wParam,LPARAM lParam)
{
	static BOOL fDelay;
	static int playOK = 1;
	int a = 1;
	a++;
	if(code<0)
	{
		return CallNextHookEx(CKMHookDll::GetInstance()->GetPlayHook(),code,wParam,lParam);
	}
	else if(code==HC_SYSMODALON)
	{
		playOK = 0;
	}
	else if(code==HC_SYSMODALOFF)
	{
		playOK = 1;
	}
	else if(playOK && (code==HC_GETNEXT))
	{
		if(fDelay)
		{
			fDelay = FALSE;
			return 50;
		}
		*((EVENTMSG*)lParam) = CKMHookDll::GetInstance()->GetVecAt(CKMHookDll::GetInstance()->GetPlayedEvent());
	}
	else if(playOK && (code==HC_SKIP))
	{
		fDelay = TRUE;
		CKMHookDll::GetInstance()->IncPlayedEvent();
	}
	if(CKMHookDll::GetInstance()->GetPlayedEvent()>=CKMHookDll::GetInstance()->GetVecSize())
	{
		UnhookWindowsHookEx(CKMHookDll::GetInstance()->GetPlayHook());
	}
	return 0;
}

CKMHookDll::CKMHookDll()
	: m_bIsKeyDown(false)
	, m_bIsMouseDown(false)
	, m_bIsMouseMove(false)
	, m_pVecAction(NULL)
	, m_hRecHook(NULL)
	, m_hPlayHook(NULL)
	, m_nPlayedEvent(0)
{
	m_pVecAction = new vector<EVENTMSG>;
}


CKMHookDll::~ CKMHookDll()
{

}

CKMHookDll* CKMHookDll::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CKMHookDll;
	}
	return m_pInstance;
}

void CKMHookDll::InstallHook(bool bIsKeyDown,bool bIsMouseDown,bool bIsMouseMove/*HWND hwnd*/)
{
	if (!m_pVecAction->empty())
	{
		m_pVecAction->clear();
	}
	m_bIsKeyDown = bIsKeyDown;
	m_bIsMouseDown = bIsMouseDown;
	m_bIsMouseMove = bIsMouseMove;
	m_hRecHook = SetWindowsHookEx(WH_JOURNALRECORD,(HOOKPROC)RecHook,m_hInst,0);
}

void CKMHookDll::UnInstallHook()
{
	UnhookWindowsHookEx(m_hRecHook);
}

void CKMHookDll::RunScript()
{
	m_nPlayedEvent = 0;
	m_hPlayHook = SetWindowsHookEx(WH_JOURNALPLAYBACK,(HOOKPROC)PlayHook,m_hInst,0);
}

void CKMHookDll::SetGlobalInstance(HINSTANCE instance)
{
	m_hInst = instance;
}

HHOOK CKMHookDll::GetRecHook()
{
	return m_hRecHook;
}

HHOOK CKMHookDll::GetPlayHook()
{
	return m_hPlayHook;
}

int CKMHookDll::GetPlayedEvent()
{
	return m_nPlayedEvent;
}

void CKMHookDll::AddEvent(EVENTMSG& action)
{
	m_pVecAction->push_back(action);
}

int CKMHookDll::GetVecSize()
{
	return m_pVecAction->size()-2;
}

void CKMHookDll::IncPlayedEvent()
{
	m_nPlayedEvent++;
}

EVENTMSG& CKMHookDll::GetVecAt(int index)
{
	return m_pVecAction->at(index);/**m_pVecAction[index];*/
}