#include "QcdWorld.h"

#include <mmsystem.h>


void QcdObject::Draw(CDC *pDC, int x, int y, int nScrCx, int nScrCy)
{
	int x1 = (int)(x - radius);
	int x2 = (int)(x + radius);
	int y1 = (int)(y - radius);
	int y2 = (int)(y + radius);
	if (x2 < 0 || x1 >= nScrCx) return;
	if (y2 < 0 || y1 >= nScrCy) return;
	pDC->Ellipse(x1, y1, x2, y2);
	CString title;
	title.Format("%d", this->id);
	pDC->TextOut(x-4,y-7,title);
}

void QcdMan::Draw(CDC *pDC, int x, int y, int nScrCx, int nScrCy)
{
    QcdObject::Draw(pDC, x, y, nScrCx, nScrCy);
}

void QcdMonster::Draw(CDC *pDC, int x, int y, int nScrCx, int nScrCy)
{
    QcdObject::Draw(pDC, x, y, nScrCx, nScrCy);
}


QcdWorld::QcdWorld()
	: m_ObjCount(0), m_ScrCx(400), m_ScrCy(300)
	, m_UcBufpStart(0), m_UcBufLen(0), m_pView(0)
	, m_eThreadStat(k_Stopped), m_ManRgnCx(200), m_ManRgnCy(150)
	, m_Cx(1600), m_Cy(1200), m_bMemDcCreated(false)
{
    SetThread(ThreadProc, this);

	// debug
	m_fpUi=0;
	m_eUiMode = k_UiNone;
}

QcdWorld::~QcdWorld()
{
    TerminateThread();

	int i;
	for (i=0; i<m_ObjCount; i++)
	{
		if (m_Objects[i])
		{
			delete m_Objects[i];
		}
	}

    if (m_MemDc.m_hDC)
    {
        m_MemDc.DeleteDC();
    }
    if (m_MemBmp.m_hObject)
    {
        m_MemBmp.DeleteObject();
    }

	// debug
	if (m_fpUi) fclose(m_fpUi);

}

void QcdWorld::SetUiMode(UiMode eUiMode)
{
    m_eUiMode = eUiMode;
}

void QcdWorld::Init(UiMode eUiMode)
{
    m_eUiMode = eUiMode;

    PrepareWorld();

    m_Qcd.BindClashList(&m_ClashList);

    // Init play time and set base time to it
    m_PlayTime = 0;
    m_ClashList.SetBaseTime(m_PlayTime);

    // Get the first clash
    m_Qcd.NextClashes();
    Instant &inst = m_ClashList.NextInstant();
    m_NextClashTime = inst.time;

    // debug
	if (m_eUiMode==k_UiOut)
	{
		m_fpUi = fopen("ui.txt", "w");
		fclose(m_fpUi); m_fpUi=0;
	}
	else if (m_eUiMode==k_UiIn)
	{
		m_fpUi = fopen("ui.txt", "r");
	}

}

void QcdWorld::SetScreen(CView *pView, int cx, int cy)
{
	m_pView = pView;
	m_ScrCx = cx;
	m_ScrCy = cy;
	m_ManRgnCx = cx/2;
	m_ManRgnCy = cy/2;
}

void QcdWorld::Start()
{
	m_eThreadStat = k_Starting;
	this->CreateThread();
}

void QcdWorld::Stop()
{
	if (m_eThreadStat == k_Stopped)
		return;

	if (m_eThreadStat == k_Stopping)	// anthor instance of Stop() is stopping it
		return;

	int tryCount = 0;
	int k_MaxTry = 3;
	for ( ; m_eThreadStat == k_Starting && tryCount < k_MaxTry; tryCount++)
	{
		m_eThreadStat = k_Stopping;
		Sleep(20);
	}

	for (tryCount = 0; m_eThreadStat == k_Running && tryCount < k_MaxTry; tryCount++)
	{
		m_eThreadStat = k_Stopping;
		Sleep(20);
	}

	m_eThreadStat = k_Stopped;
}

void QcdWorld::PushUserInput (const UserCmd &uc)
{
	int iUcBufpEnd = m_UcBufpStart + m_UcBufLen;
	iUcBufpEnd %= UCBUF_SIZE;
	
	m_UcBufMutex.Lock();
	
	if (m_UcBufLen==UCBUF_SIZE)
	{
		m_UcBufpStart++;
		m_UcBufpStart %= UCBUF_SIZE;
	}
	else
	{
		m_UcBufLen++;
	}
	m_UcBuf[iUcBufpEnd] = uc;

	m_UcBufMutex.Unlock();
	
}

void QcdWorld::DrawFrame(CDC *pDC)
{
    if (m_MemDc.m_hDC)
    {
        pDC->BitBlt(0, 0, m_ScrCx, m_ScrCy, &m_MemDc, 0, 0, SRCCOPY);
    }
    else
    {
        // error, mem DC not created
    }
}

DWORD QcdWorld::ThreadProc(LPVOID pData)
{
	QcdWorld *pCw = reinterpret_cast<QcdWorld *>(pData);
	long	k_Interval = 40;
	long	k_Safe = 5;

	DWORD	dwTCur = timeGetTime();
	long	tCur, tNext, tBase;
    const Time k_Dt = ((Time)k_Interval)/1000;

	tBase = dwTCur - (dwTCur % k_Interval);
	tCur = dwTCur % k_Interval;
	tNext = k_Interval;

	if (pCw->m_ObjCount==0)	// not initialized
		return 0;

#define GET_TIME(t)	t=timeGetTime(); t-=tBase
	
	pCw->m_eThreadStat = k_Running;

    // This program can run no more than 0xffffffff milliseconds
	while (pCw->m_eThreadStat == k_Running)	
	{
		do {
			if (tCur-tNext > k_Safe)
			{
				Sleep(tCur - tNext - k_Safe);
			}

			GET_TIME(tCur);
		} while(tCur-tNext<=0);
		
		do {
			// TODO:
			// ...
			
			pCw->ProcessUserInput();
            pCw->RunOneStep(k_Dt);
			pCw->RenderFrame();
						
			GET_TIME(tCur);
			tNext = tCur+k_Interval;
		} while(tNext-tCur <= k_Safe);
		Sleep(tNext-tCur-k_Safe);
	}
	TRACE("leave\n");
	pCw->m_eThreadStat = k_ExitProc;

    return 0;
}

void QcdWorld::ProcessUserInput()
{
	const int k_VelStep = 20;
    const int k_VelStepDiag = 16;
	QcdMan *pCm = dynamic_cast<QcdMan *>(m_Objects[0]);
	//pCm->vx=pCm->vy = 0;
    int oldVx = (int)pCm->vx;
    int oldVy = (int)pCm->vy;


	m_UcBufMutex.Lock();
	for ( ; m_UcBufLen>0; m_UcBufLen--)
	{
		UserCmd &uc = m_UcBuf[m_UcBufpStart];

		switch (uc.cmd)
		{
		case UserCmd::k_MoveUp:
			pCm->vy-=k_VelStep;
			break;
		case UserCmd::k_MoveDown:
			pCm->vy+=k_VelStep;
			break;
		case UserCmd::k_MoveLeft:
			pCm->vx-=k_VelStep;
			break;
		case UserCmd::k_MoveRight:
			pCm->vx+=k_VelStep;
			break;
        case UserCmd::k_MoveUpLeft:
            pCm->vx-=k_VelStepDiag;
            pCm->vy-=k_VelStepDiag;
            break;
        case UserCmd::k_MoveUpRight:
            pCm->vx+=k_VelStepDiag;
            pCm->vy-=k_VelStepDiag;
            break;
        case UserCmd::k_MoveDownLeft:
            pCm->vx-=k_VelStepDiag;
            pCm->vy+=k_VelStepDiag;
            break;
        case UserCmd::k_MoveDownRight:
            pCm->vx+=k_VelStepDiag;
            pCm->vy+=k_VelStepDiag;
            break;
		}

		m_UcBufpStart++;
		m_UcBufpStart %= UCBUF_SIZE;

	}

	if (m_eUiMode==k_UiOut)
	{	// debug
		m_fpUi = fopen("ui.txt", "a");
		fprintf(m_fpUi, "%f,%f\n",pCm->vx,pCm->vy);
		fclose(m_fpUi); m_fpUi=0;
	}
	else if (m_eUiMode==k_UiIn)
	{
        if (m_fpUi)
		    fscanf(m_fpUi, "%f,%f\n", &pCm->vx,&pCm->vy);
	}

	
    m_ManMovChangedInThisStep = (pCm->vx != oldVx || pCm->vy != oldVy);
    if (m_ManMovChangedInThisStep)
    {
        ObjItr objItr = m_ClashList.AcquireObject(0);
        TRACE("== BEGIN of Cancelling connections due to man mov == \n");
        m_ClashList.CancelConnections(objItr);
        TRACE("== END of Cancelling connections due to man mov ==\n");
        if (pCm->IsMoving())
            m_ClashList.TurnToMoving(objItr);
        else
            m_ClashList.TurnToStill(objItr);

    }

    m_UcBufMutex.Unlock();
	
}

void QcdWorld::RunOneStep(Time dt)
{
    bool bLooped = false;
    Time nextTime = m_PlayTime + dt;
    Time t = m_PlayTime;

    Time freeTime;

    //TRACE("t = %f/next = %f\n", t, m_NextClashTime);

__again:
    while (m_NextClashTime < nextTime)
    {
        freeTime = m_NextClashTime - t;
        FreeMove(freeTime);

        // deal with clashes
        Instant &inst = m_ClashList.NextInstant();
        Instant::ClashList::iterator clItr = inst.clashes.begin();

        for ( ; clItr != inst.clashes.end(); ++clItr)
        {
            ObjItr oiA = m_ClashList.AcquireObject(clItr->iA);
            ObjItr oiB = m_ClashList.AcquireObject(clItr->iB);
            Object *pA = oiA.pObj;
            Object *pB = oiB.pObj;
            if (pA==0||pB==0)
            {
                continue;
            }
            TestBall *pBallA = dynamic_cast<TestBall*>(pA);
            TestBall *pBallB = dynamic_cast<TestBall*>(pB);
			TRACE("HIT, %d,%d\n",clItr->iA,clItr->iB);

            pBallA->Hit(pB);

            /*
             * Note: 
             * This is for demo use, so it is not strictly correct here.
             * If one wants to make use of the still list to improve
             * the performance, one should check here regularly to 
             * ensure that each seemingly still object judged as unmoving
             * here is distant enough to any objects concerned.
             */

            if (pBallA->IsMoving())
            {
                m_ClashList.TurnToMoving(oiA);
            }
            else
            {
                m_ClashList.TurnToStill(oiA);
            }

            if (pBallB->IsMoving())
            {
                m_ClashList.TurnToMoving(oiB);
            }
            else
            {
                m_ClashList.TurnToStill(oiB);
            }

        }
        // consume
        m_ClashList.ConsumeInstant();

        t = m_NextClashTime;
        m_ClashList.SetBaseTime(t);
        TRACE("== BEGIN of normal NextClashes at %f == \n", t);
        m_Qcd.NextClashes();
        TRACE("== END of normal NextClashes at %f == \n", t);
        m_NextClashTime = m_ClashList.NextInstant().time;
        TRACE("base = %f, next after hit %f\n", t, m_NextClashTime);
        bLooped = true;
    }
    if (!bLooped && m_ManMovChangedInThisStep)
    {
        TRACE("free aiming begin\n");
        m_ClashList.SetBaseTime(t);
        TRACE("== BEGIN of NextClashes due to man mov at %f == \n", t);
        m_Qcd.NextClashes();
        TRACE("== END of NextClashes due to man mov at %f == \n", t);
        m_NextClashTime = m_ClashList.NextInstant().time;
        if (m_NextClashTime > ClashList::k_InfTime)
            m_NextClashTime = ClashList::k_InfTime;

        TRACE("next clash time = %f, current time = %f\n", m_NextClashTime, t);
        bLooped = true;
        goto __again;
    }


    freeTime = nextTime - t;
    FreeMove(freeTime);

    m_PlayTime = nextTime;

}

void QcdWorld::FreeMove(Time dt)
{
    for (int i = 0; i < m_ObjCount; i++)
    {
        m_Objects[i]->FreeMove(dt);
    }
}

void QcdWorld::RenderFrame()
{
	// scaling needed if screen and world are not in the same scale

    // keep the man in the central region
	int manRgnPx1 = (m_ScrCx - m_ManRgnCx) / 2;
	int manRgnPy1 = (m_ScrCy - m_ManRgnCy) / 2;
	int manRgnPx2 = manRgnPx1 + m_ManRgnCx;
	int manRgnPy2 = manRgnPy1 + m_ManRgnCy;
	QcdMan *pCm = dynamic_cast<QcdMan *>(m_Objects[0]);
	
	int xDrawMan = (int)(pCm->x - m_ScrPx);
	int yDrawMan = (int)(pCm->y - m_ScrPy);
	if (xDrawMan < manRgnPx1)
	{
		m_ScrPx += xDrawMan - manRgnPx1;
		if (m_ScrPx < 0) m_ScrPx = 0;
	}
	else if (xDrawMan >= manRgnPx2)
	{
		m_ScrPx += xDrawMan - manRgnPx2;
		if (m_ScrPx + m_ScrCx > m_Cx) m_ScrPx = m_Cx - m_ScrCx;
	}
	if (yDrawMan < manRgnPy1)
	{
		m_ScrPy += yDrawMan - manRgnPy1;
		if (m_ScrPy < 0) m_ScrPy = 0;
	}
	else if (yDrawMan >= manRgnPy2)
	{
		m_ScrPy += yDrawMan - manRgnPy2;
		if (m_ScrPy + m_ScrCy > m_Cy) m_ScrPy = m_Cy - m_ScrCy;
	}

    if (!m_MemDc.m_hDC)
    {
        CDC *pDC = m_pView->GetDC();
        m_MemDc.CreateCompatibleDC(pDC);
        m_MemBmp.CreateCompatibleBitmap(pDC, m_ScrCx, m_ScrCy);
        m_MemDc.SelectObject(&m_MemBmp);
    }

    if (m_MemDc.m_hDC)
    {
        // TODO:
        // Draw to the buffer

        m_MemDc.FillSolidRect(0, 0, m_ScrCx, m_ScrCy, RGB(255,255,255));
	

#ifdef VCDOTNET
        m_MemDc.SetDCPenColor(RGB(0,0,0));
#else
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
		m_MemDc.SelectObject(&pen);
#endif

        int i;
        for (i=0; i<m_ObjCount; i++)
        {
            if (m_Objects[i])
            {
                int xDraw = (int)(m_Objects[i]->x - m_ScrPx);	
                int yDraw = (int)(m_Objects[i]->y - m_ScrPy);
                m_Objects[i]->Draw(&m_MemDc, xDraw, yDraw, m_ScrCx, m_ScrCy);
            }
        }

        {
            char pszInfo[32];
            sprintf(pszInfo, "MAN:%4.1f,%4.1f", m_Objects[0]->x, m_Objects[0]->y);
            m_MemDc.TextOut(0,0,pszInfo);
        }
    }
	
	m_pView->Invalidate(TRUE);
		
}


void QcdWorld::PrepareWorld()
{
    struct BallData
    {
        Real x, y, z;
        Real vx, vy, vz;
        Real radius;
        Real mass;
        bool bMovable;
    };
    const int k_BallCount = 8;
    const BallData k_BallData[k_BallCount] = {
        { 200,200,0,    0,0,0,      5,  20, true },  // man
        { 0,0,0,        7,9,0,      10, 10, true },
        { 70,250,0,     20,12,0,    10, 5,  true },
        { 100,100,0,    30,20,0,    20, 30, true },
        { 320,240,0,    -39,32,0,   10, 50, true },
        { 178,60,0,     -30,22,0,   30, 5,  true },
        { 250,70,0,     -30,-22,0,  5,  7,  true },
        { 180,180,0,    0, 0,0,     15, 900,true },
    };

    m_ObjCount = k_BallCount;

    Real minx=(Real)10000, miny=(Real)10000;
    Real maxx=(Real)-10000, maxy=(Real)-10000;
    for (int i = 0; i < m_ObjCount; i++)
    {
        if (k_BallData[i].x < minx) minx = k_BallData[i].x;
        if (k_BallData[i].x > maxx) maxx = k_BallData[i].x;
        if (k_BallData[i].y < miny) miny = k_BallData[i].y;
        if (k_BallData[i].y > maxy) maxy = k_BallData[i].y;
    }

    Real basex = ((Real)m_Cx - (minx+maxx))/ 2;
    Real basey = ((Real)m_Cy - (miny+maxy))/ 2;



    m_Objects[0] = new QcdMan(0, k_BallData[0].x+basex, k_BallData[0].y+basey);
    m_Objects[0]->mass = k_BallData[0].mass;
    m_Objects[0]->bMovable = k_BallData[0].bMovable;


    for (int i = 1; i < m_ObjCount; i++)
    {
        m_Objects[i] = new QcdMonster(i, k_BallData[i].x+basex, k_BallData[i].y+basey, k_BallData[i].radius);
        m_Objects[i]->vx = k_BallData[i].vx;
        m_Objects[i]->vy = k_BallData[i].vy;
        m_Objects[i]->mass = k_BallData[i].mass;
        m_Objects[i]->bMovable = k_BallData[i].bMovable;
    }

    // Load the balls to the clash list
    LoadCdBalls();
}


void QcdWorld::LoadCdBalls()
{
    m_ClashList.ClearAll();
    for (int i = 0; i < m_ObjCount; ++i)
    {
        TestBall *pBall = m_Objects[i];
        TRACE("%d-%x\n", i, pBall);
        bool bMoving = pBall->IsMoving();
        m_ClashList.AddObject(pBall, bMoving);
    }
}

