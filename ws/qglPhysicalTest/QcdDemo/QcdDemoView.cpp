// QcdDemoView.cpp : implementation of the CQcdDemoView class
//

#include "stdafx.h"
#include "QcdDemo.h"

#include "QcdDemoDoc.h"
#include "QcdDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoView

IMPLEMENT_DYNCREATE(CQcdDemoView, CView)

BEGIN_MESSAGE_MAP(CQcdDemoView, CView)
	//{{AFX_MSG_MAP(CQcdDemoView)
	ON_WM_KEYDOWN()
    ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoView construction/destruction

CQcdDemoView::CQcdDemoView()
{
	// TODO: add construction code here
    m_ArrowPress = 0;
}

CQcdDemoView::~CQcdDemoView()
{
}

BOOL CQcdDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	theApp.SetView(this);

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoView drawing

void CQcdDemoView::OnDraw(CDC* pDC)
{
	CQcdDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if (theApp.Started())
		theApp.DrawFrame(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoView printing

BOOL CQcdDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CQcdDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CQcdDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoView diagnostics

#ifdef _DEBUG
void CQcdDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CQcdDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CQcdDemoDoc* CQcdDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CQcdDemoDoc)));
	return (CQcdDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQcdDemoView message handlers

#define ARROW_UP        0x1
#define ARROW_DOWN      0x2
#define ARROW_LEFT      0x4
#define ARROW_RIGHT     0x8


void CQcdDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int mx = 0, my = 0;

	switch (nChar)
	{
	case 38:
        m_ArrowPress |= ARROW_UP;
		break;
	case 40:
        m_ArrowPress |= ARROW_DOWN;
		break;
	case 37:
        m_ArrowPress |= ARROW_LEFT;
		break;
	case 39:
        m_ArrowPress |= ARROW_RIGHT;
		break;
	}

    if ((m_ArrowPress & ARROW_UP) && !(m_ArrowPress & ARROW_DOWN))
    {
        my = -1;
    }
    else if (!(m_ArrowPress & ARROW_UP) && (m_ArrowPress & ARROW_DOWN))
    {
        my = 1;
    }
    if ((m_ArrowPress & ARROW_LEFT) && !(m_ArrowPress & ARROW_RIGHT))
    {
        mx = -1;
    }
    else if (!(m_ArrowPress & ARROW_LEFT) && (m_ArrowPress & ARROW_RIGHT))
    {
        mx = 1;
    }

    if (!(mx==0 && my==0))
    {
        theApp.ManMove(UserCmd::Cmd(m_ArrowPress));
    }

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CQcdDemoView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    // TODO: Add your message handler code here and/or call default

    switch (nChar)
    {
    case 38:
        m_ArrowPress &= ~ARROW_UP;
        break;
    case 40:
        m_ArrowPress &= ~ARROW_DOWN;
        break;
    case 37:
        m_ArrowPress &= ~ARROW_LEFT;
        break;
    case 39:
        m_ArrowPress &= ~ARROW_RIGHT;
        break;
    }

    CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

