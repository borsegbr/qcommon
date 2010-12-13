// QcdDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "QcdDemo.h"

#include "MainFrm.h"
#include "QcdDemoDoc.h"
#include "QcdDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoApp

BEGIN_MESSAGE_MAP(CQcdDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CQcdDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
    ON_COMMAND(ID_APP_GAME_START, &CQcdDemoApp::OnAppGameStart)
    ON_COMMAND(ID_APP_GAME_STOP, &CQcdDemoApp::OnAppGameStop)
    ON_UPDATE_COMMAND_UI(ID_APP_GAME_LOAD, &CQcdDemoApp::OnUpdateAppGameLoad)
    ON_UPDATE_COMMAND_UI(ID_APP_GAME_SAVE, &CQcdDemoApp::OnUpdateAppGameSave)
    ON_COMMAND(ID_APP_GAME_LOAD, &CQcdDemoApp::OnAppGameLoad)
    ON_COMMAND(ID_APP_GAME_SAVE, &CQcdDemoApp::OnAppGameSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoApp construction

CQcdDemoApp::CQcdDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pView = 0;
	m_eGameStat = k_Ready;
    m_UiMode = QcdWorld::k_UiNone;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQcdDemoApp object

CQcdDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoApp initialization


BOOL CQcdDemoApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if !defined(VCDOTNET)
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CQcdDemoDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CQcdDemoView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


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
		// No message handlers
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

// App command to run the dialog
void CQcdDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CQcdDemoApp::GetViewReqDim(int &cx, int &cy)
{
	cx = 400;
	cy = 300;
}

void CQcdDemoApp::SetView(CView *pView)
{
	m_pView = pView;
}

void CQcdDemoApp::Go()
{
	if (m_eGameStat == k_Ready)
	{
		m_QcdWorld.Init(m_UiMode);
		m_QcdWorld.SetScreen(m_pView, 400, 300);
		m_QcdWorld.Start();
	}
	else if (m_eGameStat==k_Paused)
	{
		m_QcdWorld.ResumeThread();
	}
	else return;
	m_eGameStat = k_Running;
	
}

void CQcdDemoApp::Stop()
{
	if (m_eGameStat != k_Running) return;
	m_QcdWorld.SuspendThread();
	m_eGameStat = k_Paused;
}


void CQcdDemoApp::ManMove(const UserCmd::Cmd &cmd)
{
	m_QcdWorld.PushUserInput(UserCmd(cmd));
}

void CQcdDemoApp::DrawFrame(CDC *pDC)
{
	m_QcdWorld.DrawFrame(pDC);
}

bool CQcdDemoApp::Started()
{
	return m_eGameStat==k_Running||m_eGameStat==k_Paused;
}


/////////////////////////////////////////////////////////////////////////////
// CQcdDemoApp message handlers



void CQcdDemoApp::OnAppGameStart()
{
    // TODO: �ڴ���������������
    Go();
}

void CQcdDemoApp::OnAppGameStop()
{
    // TODO: �ڴ���������������
    Stop();
}

void CQcdDemoApp::OnUpdateAppGameLoad(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if (m_UiMode == QcdWorld::k_UiIn)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);

}

void CQcdDemoApp::OnUpdateAppGameSave(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if (m_UiMode == QcdWorld::k_UiOut)
        pCmdUI->SetCheck(1);
    else
        pCmdUI->SetCheck(0);
}

void CQcdDemoApp::OnAppGameLoad()
{
    // TODO: �ڴ���������������
    if (m_UiMode == QcdWorld::k_UiIn)
    {
        m_UiMode = QcdWorld::k_UiNone;
        m_QcdWorld.SetUiMode(m_UiMode);
    }
    else
    {
        m_UiMode = QcdWorld::k_UiIn;
        m_QcdWorld.SetUiMode(m_UiMode);
    }
}

void CQcdDemoApp::OnAppGameSave()
{
    // TODO: �ڴ���������������
    if (m_UiMode == QcdWorld::k_UiOut)
    {
        m_UiMode = QcdWorld::k_UiNone;
        m_QcdWorld.SetUiMode(m_UiMode);
    }
    else
    {
        m_UiMode = QcdWorld::k_UiOut;
        m_QcdWorld.SetUiMode(m_UiMode);
    }
}
