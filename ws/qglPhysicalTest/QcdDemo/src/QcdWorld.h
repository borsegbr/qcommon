#ifndef _QcdWorld_H
#define _QcdWorld_H


#include "../stdafx.h"
#include <afxmt.h>

#include "qcd_testball.h"
#include "qul/mthread/CThread.h"


#define MAX_OBJECTS		1024
#define UCBUF_SIZE		16

using namespace qul;

class QcdObject : public TestBall
{
public:
	int id;

public:
	QcdObject (int _id = 0) : id(_id) { x=0, y=0; radius = (Real)10; }
	QcdObject (int _id, Real _x, Real _y, Real _radius = (Real)10) : id(_id) 
    { x = (Real)_x, y = (Real)_y; radius = (Real)_radius; }
	virtual void Draw(CDC *pDC, int x, int y, int nScrCx, int nScrCy);
};

class QcdMan : public QcdObject
{
public:
	QcdMan (int _id = 0) : QcdObject(_id) {}
	QcdMan (int _id, Real _x, Real _y, Real _radius = (Real)5) 
        : QcdObject(_id,_x,_y,_radius) {}
	virtual void Draw(CDC *pDC, int x, int y, int nScrCx, int nScrCy);
};

class QcdMonster : public QcdObject
{
public:
    QcdMonster (int _id = 0) : QcdObject(_id) {}
    QcdMonster (int _id, Real _x, Real _y, Real _radius = (Real)5) 
        : QcdObject(_id,_x,_y,_radius) {}
    virtual void Draw(CDC *pDC, int x, int y, int nScrCx, int nScrCy);
};

struct UserCmd
{
	enum Cmd 
	{
		k_None = 0,

		k_MoveUp        = 0x1,  // 0001
		k_MoveDown      = 0x2,  // 0010
		k_MoveLeft      = 0x4,  // 0100
        k_MoveUpLeft    = 0x5,
        k_MoveDownLeft  = 0x6,
		k_MoveRight     = 0x8,  // 1000
        k_MoveUpRight   = 0x9,
        k_MoveDownRight = 0xa,
	} cmd;
	
	UserCmd() : cmd(k_None){}
	UserCmd(const Cmd &_cmd) : cmd(_cmd){}
};

class QcdWorld : public CThread
{
public:
    enum UiMode
    {
        k_UiNone,
        k_UiIn,
        k_UiOut,
    } m_eUiMode;

public:
	QcdWorld();
	virtual ~QcdWorld();

    void SetUiMode(UiMode eUiMode);
	void Init(UiMode eUiMode = k_UiNone);
	void SetScreen(CView *pView, int cx, int cy);

	void Start();
	void Stop();
	void PushUserInput (const UserCmd &uc);
	void DrawFrame (CDC *pDC);
		
protected:
    typedef ClashList::Real     Real;
    typedef ClashList::Time     Time;
    typedef ClashList::Instant  Instant;
    typedef ClashList::ObjInfo  ObjInfo;
    typedef ClashList::Object   Object;
    typedef ClashList::ObjItr   ObjItr;

protected:
	static DWORD WINAPI ThreadProc(LPVOID pData);
	
	void ProcessUserInput();
	void RunOneStep(Time dt);
	void RenderFrame();

    void FreeMove(Time dt);

    void PrepareWorld();

    // Collision Detection
    void LoadCdBalls();
    void PerformCD(Time dt);

protected:
	// thread
	HANDLE m_hThread;
	enum
	{
		k_Stopped,
		k_Starting,
		k_Running,
		k_ExitProc,
		k_Stopping,
	} m_eThreadStat;

	// objects
	QcdObject * m_Objects[MAX_OBJECTS];
	int m_ObjCount;

	// world attributes
	int m_Cx, m_Cy;			// world size
	
	// input
	UserCmd m_UcBuf[UCBUF_SIZE];
	int m_UcBufpStart, m_UcBufLen;
	CMutex	m_UcBufMutex;

	// output
	int m_ManRgnCx, m_ManRgnCy;	// screen region that man can freely move
	int m_ScrCx, m_ScrCy;	// screen size in screen scale
	int m_ScrPx, m_ScrPy;	// the position of the left top corner of the screen

	// currently the scale of the world is same as that of the screen
	CView	*m_pView;

    // Dual buffer facilities
    bool    m_bMemDcCreated;
    CDC     m_MemDc;
    CBitmap m_MemBmp;

    // CD
    ClashList       m_ClashList;
    QG_CD_001<>     m_Qcd;
    Time            m_PlayTime;
    Time            m_NextClashTime;
    bool            m_ManMovChangedInThisStep;

	// debug
	FILE*	m_fpUi;
};


#endif  /* ifndef _QcdWorld_H */


