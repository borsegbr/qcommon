#ifndef _collision_detection_H
#define _collision_detection_H


#include <vector>
#include <list>


namespace qgl {

/*
 * <Summary>
 * The default clash list that serves as a demo
 * </Summary>
 */
template <class _Real = float>
struct QG_CD_001_ClashList
{
    typedef _Real   Real;
    typedef _Real   Time;

    static const Time k_InfTime;
	
    /* Clashes */
    struct Clash
    {
        int iA; // index of object A in the object map m_ObjMap
        int iB; // index of object B
    };
    struct Instant
    {
        typedef std::list<Clash>    ClashList;
        ClashList   clashes;    // data structure enhancement possible
        Time        time;

        Instant() : time(k_InfTime) {}
    };

public:
    struct Object   /* 3D object */
    {
        Real    x, y, z;        // position
        Real    vx, vy, vz;     // velocity
        virtual void VirtualClash (Object *that, Time &t) = 0;

        Object() : x(0),y(0),z(0),vx(0),vy(0),vz(0) {}
    };

    /* Object lists */

    typedef std::list<int>      ObjList;
    typedef ObjList::iterator   ObjListItr;

    struct ObjItr
    {
        Object      *pObj;
        int         index;      // index of the object in `m_ObjMap'
        ObjListItr  listItr;

        ObjItr() : pObj(0), index(-1) {}
        ObjItr (const ObjItr &copy) : pObj(copy.pObj), index(copy.index), listItr(copy.listItr) {}
        ObjItr (Object *p, int idx, ObjListItr li) : pObj(p), index(idx), listItr(li) {}
        operator bool() { return pObj!=0; }
        void SetNull () { pObj = 0; index = -1; }
    };

    /* Object lists related */
    struct ObjInfo
    {
        Object      *pObj;
        Time        time;
        int         nClash;

        /* postion in the object list */
        ObjListItr  itr;
        bool        bMoving;

        ObjInfo() : pObj(0), time(k_InfTime), bMoving(false), nClash(0) {}
    };

public:
    QG_CD_001_ClashList ();
    ~QG_CD_001_ClashList ();

    /*
     * The following 3 subrotines will affects:
     * the object lists
     * the object map
     */
    int AddObject (Object *pObj, bool bMoving);

    // the following 2 routines in addition to affecting the 2 presented above 
    // remove the corresponsing element in the clash list or clear the whole list
    // as well since it no longer makes sense
    /* 
     * Removing from object map doesn't change the positions of other elements in the array 
     * A chain list maintaining the holes of the array may be set up in the future,
     * before that, I have to search any possible hole for the new object added and with the
     * index of that position assigned to it. A hole is indicated by its value of invalid info
     * (pObj == 0).
     */
    void RemoveObject (int index);
    void ClearAll ();   // clear all objects

    void TurnToMoving (ObjItr &oitr);
    void TurnToStill (ObjItr &oitr);

    /* consume the first instant */
    void SetBaseTime(Time t);
    Instant & NextInstant();
    void ConsumeInstant();

    /* access the object */
    ObjItr AcquireObject(int index);

    void AttemptConnection (ObjItr pA, ObjItr pB, Time t);
    void CancelConnections (ObjItr pA);
    void AcceptConnections (ObjItr pA);
    bool TryClash (ObjItr pA, ObjItr pB, Time &t);
    void ClashTimeSofar (ObjItr pA, Time &t);

    ObjItr FirstMoving ();
    ObjItr NextMoving (ObjItr cur);
    ObjItr FirstStill ();
    ObjItr NextStill (ObjItr cur);

protected:
    void BreakAllConnections (int iA);
    /*
     * Returns the first clash at the time no earlier than `t'
     */
    bool Search ( Time t, int &m );

protected:
    std::vector<Instant>    m_InstList; /* ordered by time */
    bool                    m_bNeedConfirm;

    std::vector<ObjInfo>    m_ObjMap;
    int                     m_FirstHole;
    ObjList                 m_MovingList;
    ObjList                 m_StillList;

    Time                    m_BaseTime;

/* debug */
protected:
#if defined(DEBUG_collision_detection) && DEBUG_collision_detection
#   if defined(TRACE)
#       define TRACE_collision_detection    TRACE
#   else
#       define TRACE_collision_detection    printf
#   endif
    void ViewAllTime() { for (int i=0; i<(int)m_ObjMap.size(); i++){ TRACE_collision_detection("(%f,%d),",m_ObjMap[i].time,m_ObjMap[i].nClash); } TRACE_collision_detection("instsize=%d\n", m_InstList.size()); }
    void ViewList() { for(int i=0;i<(int)m_InstList.size();i++) { TRACE_collision_detection("(t=%f,n=%d),", m_InstList[i].time,m_InstList[i].clashes.size());} TRACE_collision_detection("\n");  }
#else
#   define TRACE_collision_detection(...)
#   define ViewAllTime()
#   define ViewList()
#endif

};

template <class _Real>
const typename QG_CD_001_ClashList<_Real>::Time  QG_CD_001_ClashList<_Real>::k_InfTime = 10000;


template <class _Real>
QG_CD_001_ClashList<_Real>::QG_CD_001_ClashList () 
    : m_FirstHole(0), m_bNeedConfirm(false), m_BaseTime(0)
{
}

template <class _Real>
QG_CD_001_ClashList<_Real>::~QG_CD_001_ClashList ()
{
}

template <class _Real>
int QG_CD_001_ClashList<_Real>::AddObject (Object *pObj, bool bMoving)
{
    int     res = -1;
    ObjInfo info;
    if (bMoving)
    {
        m_MovingList.push_back(m_FirstHole);
        info.itr = m_MovingList.end(); --info.itr;
    }
    else
    {
        m_StillList.push_back(m_FirstHole);
        info.itr = m_StillList.end(); --info.itr;
    }
    info.bMoving = bMoving;
    info.time = k_InfTime;
    info.pObj = pObj;

    if (m_FirstHole == m_ObjMap.size())
    {
        m_ObjMap.push_back(info);
    }
    else
    {
        m_ObjMap[m_FirstHole] = info;
    }
    res = m_FirstHole;

    // find the next hole in the object map
    for (m_FirstHole++ ; m_FirstHole < (int)m_ObjMap.size(); m_FirstHole++)
    {
        if (m_ObjMap[m_FirstHole].time < 0)
        {
            break;
        }
    }
    return res;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::RemoveObject (int index)
{
    ObjListItr  olitr = m_ObjMap[index].itr;
    if (m_ObjMap[index].bMoving)
    {
        m_MovingList.erase(olitr);
    }
    else
    {
        m_StillList.erase(olitr);
    }

    if (m_ObjMap[index].time >= 0)
    {
        BreakAllConnections(index);
    }

    m_ObjMap[index].pObj = 0;
    m_ObjMap[index].time = k_InfTime;

    if (index < m_FirstHole)
    {
        m_FirstHole = index;
    }
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::ClearAll ()
{
    m_InstList.clear();
    m_ObjMap.clear();
    m_FirstHole = 0;
    m_MovingList.clear();
    m_StillList.clear();
    m_bNeedConfirm = false;
    m_BaseTime = 0;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::TurnToMoving (ObjItr &oitr)
{
    if (m_ObjMap[oitr.index].bMoving) return;
    m_MovingList.push_back(*oitr.listItr);
    ObjListItr itrNew = m_MovingList.end();
    --itrNew;
    m_StillList.erase(oitr.listItr);
    m_ObjMap[oitr.index].itr = oitr.listItr = itrNew;
    m_ObjMap[oitr.index].bMoving = true;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::TurnToStill (ObjItr &oitr)
{
    if (!m_ObjMap[oitr.index].bMoving) return;
    m_StillList.push_back(*oitr.listItr);
    ObjListItr itrNew = m_StillList.end();
    --itrNew;
    m_MovingList.erase(oitr.listItr);
    m_ObjMap[oitr.index].itr = oitr.listItr = itrNew;
    m_ObjMap[oitr.index].bMoving = false;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::SetBaseTime (Time t)
{
    m_BaseTime = t;
}

template <class _Real>
typename QG_CD_001_ClashList<_Real>::Instant & QG_CD_001_ClashList<_Real>::NextInstant ()
{
    if (m_InstList.empty())
    {
        static Instant inst;
        return inst;
    }
    return m_InstList[0];
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::ConsumeInstant ()
{
    if (!m_InstList.empty())
    {
        Instant::ClashList::iterator itr = m_InstList.begin()->clashes.begin();
        for ( ; itr != m_InstList.begin()->clashes.end(); ++itr )
        {
            m_ObjMap[itr->iA].time = k_InfTime;
            m_ObjMap[itr->iB].time = k_InfTime;
			m_ObjMap[itr->iA].nClash = 0;
			m_ObjMap[itr->iB].nClash = 0;
        }
        m_InstList.erase(m_InstList.begin());
    }
}

template <class _Real>
typename QG_CD_001_ClashList<_Real>::ObjItr 
    QG_CD_001_ClashList<_Real>::AcquireObject(int index)
{
    ObjItr objItr;
    if (index < 0 || index >= (int)m_ObjMap.size())
    {
        return objItr;
    }
    objItr.index = index;
    objItr.listItr = m_ObjMap[index].itr;
    objItr.pObj = m_ObjMap[index].pObj;
    return objItr;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::AttemptConnection (ObjItr pA, ObjItr pB, Time t)
{

    // Insert at the proper place in the `m_InstList' with respect to the time
    int instIndex;
    TRACE_collision_detection("BEGIN of AttemptConnection\n");

	TRACE_collision_detection("BeforeADDing, m_InstList.size()=%d\n", m_InstList.size());

    if (!Search(t, instIndex))
    {
        m_InstList.insert(m_InstList.begin()+instIndex, Instant());
    }

    // instant list
    Clash clash;
    clash.iA = pA.index;
    clash.iB = pB.index;
    m_InstList[instIndex].clashes.push_back(clash);
    m_InstList[instIndex].time = t;
    TRACE_collision_detection("ADDING %d,%d, with %d, time = %f\n", clash.iA, clash.iB, instIndex, m_InstList[instIndex].time);
	
    // clash time list
    m_ObjMap[pA.index].time = t;

    m_bNeedConfirm = true;

    ViewAllTime();
    TRACE_collision_detection("END of AttemptConnection\n");
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::CancelConnections (ObjItr pA)
{
    int iA = pA.index;
    TRACE_collision_detection("BEGIN of CancelConnections\n");
    BreakAllConnections(iA);
    // clash time list
    m_ObjMap[iA].time = k_InfTime;

    ViewAllTime();
    TRACE_collision_detection("END of CancelConnections\n");
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::AcceptConnections (ObjItr pA)
{
    if (!m_bNeedConfirm)
    {
        TRACE_collision_detection("BEGIN of AcceptConnections--NO CHANGE\n");
        ViewAllTime();
        TRACE_collision_detection("END of AcceptConnections--NO CHANGE\n");
        return;
    }

    Time t = m_ObjMap[pA.index].time;
    int iclist;

    if (!Search(t, iclist))
    {
        /*
         * the time of pA is k_InfTime
         */

        TRACE_collision_detection("BEGIN of AcceptConnections--NO CHANGE, inf time, t = %f\n", t);
        ViewList();
        ViewAllTime();
        TRACE_collision_detection("END of AcceptConnections--NO CHANGE, inf time, t = %f\n", t);
        return;
    }

    TRACE_collision_detection("BEGIN of AcceptConnections\n");

    assert(iclist<(int)m_InstList.size());

    Instant &inst = m_InstList[iclist];
    typename Instant::ClashList::iterator cli = inst.clashes.begin(), tmpcli;

    m_ObjMap[pA.index].nClash = 0;
    for ( ; cli != inst.clashes.end(); ++cli)
    {
        int iB;
        if (cli->iA==pA.index)
        {
            iB = cli->iB;
        }
        else if (cli->iB==pA.index)
        {
            iB = cli->iA;
        }
        else continue;
        Time tB = m_ObjMap[iB].time;

        if (tB > t)
        {
            BreakAllConnections(iB);
            m_ObjMap[iB].time = t;
        }
        m_ObjMap[iB].nClash++;
        m_ObjMap[pA.index].nClash++;

    }

    ViewAllTime();
    TRACE_collision_detection("END of AcceptConnections\n");

}

template <class _Real>
bool QG_CD_001_ClashList<_Real>::TryClash (ObjItr pA, ObjItr pB, Time &t)
{
    pA.pObj->VirtualClash(pB.pObj, t);
    t += m_BaseTime;
    if (t >= m_ObjMap[pB.index].time)
    {
        t = k_InfTime;
        return false;
    }
    return true;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::ClashTimeSofar (ObjItr pA, Time &t)
{
    t = m_ObjMap[pA.index].time;
    m_bNeedConfirm = false;
}

template <class _Real>
typename QG_CD_001_ClashList<_Real>::ObjItr QG_CD_001_ClashList<_Real>::FirstMoving ()
{
    if (m_MovingList.empty())
    {
        return ObjItr();
    }
    else
    {
        int mapIdx = *m_MovingList.begin();
        return ObjItr(m_ObjMap[mapIdx].pObj, mapIdx, m_MovingList.begin());
    }
}

template <class _Real>
typename QG_CD_001_ClashList<_Real>::ObjItr QG_CD_001_ClashList<_Real>::NextMoving (ObjItr cur)
{
    if (++cur.listItr == m_MovingList.end())
    {
        cur.pObj = 0;
        cur.index = -1;
    }
    else
    {
        cur.index = *cur.listItr;
        cur.pObj = m_ObjMap[cur.index].pObj;
    }
    return cur;
}

template <class _Real>
typename QG_CD_001_ClashList<_Real>::ObjItr QG_CD_001_ClashList<_Real>::FirstStill ()
{
    if (m_StillList.empty())
    {
        return ObjItr();
    }
    else
    {
        int mapIdx = *m_StillList.begin();
        return ObjItr(m_ObjMap[mapIdx].pObj, mapIdx, m_StillList.begin());
    }
}

template <class _Real>
typename QG_CD_001_ClashList<_Real>::ObjItr QG_CD_001_ClashList<_Real>::NextStill (ObjItr cur)
{
    if (++cur.listItr == m_StillList.end())
    {
        cur.pObj = 0;
        cur.index = -1;
    }
    else
    {
        cur.index = *cur.listItr;
        cur.pObj = m_ObjMap[cur.index].pObj;
    }
    return cur;
}

template <class _Real>
void QG_CD_001_ClashList<_Real>::BreakAllConnections (int iA)
{
    TRACE_collision_detection("BEGIN of BreakAllConnections\n");

    Time t = m_ObjMap[iA].time;
    int iclist;
    if (!Search(t, iclist))
    {
        TRACE_collision_detection("END of BreakAllConnections, not found %f\n", t);
        return;
    }
    assert(iclist<(int)m_InstList.size());
    Instant &inst = m_InstList[iclist];
    typename Instant::ClashList::iterator cli = inst.clashes.begin(), tmpcli;
    for ( ; cli != inst.clashes.end(); )
    {
        if (cli->iA==iA||cli->iB==iA)
        {
            int iB = cli->iA == iA? cli->iB : cli->iA;

            tmpcli = cli;
            ++tmpcli;
            // instant list
            inst.clashes.erase(cli);
            cli = tmpcli;
            
            m_ObjMap[iB].nClash--;
            if (m_ObjMap[iB].nClash <= 0)
            {
                m_ObjMap[iB].time = k_InfTime;
            }
        }
        else
            ++cli;
    }
    m_ObjMap[iA].nClash = 0;
    if (inst.clashes.size()==0)
    {
        m_InstList.erase(m_InstList.begin() + iclist);
    }

    {
        ViewAllTime();
    }
    TRACE_collision_detection("END of BreakAllConnections\n");
}

template <class _Real>
bool QG_CD_001_ClashList<_Real>::Search ( Time t, int &m )
/*
 * Returns the first clash at the time no earlier than `t'
 */
{
#define REAL_EQ(a,b)    (fabs((a)-(b))<0.0001)

    int n = (int)m_InstList.size();
    int i0 = 0, i1 = n;
    if (n == 0) { m = 0; return false; }
    if (n == 1)
    {
        m = 0;
        if (REAL_EQ(m_InstList[m].time, t)) return true;
        else if (m_InstList[m].time < t) m = 1;
        return false;
    }

    for (m = (i0+i1)/2; i0 < i1; m = (i0+i1)/2)
    {
        const Instant &inst = m_InstList[m];
        if (REAL_EQ(inst.time, t))
        {
            return true;
        }
        else if (inst.time < t)
        {
            i0 = m + 1;
        }
        else    // inst.time > t
        {
            i1 = m;
        }
    }
    return false;
}


/*
 * <Summary>
 * The default traits for QG_CD_001 that serves as a demo
 * </Summary>
 */

struct QG_CD_001_DefTraits
{
    typedef QG_CD_001_ClashList<>   ClashList;
    typedef ClashList::ObjList      MovingList;
    typedef ClashList::ObjList      StillList;
    typedef ClashList::ObjItr       ObjItr;
    typedef ClashList::Time         Time;
};


/*
 * <summary>
 * This algorithm finds the next clash(es) in the system
 * in a straightforward way and may preserve the 
 * best completeness.
 * It was created on my own in a very intuitive manner.
 * </summary>
 */
template <class Traits = QG_CD_001_DefTraits>
struct QG_CD_001
{
    typedef typename Traits::ClashList  ClashList;
    typedef typename Traits::MovingList MovingList;
    typedef typename Traits::StillList  StillList;
    typedef typename Traits::ObjItr     ObjItr;
    typedef typename Traits::Time       Time;

    QG_CD_001 ();
    ~QG_CD_001 ();

    void BindClashList ( ClashList *pCl );

    /*
     * <Summary>
     * Requirements: all the three pointers are set
     * </Summary>
     */
    void NextClashes ();

protected:
    /*
     * The earliest clashes in this list must take place
     * Any of the later clashes may occur or not
     * Possible of later clashes may occur between any of
     * the later clashes
     */
    ClashList   *m_pCl;

};  /* struct QG_CD_001 */

template <class Traits>
QG_CD_001<Traits>::QG_CD_001 ()
    : m_pCl(0)
{
}

template <class Traits>
QG_CD_001<Traits>::~QG_CD_001 ()
{
}

template <class Traits>
void QG_CD_001<Traits>::BindClashList ( ClashList *pCl )
{
    m_pCl = pCl;
}

template <class Traits>
void QG_CD_001<Traits>::NextClashes ()
{
    ObjItr  pSt, pM1, pM2;
    for ( pM1 = m_pCl->FirstMoving() ; pM1 ; pM1 = m_pCl->NextMoving(pM1) )
    {
        Time    t;
        Time    minTime;
        /*
         * pM1 least hit time derived in the past
         */
        m_pCl->ClashTimeSofar(pM1, /*out*/ minTime);

        // go through all still objects
        for ( pSt = m_pCl->FirstStill(); pSt; pSt = m_pCl->NextStill(pSt))
        {
            bool bClash = m_pCl->TryClash(pM1, pSt, /*out*/t);
            // if pSt movable, pSt must not be hit before `t' 
            // according to former calculation

            if (bClash && t <= minTime)
            {
                if (t < minTime)
                {
                    /*
                     * This subroutine is expected to disconnect all objects previously connected to PM1
                     */
                    m_pCl->CancelConnections(pM1);
                    minTime = t;
                }
                m_pCl->AttemptConnection(pM1, pSt, t);
            }
        }

        // go through all remaining moving objects
        for ( pM2 = m_pCl->NextMoving(pM1); pM2; pM2 = m_pCl->NextMoving(pM2))
        {
            bool bClash = m_pCl->TryClash(pM1, pM2, t);
            if (bClash && t <= minTime)
            {
                if (t < minTime)
                {
                    m_pCl->CancelConnections(pM1);
                    minTime = t;
                }
                m_pCl->AttemptConnection(pM1, pM2, t);
            }
        }
        
        // anything now attached to pM1 renounces its relationship with any objects
        // later than `minTime'
        m_pCl->AcceptConnections(pM1);

    }
}   // void QG_CD_001::NextClashes ()


}   /* namespace qgl */


#endif  /* ifndef _collision_detection_H */


