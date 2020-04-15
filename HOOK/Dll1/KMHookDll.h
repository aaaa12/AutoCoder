#ifndef __KMHOOKDLL_H__
#define __KMHOOKDLL_H__

#include <windows.h>
#include <vector>
using namespace std;

#ifdef KMHOOKDLL_EXPORTS
#define KMHOOKDLL_API __declspec(dllexport)
#else
#define KMHOOKDLL_API __declspec(dllimport)
#endif

class KMHOOKDLL_API CKMHookDll
{
public:
	static CKMHookDll* GetInstance();

	void InstallHook(bool bIsKeyDown,bool bIsMouseDown,bool bIsMouseMove/*HWND hwnd*/);

	void UnInstallHook();

	void RunScript();

	void SetGlobalInstance(HINSTANCE instance);

	HHOOK GetRecHook();
	HHOOK GetPlayHook();
	int GetPlayedEvent();
	void IncPlayedEvent();
	void AddEvent(EVENTMSG& action);
	int GetVecSize();
	EVENTMSG& GetVecAt(int index);
private:
	 CKMHookDll();
	~ CKMHookDll();

private:
	static CKMHookDll* m_pInstance;
	bool m_bIsKeyDown;
	bool m_bIsMouseDown;
	bool m_bIsMouseMove;
	vector<EVENTMSG> *m_pVecAction;
	HHOOK m_hRecHook;
	HHOOK m_hPlayHook;
	int m_nPlayedEvent;
	HINSTANCE m_hInst;
};

#endif