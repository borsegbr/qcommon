// QcdDemoDoc.h : interface of the CQcdDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QcdDemoDOC_H__2CB0F653_B080_49FD_8AC3_3262A3D3D6B2__INCLUDED_)
#define AFX_QcdDemoDOC_H__2CB0F653_B080_49FD_8AC3_3262A3D3D6B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQcdDemoDoc : public CDocument
{
protected: // create from serialization only
	CQcdDemoDoc();
	DECLARE_DYNCREATE(CQcdDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQcdDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CQcdDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CQcdDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QcdDemoDOC_H__2CB0F653_B080_49FD_8AC3_3262A3D3D6B2__INCLUDED_)
