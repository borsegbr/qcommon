// QglDemoView.h : CQglDemoView 类的接口
//


#pragma once

#include "qgl.h"



class CQglDemoView : public CView
{
protected: // 仅从序列化创建
	CQglDemoView();
	DECLARE_DYNCREATE(CQglDemoView)

// 属性
public:
	CQglDemoDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// 实现
public:
	virtual ~CQglDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();



// 我的定义
protected:

// line drawing
	static void LineFill (void *data, int x, int y);
	static void PixelFill (void *data, int x, int y);
	static void SeedFillSet (void *data, int x, int y, int chnl, int val);
	static int SeedFillGet (void *data, int x, int y, int chnl);

	void SetBorder ();
	void TestPolygon ();
	void TestClip ();
	void DrawRect (QglRect *rect, QglPixie *pixie);

// rendering
	int		m_DispWidth;
	int		m_DispHeight;
	int		m_DispDataWidth;
	BYTE	*m_DispBuf;

	struct SfillBmp
	{
		BYTE	*data;
		int		left, top, right, bottom;
	};
	SfillBmp	m_SfillBmp;

#define MAX_OBJECTS 100
    QglObjHandle    m_Objects[MAX_OBJECTS];
    int             m_ObjNum;

    QglReal     m_CamWidth, m_CamHeight;
    QglReal     m_CamD;
    int         m_TraceDepth;
    QglHandle   m_hQgl;
    bool        m_bCtrlPressed;
    bool        m_bShiftPressed;

    enum MoveDir
    {
        k_Left,
        k_Right,
        k_Forward,
        k_Backward,
        k_Up,
        k_Down
    };

    void CreateDemoScene ();
    void SetupCamSystem ();

    void DrawWireframe ();
    void RaytraceRender ();

    void Move (MoveDir dir);
    void RotateUpDown (QglReal dPsy);
    void RotateLeftRight (QglReal dTheta);
    void RotateLensAxis (QglReal dTilt);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // QglDemoView.cpp 的调试版本
inline CQglDemoDoc* CQglDemoView::GetDocument() const
   { return reinterpret_cast<CQglDemoDoc*>(m_pDocument); }
#endif

