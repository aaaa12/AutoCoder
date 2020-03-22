// MsgDlg.cpp: 实现文件
//

#include "pch.h"
#include "GrobHook.h"
#include "MsgDlg.h"
#include "afxdialogex.h"


// CMsgDlg 对话框

IMPLEMENT_DYNAMIC(CMsgDlg, CDialogEx)

CMsgDlg::CMsgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSG, pParent)
{

}

CMsgDlg::~CMsgDlg()
{
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialogEx)
END_MESSAGE_MAP()


// CMsgDlg 消息处理程序

void CMsgDlg::SetTxt(CString str,bool bShow)
{

	m_edit.SetWindowText(str);
	CDC* pDC = m_edit.GetDC();

	CSize size = pDC->GetTextExtent(str);
	m_edit.ReleaseDC(pDC);
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right = rc.left + size.cx;
	if (rc.right < 20)
		rc.right = 20;
	if(rc.right > 220)
		rc.right = rc.right*9/10;
	
	MoveWindow(rc);
	m_edit.MoveWindow(rc);
	GetWindowRect(rc);
	if(bShow)
		::SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	else
		::SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
}