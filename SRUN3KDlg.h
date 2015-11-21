// SRUN3KDlg.h : header file
//

#if !defined(AFX_SRUN3KDLG_H__4AB4D18C_9DD2_44EA_B815_3D58B02AF5CF__INCLUDED_)
#define AFX_SRUN3KDLG_H__4AB4D18C_9DD2_44EA_B815_3D58B02AF5CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include   "winsock.h"  
#include   <nb30.h>  
typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;


/////////////////////////////////////////////////////////////////////////////
// CSRUN3KDlg dialog

class CSRUN3KDlg : public CDialog
{
// Construction
public:
	unsigned char ToHex(unsigned char x);
	CString URLEncode(CString str);
	CString UrlGB2312(CString str);
	void GetuIDCode(char *data);
	void keeplive();
	BOOL getinfo();
	BOOL CheckUse();
	void LOG(CString cs);
	void SaveInfo();

	bool Login();
	bool Logout();

	void DealError(CString cs);
	CString GetMac();

	CString encrypt(CString password,CString key, bool checkLength);
	int _get_keychar(CString key, int num);
	CString _build_key(int num, int reverse);

	CString ciphering(CString pa,CString key,bool en);
	CString Encrypt(CString S, WORD Key); // ¼ÓÃÜº¯Êý
	CString Decrypt(CString S, WORD Key);


	
	CSRUN3KDlg(CWnd* pParent = NULL);	// standard constructor

	CString Mac;
	CString uID;
	CString inipath;
	CString curPath;
	BOOL conState;

	CString conStyle;

	NOTIFYICONDATA m_nid;
	LRESULT CSRUN3KDlg::OnShowTask(WPARAM wParam, LPARAM lParam);
	

	SOCKET sockCliBC;
	SOCKADDR_IN addrSrvInfo;
// Dialog Data
	//{{AFX_DATA(CSRUN3KDlg)
	enum { IDD = IDD_SRUN3K_DIALOG };
	CComboBox	m_login_style;
	CString	m_username;
	CString	m_password;
	BOOL	m_save;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRUN3KDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSRUN3KDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAction();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnDropall();
	afx_msg void OnBtnUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRUN3KDLG_H__4AB4D18C_9DD2_44EA_B815_3D58B02AF5CF__INCLUDED_)
