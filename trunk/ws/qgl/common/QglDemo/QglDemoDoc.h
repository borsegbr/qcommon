// QglDemoDoc.h :  CQglDemoDoc ��Ľӿ�
//


#pragma once

class CQglDemoDoc : public CDocument
{
protected: // �������л�����
	CQglDemoDoc();
	DECLARE_DYNCREATE(CQglDemoDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CQglDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


