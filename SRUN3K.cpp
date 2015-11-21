// SRUN3K.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SRUN3K.h"
#include "SRUN3KDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSRUN3KApp

BEGIN_MESSAGE_MAP(CSRUN3KApp, CWinApp)
	//{{AFX_MSG_MAP(CSRUN3KApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRUN3KApp construction

CSRUN3KApp::CSRUN3KApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSRUN3KApp object

CSRUN3KApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSRUN3KApp initialization

BOOL CSRUN3KApp::InitInstance()
{
	HANDLE m_hMutex = ::CreateMutex(NULL,FALSE, "客户端（终版）");
    if (GetLastError() == ERROR_ALREADY_EXISTS) //程序已经运行
    {
		AfxMessageBox(TEXT("程序已经运行！"),IDOK);
        HWND oldHWnd = NULL;   
        oldHWnd = ::FindWindow(NULL, _T("客户端（终版）")); //查找已经运行的程序
        if(oldHWnd)   
        {
			::ShowWindow(oldHWnd, SW_SHOWNORMAL);
			::SetForegroundWindow(oldHWnd);
        }
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
        exit(0);
    }


	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSRUN3KDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
