// QglDemoDoc.cpp :  CQglDemoDoc ���ʵ��
//

#include "stdafx.h"
#include "QglDemo.h"

#include "QglDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQglDemoDoc

IMPLEMENT_DYNCREATE(CQglDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CQglDemoDoc, CDocument)
END_MESSAGE_MAP()


// CQglDemoDoc ����/����

CQglDemoDoc::CQglDemoDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CQglDemoDoc::~CQglDemoDoc()
{
}

BOOL CQglDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CQglDemoDoc ���л�

void CQglDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CQglDemoDoc ���

#ifdef _DEBUG
void CQglDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CQglDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CQglDemoDoc ����
