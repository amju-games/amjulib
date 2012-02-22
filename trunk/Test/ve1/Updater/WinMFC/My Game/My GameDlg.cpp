#include "stdafx.h"
#include "My Game.h"
#include "My GameDlg.h"
#include "afxdialogex.h"
#include "Updater.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

using namespace Amju;

CMyGameDlg::CMyGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyGameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_text = _T("");  // TODO Localisation
}

void CMyGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_text);
}

BEGIN_MESSAGE_MAP(CMyGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

namespace Amju
{
  // Silence file not found error
  void ReportError(const std::string& error)
  {
  }
}

// CMyGameDlg message handlers
static CMyGameDlg* theDlg = 0;
bool more = false;

void AddText(const char* s)
{
	theDlg->AddText(s);
}

void CMyGameDlg::AddText(const char* s)
{
	Invalidate();

	more = true;
	m_text += s;
  m_text += "\r\n";
}

BOOL CMyGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    // Start update thread
//	theDlg = this;
//	Updater* u = new Updater(::AddText);
//	u->Start();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		if (more)
		{
			UpdateData(FALSE);
			more = false;
		}

		static bool first = true;
		if (first)
		{
			first = false;

			theDlg = this;
			Updater* u = new Updater(::AddText);
			u->Start();
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


