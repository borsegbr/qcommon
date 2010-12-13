// QcdDemoView.h : interface of the CQcdDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QcdDemoVIEW_H__7BAB2E54_596B_4E1B_964B_46EBD9A9F307__INCLUDED_)
#define AFX_QcdDemoVIEW_H__7BAB2E54_596B_4E1B_964B_46EBD9A9F307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQcdDemoView : public CView
{
protected:
    unsigned char m_ArrowPress;

protected: // create from serialization only
	CQcdDemoView();
	DECLARE_DYNCREATE(CQcdDemoView)

// Attributes
public:
	CQcdDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQcdDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CQcdDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CQcdDemoView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in QcdDemoView.cpp
inline CQcdDemoDoc* CQcdDemoView::GetDocument()
   { return (CQcdDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QcdDemoVIEW_H__7BAB2E54_596B_4E1B_964B_46EBD9A9F307__INCLUDED_)
