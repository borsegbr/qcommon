// QcdDemo.h : main header file for the QcdDemo application
//

#if !defined(AFX_QcdDemo_H__D3B4CE36_9706_45CF_844A_C5C3CC5EABEB__INCLUDED_)
#define AFX_QcdDemo_H__D3B4CE36_9706_45CF_844A_C5C3CC5EABEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "src/QcdWorld.h"

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoApp:
// See QcdDemo.cpp for the implementation of this class
//

class CQcdDemoApp : public CWinApp
{
public:
	void GetViewReqDim(int &cx, int &cy);
	void SetView(CView *pView);

	void Go();
	void Stop();

	void ManMove(const UserCmd::Cmd &cmd);
	void DrawFrame(CDC *pDC);
	bool Started();

protected:
	QcdWorld	m_QcdWorld;
	CView		*m_pView;
	enum
	{
		k_Ready,
		k_Running,
		k_Paused
	} m_eGameStat;

    QcdWorld::UiMode    m_UiMode;

public:
	CQcdDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQcdDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CQcdDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    afx_msg void OnAppGameStart();
    afx_msg void OnAppGameStop();
    afx_msg void OnUpdateAppGameLoad(CCmdUI *pCmdUI);
    afx_msg void OnUpdateAppGameSave(CCmdUI *pCmdUI);
    afx_msg void OnAppGameLoad();
    afx_msg void OnAppGameSave();
};

extern CQcdDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QcdDemo_H__D3B4CE36_9706_45CF_844A_C5C3CC5EABEB__INCLUDED_)
