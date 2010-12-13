// QcdDemoDoc.cpp : implementation of the CQcdDemoDoc class
//

#include "stdafx.h"
#include "QcdDemo.h"

#include "QcdDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoDoc

IMPLEMENT_DYNCREATE(CQcdDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CQcdDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CQcdDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoDoc construction/destruction

CQcdDemoDoc::CQcdDemoDoc()
{
	// TODO: add one-time construction code here

}

CQcdDemoDoc::~CQcdDemoDoc()
{
}

BOOL CQcdDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CQcdDemoDoc serialization

void CQcdDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoDoc diagnostics

#ifdef _DEBUG
void CQcdDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CQcdDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoDoc commands
