// QglDemoView.h : CQglDemoView ��Ľӿ�
//


#pragma once

#include "qgl.h"



class CQglDemoView : public CView
{
protected: // �������л�����
	CQglDemoView();
	DECLARE_DYNCREATE(CQglDemoView)

// ����
public:
	CQglDemoDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// ʵ��
public:
	virtual ~CQglDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();



// �ҵĶ���
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

#ifndef _DEBUG  // QglDemoView.cpp �ĵ��԰汾
inline CQglDemoDoc* CQglDemoView::GetDocument() const
   { return reinterpret_cast<CQglDemoDoc*>(m_pDocument); }
#endif

