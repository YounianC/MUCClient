// SRUN3KDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SRUN3K.h"
#include "SRUN3KDlg.h"

#include "HttpClient.h"


#include <atlbase.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SHOWTASK (WM_USER + 200)

// 常量
#define C1 52845
#define C2 22719

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRUN3KDlg dialog

CSRUN3KDlg::CSRUN3KDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSRUN3KDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSRUN3KDlg)
	m_username = _T("");
	m_password = _T("");
	m_save = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSRUN3KDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSRUN3KDlg)
	DDX_Control(pDX, IDC_COMBO_STYLE, m_login_style);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_save);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSRUN3KDlg, CDialog)
//{{AFX_MSG_MAP(CSRUN3KDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_SIZE()
ON_BN_CLICKED(IDC_BTN_ACTION, OnBtnAction)
ON_WM_CLOSE()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BTN_DROPALL, OnBtnDropall)
ON_BN_CLICKED(IDC_BTN_USER, OnBtnUser)
ON_MESSAGE(WM_SHOWTASK,OnShowTask)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRUN3KDlg message handlers

BOOL CSRUN3KDlg::OnInitDialog()
{
	//	if (CheckUse()==false)
	//	{
	//		exit(0);
	//	}
	
	
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
	//---------------------------托盘显示---------------------------------//
    m_nid.cbSize  = (DWORD)sizeof(NOTIFYICONDATA);
    m_nid.hWnd    = this->m_hWnd;
    m_nid.uID     = IDR_MAINFRAME;
    m_nid.uFlags  = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
    m_nid.uCallbackMessage = WM_SHOWTASK;             // 自定义的消息名称
    m_nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    strcpy(m_nid.szTip, "MUC客户端");                // 信息提示条为"服务器程序"，VS2008 UNICODE编码用wcscpy_s()函数
    Shell_NotifyIcon(NIM_ADD, &m_nid);                // 在托盘区添加图标
	
	
	
	char path[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH,path);
	inipath=path;
	curPath=path;
	inipath=inipath+"\\USERINFO.ini";
	
	
	char arr[260];
	::GetPrivateProfileString("USER","UserName","学号",arr,260,inipath);
	m_username=arr;
	::GetPrivateProfileString("USER","Password","password",arr,260,inipath);
	m_password=arr;
	m_password=Decrypt(m_password,atoi(m_username));
	
	::GetPrivateProfileString("USER","Style","Win32",arr,260,inipath);
	conStyle=arr;
	
	conState=FALSE;
	m_save=TRUE;
	
	m_login_style.InsertString(0,"Win32模式");
	m_login_style.InsertString(1,"临时登陆模式");
	if (conStyle=="Win32")
		m_login_style.SetCurSel(0);
	else m_login_style.SetCurSel(1);
	
	Mac=GetMac();
	
	LOG("STARTUP");
	UpdateData(FALSE);
	
	
	sockCliBC = ::socket(AF_INET, SOCK_DGRAM, 0);	
	// 有效SO_BROADCAST选项
	BOOL bBroadcast = TRUE;
	::setsockopt(sockCliBC, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL));
	int iMode = 1; //0：阻塞
	ioctlsocket(sockCliBC,FIONBIO, (u_long FAR*) &iMode);//非阻塞设置
	
	addrSrvInfo.sin_family = AF_INET;
	addrSrvInfo.sin_addr.s_addr =inet_addr("192.168.2.250");
	addrSrvInfo.sin_port = htons(3335);
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSRUN3KDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSRUN3KDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSRUN3KDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



bool CSRUN3KDlg::Logout() 
{	
	CString mystr;
	CString tmp;
	
	LPCTSTR pstrURL="http://192.168.2.250/cgi-bin/do_logout";
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	CString tempStr="uid="+uID;
	try
	{
		CInternetSession sess;//建立会话
		CHttpFile* pF=(CHttpFile*)sess.OpenURL(pstrURL);
		BOOL result = pF->SendRequest(strHeaders,(LPVOID)(LPCTSTR)tempStr, tempStr.GetLength());
		
		while(pF->ReadString(tmp))
			mystr+=tmp;	
		pF->Close();
		sess.Close();
	}
	catch (CException *e)
	{
		return false;
	}	
	
	KillTimer(1);
	if (mystr=="logout_ok")
		LOG("LOGOUT_SUCCESSFUL");
	else
	{
		MessageBox("登出错误-"+mystr);
		LOG("LOGOUT_ERROR_"+mystr);
		return false;
	}
	return true;
}

bool CSRUN3KDlg::Login() 
{
	UpdateData(true);
	CString mystr;
	string m;
	LPCTSTR pstrURL="http://192.168.2.250/cgi-bin/do_login";
	
	if (m_login_style.GetCurSel())
	{
		CString tmp;
		CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
		CString url = pstrURL;
		CString tempStr = url+"?username="+m_username+"&password="+m_password+"&type=3";
		CInternetSession sess;//建立会话
		sess.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000);      // 3秒的连接超时
		sess.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 3000);           // 3秒的发送超时
		sess.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000);        // 3秒的接收超时
		sess.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 3000);     // 3秒的发送超时
		sess.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 3000);       // 3秒的接收超时
		sess.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);         
		CHttpFile* pF=(CHttpFile*)sess.OpenURL(tempStr);
		mystr="";
		while(pF->ReadString(tmp))
			mystr+=tmp;
		pF->Close();
		sess.Close();
	}
	else
	{
		CString tempStr = "username="+m_username+"&password=123456&drop=0&type=2&n=100&ip=1300957706";
		CHttpClient hc;
		hc.HttpPost(pstrURL,tempStr,m);
		
		mystr=m.c_str();
		LOG(mystr);
		
		int n=mystr.Find('@');
		CString time=mystr.Mid(n+1);
		
		int t=atoi(time);
		t=t/60;
		
		CString s_key,pass,ret,mac;
		
		for (int i=-4;i<3;i++)
		{
			char tString[32];
			itoa(t+i,tString,10);
			s_key=tString;
			pass =m_password;
			
			ret=encrypt(pass,s_key,TRUE);
			mac=Mac;
			mac=encrypt(mac,s_key,false);
			
			//ret=URLEncode(ret);
			ret=UrlGB2312(ret);
			mac=UrlGB2312(mac);

			tempStr="username="+m_username+"&password="+ret+"&drop=0&type=2&n=116&mac="+mac+"&ip=1300957706";
			if(hc.HttpPost(pstrURL,tempStr,m)!=0)
			{
				MessageBox("请检查网络！");
				return false;
			}
			mystr=m.c_str();
			
			
			tempStr.Format("LogIn Try i=%d:",i);
			LOG(tempStr+mystr+":"+s_key+":"+ret);
			
			if (mystr.Left(14)!="password_error")
				break;//登陆成功
		}
	}


	if (mystr.Find("error")>=0)
	{
		DealError(mystr);
		return false;
	}
	else
	{
		if (m_save)
			SaveInfo();
		LOG("LOGIN_SUCCESSFUL");
		
		if (m_login_style.GetCurSel()){
			int p=mystr.Find('`');
			uID=mystr.Left(p);
		}
		else
		{
			int n=mystr.Find(',');
			uID=mystr.Left(n);
		}
		SetTimer(1,1000*30,0);//每分钟发一次keeplive
		getinfo();
		return true;
	}	
	return false;
/*	UpdateData(true);
	
	CString mystr;
	CString tmp;
	LPCTSTR pstrURL="http://192.168.2.250/cgi-bin/do_login";
	
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	
	if (m_login_style.GetCurSel())
	{
		CString tempStr = "username="+m_username+"&password="+m_password+"&type=3";
		CInternetSession sess;//建立会话
		CHttpFile* pF=(CHttpFile*)sess.OpenURL(pstrURL);
		mystr="";
		if (pF->SendRequest(strHeaders,(LPVOID)(LPCTSTR)tempStr,tempStr.GetLength()))
			while(pF->ReadString(tmp))
				mystr+=tmp;
			pF->Close();
			sess.Close();
	}
	else
	{
		CString tempStr = "username="+m_username+"&password=123456&drop=0&type=2&n=100";
		try
		{
			////////////////////////////////////////////111111
			CInternetSession mysession;
			CHttpConnection *myconn;
			myconn=mysession.GetHttpConnection("192.168.2.250",(INTERNET_PORT)3333);
			CHttpFile *myfile;
			myfile=myconn->OpenRequest(CHttpConnection::HTTP_VERB_POST,"/cgi-bin/do_login");
			
			
			if (myfile->SendRequest(strHeaders,tempStr.GetBuffer(tempStr.GetLength()),tempStr.GetLength()))
			{	
				while(myfile->ReadString(tmp))
					mystr+=tmp;
			}
			
		
			LOG(mystr);
			int n=mystr.Find('@');
			CString time=mystr.Mid(n+1);
			
			int t=atoi(time);
			t=t/60;
			
			CString s_key,pass,ret,mac;
			for (int i=-10;i<40;i++)
			{
				char tString[32];
				itoa(t+i,tString,10);
				s_key=tString;
				pass =m_password;
				
				ret=encrypt(pass,s_key,TRUE);
				mac=Mac;
				mac=encrypt(mac,s_key,false);
				
				//ret=URLEncode(ret);
				//	ret=UrlGB2312(ret);
				//	ret.MakeLower();
				
				tempStr="username="+m_username+"&password="+ret+"&drop=0&type=2&n=116&mac="+mac+"&mbytes=0&minutes=0&ac_type=h3c";				
				mystr="";
				//////////////////////////////////////////////////////////////////////////1
				if (myfile->SendRequest(strHeaders,tempStr.GetBuffer(tempStr.GetLength()),tempStr.GetLength()))
					while(myfile->ReadString(tmp))
						mystr+=tmp;
					
					
					
					//////////////////////////////////////////////////////////////////////////2
					//	if (pF->SendRequest(strHeaders,(LPVOID)(LPCTSTR)tempStr,tempStr.GetLength()))
					//		while(pF->ReadString(tmp))
					//			mystr+=tmp;
					
					tempStr.Format("LogIn Try i=%d:",i);
					LOG(tempStr+mystr+":"+s_key+":"+ret);
					
					if (mystr.Left(14)!="password_error")
						break;//登陆成功
			}
			//		pF->Close();
			//		sess.Close();
		}
		catch (CInternetException *e)
		{
			MessageBox("请检查网络！");
			return false;
		}
	}
	
	if (mystr.Right(5)=="error"||mystr.Left(14)=="password_error")
	{
		DealError(mystr);
		return false;
	}
	else
	{
		if (m_save)
			SaveInfo();
		LOG("LOGIN_SUCCESSFUL");
		
		int n=mystr.Find(',');
		if (m_login_style.GetCurSel())
			uID=mystr;
		else
		{
			uID=mystr.Left(n);
			
		}
		SetTimer(1,1000*30,0);//每分钟发一次keeplive
		getinfo();
	}
	return true;*/
}

int CSRUN3KDlg::_get_keychar(CString key, int num)
{
	int len = key.GetLength();
	return key[len-num%len-1];
}

CString CSRUN3KDlg::_build_key(int num, int reverse)
{
	CString ret = "";
	int _low = num & 0x0f;
	
	int _high = num >> 4;
	_high = _high & 0x0f;
	
	if (reverse == 0)
	{
		char temp1 = (char)(_low + 0x36);
		char temp2 = (char)(_high + 0x63);
		ret+=temp1;
		ret+=temp2;
	}
	else
	{
		char temp1 = (char)(_high + 0x63);
		char temp2 = (char)(_low + 0x36);
		ret+=temp1;
		ret+=temp2;
	}
	return ret;
}


CString CSRUN3KDlg::encrypt(CString password, CString key, bool checkLength)
{
	if (checkLength)
	{
		password = password.Left(16);
	}
	int pw_len = password.GetLength();
	CString ret;
	
	for (int i = 0; i < pw_len; i++)
	{
		int _pass=(DWORD)password.GetAt(i);
		int _key = _get_keychar(key, i);
		_key = _key ^ _pass;
		ret+=_build_key(_key, i % 2);
	}
	
	return ret;
}


void CSRUN3KDlg::SaveInfo()
{
	::WritePrivateProfileString("USER","UserName",m_username,inipath);
	CString fake_pa=Encrypt(m_password,atoi(m_username));
	::WritePrivateProfileString("USER","Password",fake_pa,inipath);
	if (m_login_style.GetCurSel())
		::WritePrivateProfileString("USER","Style","TEMP",inipath);
	else ::WritePrivateProfileString("USER","Style","Win32",inipath);
}

void CSRUN3KDlg::LOG(CString cs)
{
/*	CString str;//获取系统时间
	CTime tm=CTime::GetCurrentTime();
	str=tm.Format("%Y-%m-%d-%X:	");
	cs=str+=cs;
	cs+="\r\n";
	CFile file;
	file.Open("LOG.txt",CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate);
	file.SeekToEnd();
	file.Write(cs.GetBuffer(cs.GetLength()),cs.GetLength());
*/
}

LRESULT CSRUN3KDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:                // 双击左键的处理
		{
			this->ShowWindow(SW_SHOWNORMAL);
			this->SetForegroundWindow();
		}
		break;
	}
	return 0;
}

void CSRUN3KDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)  
	{  
		ShowWindow(SW_HIDE); // 当最小化市，隐藏主窗口              
	}
}

void CSRUN3KDlg::OnBtnAction() 
{
	if (conState==FALSE)
	{
		if (Login())
		{
			conState=true;
			GetDlgItem(IDC_BTN_ACTION)->SetWindowText("断开");
			GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(FALSE);
		}
	}
	else
	{
		if (!Logout())
			MessageBox("登出出错！");
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText("已连接：0(分钟)\n剩余时长：0(分钟)\n余额约：0(元)");
		conState=false;
		GetDlgItem(IDC_BTN_ACTION)->SetWindowText("连接");
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
	}
}

void CSRUN3KDlg::OnClose() 
{
	if (conState==TRUE)
	{
		if (IDOK==MessageBox("退出将切断网络！","警告",MB_OKCANCEL|MB_ICONWARNING))
		{
			Logout();	
			CDialog::OnClose();
			
			// 在托盘区删除图标
			Shell_NotifyIcon(NIM_DELETE, &m_nid);   
			
			LOG("DROP_OUT");
			//	SetFileAttributes(curPath+"\\LOG.txt",FILE_ATTRIBUTE_HIDDEN);
			SetFileAttributes(inipath,FILE_ATTRIBUTE_HIDDEN);		
		}
	}
	else
	{
		CDialog::OnClose();		
		// 在托盘区删除图标
		Shell_NotifyIcon(NIM_DELETE, &m_nid);   
		
		LOG("DROP_OUT");
		//SetFileAttributes(curPath+"LOG.txt",FILE_ATTRIBUTE_HIDDEN);
		SetFileAttributes(inipath,FILE_ATTRIBUTE_HIDDEN);
		
	}
}

BOOL CSRUN3KDlg::CheckUse()
{
	CString version="TIME_BETA";
	// 打开HKEY_LOCAL_MACHINE主键下的SoftWare\\Knight Studio\\Knight子键
	HKEY hKey;   
	//找到系统的启动项   
	LPCTSTR lpRun = "SoftWare\\SRUN3K_Y\\Used\\TIME";   
	
	LONG bOpenReg =RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE|KEY_READ, &hKey); 
	if(bOpenReg!=ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行
	{
		DWORD dw;
        bOpenReg=RegCreateKeyEx(HKEY_LOCAL_MACHINE,lpRun,NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);
		
		bOpenReg= RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
		if(bOpenReg == ERROR_SUCCESS)   
		{
			SYSTEMTIME st;
			CString strDate,strTime;
			GetLocalTime(&st);
			strDate.Format("%4d-%2d-%2d-%2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
			
			bOpenReg=RegSetValueEx(hKey,version,0L,REG_SZ,(CONST BYTE*)strDate.GetBuffer(strDate.GetLength()),strDate.GetLength()+1);
			RegCloseKey(hKey);
		}
	}
	else
	{
		DWORD dwType = REG_SZ; 
		DWORD dwSize = 512;
		char string[512]; 
		bOpenReg=RegQueryValueEx(hKey,version,NULL,&dwType,(LPBYTE)string,&dwSize);
		if (bOpenReg==ERROR_SUCCESS)	
		{	
			CString t=string;
			SYSTEMTIME stReg,stReal;
			stReg.wYear=atoi(t.Left(4));
			stReg.wMonth=atoi(t.Mid(5,2));
			stReg.wDay=atoi(t.Mid(8,2));
			stReg.wHour=atoi(t.Mid(11,2));
			stReg.wMinute=atoi(t.Mid(14,2));
			stReg.wSecond=atoi(t.Mid(17,2));
			
			GetLocalTime(&stReal);
			
			CTime tReal,tReg;
			tReal = CTime((int)stReal.wYear, (int)stReal.wMonth, (int)stReal.wDay,(int)stReal.wHour, (int)stReal.wMinute,(int)stReal.wSecond,-1);
			tReg = CTime((int)stReg.wYear, (int)stReg.wMonth, (int)stReg.wDay,(int)stReg.wHour, (int)stReg.wMinute,(int)stReg.wSecond,-1);
			
			
			CTimeSpan ts=tReal-tReg;
			UINT nTotalDays=ts.GetTotalHours()/24;
			
			if (nTotalDays>25&&nTotalDays<30)
			{
				CString t2;
				t2.Format("还可以用%d天！",30-nTotalDays);
				MessageBox(t2);
			}
			
			if (nTotalDays>30)
			{
				MessageBox("使用时间过了30天啊，不能再用了。。。");
				return false;
			}
			
		}
	}
	return true;
}

void CSRUN3KDlg::OnTimer(UINT nIDEvent) 
{
	if (m_login_style.GetCurSel())//1
		getinfo();
	else///0
	{
		keeplive();
		getinfo();
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CSRUN3KDlg::getinfo()
{/*
	CString mystr;
	CString tmp;
	LPCTSTR pstrURL="http://192.168.2.250/cgi-bin/keeplive";
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	
	  CString tempStr="uid";
	  CInternetSession sess;//建立会话
	  try
	  {
	  CHttpFile* pF=(CHttpFile*)sess.OpenURL(pstrURL);
	  BOOL result =pF->SendRequest(strHeaders,(LPVOID)(LPCTSTR)tempStr, tempStr.GetLength());
	  while(pF->ReadString(tmp))
	  mystr+=tmp;
	  
		pF->Close();
		sess.Close();
		}catch(CInternetException * m_pException){
		m_pException->m_dwError;
		m_pException->Delete();
		sess.Close();
		return false;
		}
	*/
	CString mystr;
	CString tmp;
	try
	{
		CInternetSession mysession;
		CHttpConnection *myconn;
		myconn=mysession.GetHttpConnection("192.168.2.250");
		CHttpFile *myfile;
		myfile=myconn->OpenRequest(CHttpConnection::HTTP_VERB_POST,
			"/cgi-bin/keeplive");
		
		CString strHeaders,tempStr;  
		strHeaders=_T("Content-Type:application/x-www-form-urlencoded"); 
		tempStr="username="+m_username+"&password="+m_password;		
		
		if (myfile->SendRequest(strHeaders,tempStr.GetBuffer(tempStr.GetLength()),tempStr.GetLength()))
		{	
			while(myfile->ReadString(tmp))
				mystr+=tmp;
		}
	}
	catch(CException *e)
	{
	}
	
	int n=mystr.Find(',');
	CString time_connected=mystr.Left(n);
	
	n=mystr.ReverseFind(',');
	int m=0;
	for (int i=0;i<6;i++)
	{
		m=mystr.Find(',',m+1);
	}
	
	CString time_left=mystr.Mid(m+1,n-m-1);
	
	int t=atoi(time_left)/60;//分钟
	time_left.Format("%d",t);
	
	int t2=atoi(time_connected)/60;
	time_connected.Format("%d",t2);
	time_connected="已连接："+time_connected;
	time_connected+="(分钟)";
	time_connected+="\n剩余时长：";
	time_connected+=time_left;
	time_connected+="(分钟)";
	
	t/=60;//小时
	t*=0.3;
	time_left.Format("\n余额约：%d(元)",t);
	time_connected+=time_left;
	
	LOG("getinfo:"+mystr);
	if (mystr=="")
	{
		KillTimer(1);
		
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText("已连接：0(分钟)\n剩余时长：0(分钟)\n余额约：0(元)");
		conState=false;
		GetDlgItem(IDC_BTN_ACTION)->SetWindowText("连接");
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
		
		MessageBox("请检查网络!");
		return false;
	}
	if(mystr=="error")
	{
		KillTimer(1);
		
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText("已连接：0(分钟)\n剩余时长：0(分钟)\n余额约：0(元)");
		conState=false;
		GetDlgItem(IDC_BTN_ACTION)->SetWindowText("连接");
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
		
		
		MessageBox("您已经被强制下线!");
		return false;
	}
	
	
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(time_connected);		
	UpdateData(FALSE);
	return true;
}

void CSRUN3KDlg::DealError(CString cs)
{
	LOG(cs);
	if (cs.Find("username_error")>=0)
	{
		MessageBox("学号错误！");
	}
	else if (cs.Find("password_error")>=0)
	{
		MessageBox("密码错误！");
	}
	else if (cs.Find("ip_exist_error")>=0)
	{
		MessageBox("IP已存在！");
	}
	else if (cs=="status_error")
	{
		MessageBox("用户已欠费！");
	}
	else if (cs=="usernum_error")
	{
		MessageBox("人数已达上限，请强制下线后再登陆！");
	}
	else MessageBox(cs);
}

void CSRUN3KDlg::OnBtnDropall() 
{
	CString mystr;
	CString tmp;
	LPCTSTR pstrURL="http://192.168.2.250/cgi-bin/force_logout";
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	CString tempStr="username="+m_username+"&password="+m_password+"&drop=0&type=1&n=1";
	try
	{
		CInternetSession sess;
		CHttpFile* pF=(CHttpFile*)sess.OpenURL(pstrURL);
		BOOL result = pF->SendRequest(strHeaders,(LPVOID)(LPCTSTR)tempStr, tempStr.GetLength());
		
		while(pF->ReadString(tmp))
			mystr+=tmp;	
		sess.Close();
	}
	catch (CException *e)
	{
	}	
	
	KillTimer(1);
	if (mystr=="logout_ok")
	{
		MessageBox("登出成功！");
		LOG("DROPALL_SUCCESSFUL");
		
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText("已连接：0(分钟)\n剩余时长：0(分钟)\n余额约：0(元)");
		conState=false;
		GetDlgItem(IDC_BTN_ACTION)->SetWindowText("连接");
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
	}
	else
	{
		MessageBox("无需登出！");
		LOG("FORCE_LOGOUT_ERROR_"+mystr);
	}
}

void CSRUN3KDlg::OnBtnUser() 
{
	CString tempStr="http://192.168.2.250:8800/services1.php?checkid=9538&user_login_name="+m_username+"&user_password="+m_password;
	ShellExecute(NULL, "open",tempStr,NULL, NULL, SW_SHOWNORMAL); 
}

void CSRUN3KDlg::keeplive() 
{
	char *data=new char(sizeof(100));
	GetuIDCode(data);
	CString msg=data;
	
	int n=::sendto(sockCliBC,data,8,0,(sockaddr*)&addrSrvInfo,sizeof(addrSrvInfo));
}

CString CSRUN3KDlg::GetMac() 
{
	char *mac;
	ASTAT Adapter;
    NCB Ncb;
    UCHAR uRetCode;
    LANA_ENUM lenum;
	
    memset(&Ncb, 0, sizeof(Ncb));
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
	
    uRetCode = Netbios( &Ncb );
	
	CString t;
	t.Format("The NCBENUM return adapter number is: %d \n ", lenum.length);
	
    for(int i=0;i<lenum.length;i++)
    {
        memset(&Ncb, 0, sizeof(Ncb));
        Ncb.ncb_command = NCBRESET;
        Ncb.ncb_lana_num = lenum.lana[i];
        uRetCode = Netbios( &Ncb );
		
        memset(&Ncb, 0, sizeof(Ncb));
        Ncb.ncb_command = NCBASTAT;
        Ncb.ncb_lana_num = lenum.lana[i];
        strcpy((char *)Ncb.ncb_callname, "* ");
        Ncb.ncb_buffer = (unsigned char *) &Adapter;
        Ncb.ncb_length = sizeof(Adapter);
        uRetCode = Netbios( &Ncb );
		
        if (uRetCode == 0)
        {
            //sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x ",
            sprintf(mac,"%02X:%02X:%02X:%02X:%02X:%02X ",
				Adapter.adapt.adapter_address[0],
				Adapter.adapt.adapter_address[1],
				Adapter.adapt.adapter_address[2],
				Adapter.adapt.adapter_address[3],
				Adapter.adapt.adapter_address[4],
				Adapter.adapt.adapter_address[5]
				);
            //printf( "The Ethernet Number on LANA %d is: %s\n ", lenum.lana[i], mac);
        }
    }
	t=mac;
	return t;
}

void CSRUN3KDlg::GetuIDCode(char *data)
{
	__int64 num=_atoi64(uID.GetBuffer(uID.GetLength())); 
	CString str_num="";  
	CString s="";
    while(num>=16)
    {
        __int64 a=num/16;  
        __int64 mod_a = num % 16;  
        switch(mod_a)  
        {  
        case 10:  
            s = "A";  
            break;  
        case 11:  
            s = "B";  
            break;  
        case 12:  
            s = "C";  
            break;  
        case 13:  
            s = "D";  
            break;  
        case 14:  
            s = "E";  
            break;  
        case 15:  
            s = "F";  
            break;  
        default:  
            s.Format("%c",mod_a+48);
            break;  
        }
        str_num = s + str_num;  
        num = a;
    }  
    if(num%16!= 0)  
    {  
        int last_a=num%16;
		if (last_a<10)
			s.Format("%c",last_a+48);
		else s.Format("%c",last_a+55);
		
        str_num=s+str_num;  
    }  
	
	CString res;
	
	int t;
	s=str_num.Mid(str_num.GetLength()-2,2);
	t=strtol(s.GetBuffer(s.GetLength()),NULL,16);
	data[0]=t;
	s=str_num.Mid(str_num.GetLength()-4,2);
	t=strtol(s.GetBuffer(s.GetLength()),NULL,16);
	data[1]=t;
	s=str_num.Mid(str_num.GetLength()-6,2);
	t=strtol(s.GetBuffer(s.GetLength()),NULL,16);
	data[2]=t;
	s=str_num.Mid(str_num.GetLength()-8,2);
	t=strtol(s.GetBuffer(s.GetLength()),NULL,16);
	data[3]=t;
	s=str_num.Mid(str_num.GetLength()-10,2);
	t=strtol(s.GetBuffer(s.GetLength()),NULL,16);
	data[4]=t;
	s=str_num.Mid(str_num.GetLength()-12,2);
	t=strtol(s.GetBuffer(s.GetLength()),NULL,16);
	data[5]=t;
	s.Format("00");
	data[6]=0;
	s.Format("00");
	data[7]=0;
}

CString CSRUN3KDlg::ciphering(CString pa,CString key,bool en)
{
	int pw_len = pa.GetLength();
	CString ret;
	int len=key.GetLength();
	CString t;
	for (int i = 0; i < pw_len; i++)
	{
		int _pass=(DWORD)pa.GetAt(i);
		int _key =key.GetAt(len-i%len-1);
		
		if (en)
			_key = (_key-_pass);
		else
			_key = (_key+_pass);
		ret+=_key;
	}
	return ret;
}

CString CSRUN3KDlg::Encrypt(CString S, WORD Key) // 加密函数
{
	CString Result,str;
	int i,j;
	
	Result=S; // 初始化结果字符串
	for(i=0; i<S.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, S.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	S=Result; // 保存结果
	Result.Empty(); // 清除结果
	for(i=0; i<S.GetLength(); i++) // 对加密结果进行转换
	{
		j=(BYTE)S.GetAt(i); // 提取字符
		// 将字符转换为两个字母保存
		str="12"; // 设置str长度为2
		str.SetAt(0, 65+j/26);//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		str.SetAt(1, 65+j%26);
		Result += str;
	}
	return Result;
}

// 解密函数

CString CSRUN3KDlg::Decrypt(CString S,WORD Key) 
{
	CString Result,str;
	int i,j;
	
	Result.Empty(); // 清除结果
	for(i=0; i < S.GetLength()/2; i++) // 将字符串两个字母一组进行处理
	{
		j = ((BYTE)S.GetAt(2*i)-65)*26;//相应的，解密处要改为相同的数
		
		j += (BYTE)S.GetAt(2*i+1)-65;
		str="1"; // 设置str长度为1
		str.SetAt(0, j);
		Result+=str; // 追加字符，还原字符串
	}
	S=Result; // 保存中间结果
	for(i=0; i<S.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, (BYTE)S.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)S.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	return Result;
}

BOOL CSRUN3KDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_SETTING)
		MessageBox("12");
	return CDialog::PreTranslateMessage(pMsg);
}



CString CSRUN3KDlg::UrlGB2312(CString str)
{
	char * s=str.GetBuffer(str.GetLength());
	CString dd;  
    size_t len = strlen(str);  
    for (size_t i=0;i<len;i++)  
    {  
        if(isalnum((BYTE)s[i]))  
        {  
            char tempbuff[2];  
            sprintf(tempbuff,"%c",s[i]);  
            dd+=tempbuff;  
        }  
        else if (isspace((BYTE)s[i]))  
        {  
            dd+="+";  
        }  
        else  
        {  
            char tempbuff[4];  
            sprintf(tempbuff,"%%%X%X",((BYTE*)s)[i] >>4,((BYTE*)s)[i] %16);  
            dd+=tempbuff;  
        }  
		
    }  
    return dd;  
}

unsigned char CSRUN3KDlg::ToHex(unsigned char x)   
{   
    return  x > 9 ? x + 55 : x + 48;   
}

CString CSRUN3KDlg::URLEncode(CString str)
{
	CString strTemp = "";  
    int length = str.GetLength();  
    for (int i = 0; i < length; i++)  
    {  
        if (isalnum((unsigned char)str[i]) ||   
            (str[i] == '-') ||  
            (str[i] == '_') ||   
            (str[i] == '.') ||   
            (str[i] == '~'))  
            strTemp += str[i];  
        else if (str[i] == ' ')  
            strTemp += "+";  
        else  
        {  
            strTemp += '%';  
            strTemp += ToHex((unsigned char)str[i] >> 4);  
            strTemp += ToHex((unsigned char)str[i] % 16);  
        }  
    }  
    return strTemp; 
}
