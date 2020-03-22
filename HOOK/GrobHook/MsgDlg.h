#pragma once


// CMsgDlg 对话框

class CMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgDlg)

public:
	CMsgDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMsgDlg();
	void SetTxt(CString str, bool bShow);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
};
