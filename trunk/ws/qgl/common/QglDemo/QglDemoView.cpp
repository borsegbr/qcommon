// QglDemoView.cpp : CQglDemoView 类的实现
//

#include "stdafx.h"
#include "QglDemo.h"

#include "QglDemoDoc.h"
#include "QglDemoView.h"
#include ".\qgldemoview.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MAX_DISP_WIDTH		1280
#define MAX_DISP_HEIGHT		800
#define BMP_BPP				4
#define MAX_DISP_BUF_SIZE	(MAX_DISP_WIDTH*MAX_DISP_HEIGHT*BMP_BPP)


// CQglDemoView

IMPLEMENT_DYNCREATE(CQglDemoView, CView)

BEGIN_MESSAGE_MAP(CQglDemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
    ON_WM_KEYDOWN()
    ON_WM_MOUSEWHEEL()
    ON_WM_KEYUP()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CQglDemoView 构造/析构

CQglDemoView::CQglDemoView()
{
	// TODO: 在此处添加构造代码
	
	m_DispBuf = new BYTE[MAX_DISP_BUF_SIZE];
	memset(m_DispBuf, 0, MAX_DISP_BUF_SIZE);

    m_bCtrlPressed = false;
    m_bShiftPressed = false;
    m_ObjNum = 0;
    m_hQgl = Qgl_CreateHandle();

}

CQglDemoView::~CQglDemoView()
{
    Qgl_DestroyHandle(m_hQgl);
	delete m_DispBuf;
}

BOOL CQglDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CQglDemoView 绘制

void CQglDemoView::OnDraw(CDC* /*pDC*/)
{
	CQglDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CQglDemoView 打印

BOOL CQglDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CQglDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CQglDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


static void Trace (const char *str)
{
    TRACE(str);
}

void CQglDemoView::OnInitialUpdate()
{
    SetTracer(Trace);

    CreateDemoScene();

}

// CQglDemoView 诊断

#ifdef _DEBUG
void CQglDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CQglDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CQglDemoDoc* CQglDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CQglDemoDoc)));
	return (CQglDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CQglDemoView 消息处理程序

void CQglDemoView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CBitmap bitmap;

	bitmap.CreateBitmap(m_DispWidth, m_DispHeight, 1, 32, m_DispBuf);

	CDC dcComp;

	dcComp.CreateCompatibleDC(&dc);
	dcComp.SelectObject(&bitmap);

	BITMAP	bmpInfo;
	bitmap.GetObject(sizeof(BITMAP), &bmpInfo);

	dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcComp, 0, 0, SRCCOPY);

}

void CQglDemoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	RECT clientRect;
	GetClientRect(&clientRect);
	m_DispWidth = clientRect.right - clientRect.left;
	m_DispHeight = clientRect.bottom - clientRect.top;
	m_DispDataWidth = m_DispWidth * BMP_BPP;

    SetupCamSystem();
}


void CQglDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
#if 0
	TestPolygon();
	TestClip();

#endif
#if 1
    DrawWireframe();
#endif

    Invalidate();
	CView::OnLButtonDown(nFlags, point);
}

// Key map
enum
{
    m_CharRightShift = 13,
    m_CharLeftShift = 16,
    k_CharCtrl = 17,
    k_CharLeft = 37,
    k_CharUp = 38,
    k_CharRight = 39,
    k_CharDown = 40,
};

void CQglDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    switch (nChar)
    {
    case k_CharLeft:
        if (m_bCtrlPressed)
        {
            RotateLeftRight(-0.05);
        }
        else if (m_bShiftPressed)
        {
            RotateLensAxis(-0.05);
        }
        else
        {
            Move(k_Left);
        }
        break;
    case k_CharUp:
        if (m_bCtrlPressed)
        {
            RotateUpDown(0.05);
        }
        else if (m_bShiftPressed)
        {
            Move(k_Forward);
        }
        else
        {
            Move(k_Up);
        }
        break;
    case k_CharRight:
        if (m_bCtrlPressed)
        {
            RotateLeftRight(0.05);
        }
        else if (m_bShiftPressed)
        {
            RotateLensAxis(0.05);
        }
        else
        {
            Move(k_Right);
        }
        break;
    case k_CharDown:
        if (m_bCtrlPressed)
        {
            RotateUpDown(-0.05);
        }
        else if (m_bShiftPressed)
        {
            Move(k_Backward);
        }
        else
        {
            Move(k_Down);
        }
        break;
    case k_CharCtrl:
        m_bCtrlPressed = true;
        break;
    case m_CharLeftShift: case m_CharRightShift:
        m_bShiftPressed = true;
        break;
    }

    DrawWireframe();

    Invalidate();

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CQglDemoView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    switch (nChar)
    {
    case k_CharCtrl:
        m_bCtrlPressed = false;
        break;
    case m_CharLeftShift: case m_CharRightShift:
        m_bShiftPressed = false;
        break;
    }

    CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


BOOL CQglDemoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    return CView::OnMouseWheel(nFlags, zDelta, pt);
}



/**
 * Key down table
 * 
 * nFlags plus 0x4000, when hold the key down
 * nFlags plus 0xC000, for corresponding key up
 *
 * up: 
 * nChar=37,nRepCnt=1,nFlags=331
 * right: 
 * nChar=38,nRepCnt=1,nFlags=328
 * left: 
 * nChar=39,nRepCnt=1,nFlags=333
 * down: 
 * nChar=40,nRepCnt=1,nFlags=336
 * 
 * 
 * left ctrl:
 * nChar=17,nRepCnt=1,nFlags=29
 * right ctrl:
 * nChar=17,nRepCnt=1,nFlags=285
 * 
 * page up:
 * nChar=33,nRepCnt=1,nFlags=329
 * page down:
 * nChar=34,nRepCnt=1,nFlags=337
 * 
 * home:
 * nChar=36,nRepCnt=1,nFlags=327
 * end:
 * nChar=35,nRepCnt=1,nFlags=335
 * 
 * left shift:
 * nChar=16,nRepCnt=1,nFlags=42
 * right shift:
 * nChar=13,nRepCnt=1,nFlags=28
 * 
 */
void CQglDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    RaytraceRender();

    Invalidate();

    CView::OnLButtonDblClk(nFlags, point);
}
