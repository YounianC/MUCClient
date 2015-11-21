// SRUN3K.h : main header file for the SRUN3K application
//

#if !defined(AFX_SRUN3K_H__595DED50_E61B_47F2_8E1A_C366FB615ECB__INCLUDED_)
#define AFX_SRUN3K_H__595DED50_E61B_47F2_8E1A_C366FB615ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CSRUN3KApp:
// See SRUN3K.cpp for the implementation of this class
//

class CSRUN3KApp : public CWinApp
{
public:
	CSRUN3KApp();

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRUN3KApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSRUN3KApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRUN3K_H__595DED50_E61B_47F2_8E1A_C366FB615ECB__INCLUDED_)
