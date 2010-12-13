// QglDemoDoc.cpp :  CQglDemoDoc 类的实现
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


// CQglDemoDoc 构造/析构

CQglDemoDoc::CQglDemoDoc()
{
	// TODO: 在此添加一次性构造代码

}

CQglDemoDoc::~CQglDemoDoc()
{
}

BOOL CQglDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CQglDemoDoc 序列化

void CQglDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CQglDemoDoc 诊断

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


// CQglDemoDoc 命令
