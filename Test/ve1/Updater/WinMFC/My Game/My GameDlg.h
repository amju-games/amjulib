
// My GameDlg.h : header file
//

#pragma once


// CMyGameDlg dialog
class CMyGameDlg : public CDialogEx
{
// Construction
public:
	CMyGameDlg(CWnd* pParent = NULL);	// standard constructor

	void AddText(const char*);

// Dialog Data
	enum { IDD = IDD_MYGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CString m_text;
};