// QglDemo.h : QglDemo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CQglDemoApp:
// �йش����ʵ�֣������ QglDemo.cpp
//

class CQglDemoApp : public CWinApp
{
public:
	CQglDemoApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CQglDemoApp theApp;
