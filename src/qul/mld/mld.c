#include "mld.h"

#include "mld_priv.h"
#include "mld_memoper.h"
#include "mld_observer.h"


BOOL    g_bEnMemOper;
BOOL    g_bEnObserver;
int     g_nRecentErr;


void mld_Init(unsigned long *pPool, unsigned long nPoolSize, int nMode)
{
    U32 nPoolSize32 = nPoolSize / 4;
    if (pPool)
    {
        mld_memoper_Init(pPool, nPoolSize32);
        g_bEnMemOper = TRUE;
    }
    else
    {
        g_bEnMemOper = FALSE;
    }
    if (nMode == MLD_MODE_ENABLE_OBSERVER)
    {
        mld_observer_Init();
        g_bEnObserver = TRUE;
        g_nRecentErr = MLD_RES_SUCCESS;
    }
    else
    {
        g_bEnObserver = FALSE;
    }
}

void mld_Deinit(void)
{
    if (g_bEnMemOper)
    {
        mld_memoper_Deinit();
        g_bEnMemOper = FALSE;
    }
    if (g_bEnObserver)
    {
        mld_observer_Deinit();
        g_bEnObserver = FALSE;
        g_nRecentErr = MLD_RES_SUCCESS;
    }
}

void * mld_Alloc(unsigned long nSize)
{
    U32 nSize32 = (nSize + sizeof(U32) - 1) / sizeof(U32);
    void * pRes = NULL;

    if (g_bEnMemOper)
    {
        pRes = mld_memoper_Alloc(nSize32);
    }
    else
    {
        pRes = sys_malloc(nSize);
    }
    
    if (g_bEnObserver)
    {
        if (g_bEnMemOper)
        {
            nSize = nSize32 * sizeof(U32);
        }
        g_nRecentErr = mld_observer_Register(pRes, nSize);
        if (g_nRecentErr)
        {
            return NULL;
        }
    }

    return pRes;
}

void * mld_Realloc(void *p, unsigned long nSize)
{
    U32 nSize32 = (nSize + sizeof(U32) - 1) / sizeof(U32);
    void * pRes = NULL;

    if (g_bEnObserver)
    {
        g_nRecentErr = mld_observer_Unregister(p);
        if (g_nRecentErr)
        {
            return NULL;
        }
    }

    if (g_bEnMemOper)
    {
        pRes = mld_memoper_Realloc((U32Ptr)p, nSize32);
        nSize = nSize32 * 4;
    }
    else
    {
        pRes = sys_realloc(p, nSize);
    }

    if (g_bEnObserver)
    {
        if (g_bEnMemOper)
        {
            nSize = nSize32 * sizeof(U32);
        }
        g_nRecentErr = mld_observer_Register(pRes, nSize);
        if (g_nRecentErr)
        {
            return NULL;
        }
    }
    return pRes;
}

int mld_Free(void *p)
{
    int nRes;

    if (g_bEnObserver)
    {
        g_nRecentErr = mld_observer_Unregister(p);
        if (g_nRecentErr)
        {
            return -1;
        }
    }

    if (g_bEnMemOper)
    {
        nRes = mld_memoper_Free((U32Ptr)p);
    }
    else
    {
        sys_free(p);
        nRes = 0;
    }

    return nRes;
}


/* the following routines are related to observer */

int mld_GetRecentError(void)
{
    if (g_bEnObserver)
    {
        return g_nRecentErr;
    }
    else
    {
        return MLD_RES_OBSERVER_DISABLED;
    }
}

int mld_SnapshotCreate(void **ppSnapshot)
{
    if (g_bEnObserver)
    {
        return mld_observer_SnapshotCreate(ppSnapshot);
    }
    else
    {
        return MLD_RES_OBSERVER_DISABLED;
    }
}

int mld_SnapshotDestroy(void *pSnapshot)
{
    return mld_observer_SnapshotDestroy(pSnapshot);
}

int mld_SnapshotCompare(void *pSnapshot1, void *pSnapshot2)
{
    void *p1 = pSnapshot1;
    void *p2 = pSnapshot2;
    for ( ; p1 && p2; )
    {
        void *pAddr1;
        void *pAddr2;
        U32 nSize1;
        U32 nSize2;
        mld_observer_SnapshotVisit(p1, &pAddr1, &nSize1, &p1);
        mld_observer_SnapshotVisit(p2, &pAddr2, &nSize2, &p2);
        if (pAddr1 > pAddr2)
        {
            return 1;
        }
        if (pAddr1 < pAddr2)
        {
            return -1;
        }
    }

    if (p1)
    {
        return 1;
    }
    if (p2)
    {
        return -1;
    }
    return 0;
}

void mld_SnapshotDisplay(void *pSnapshot, char *pszName)
{
    void *p = pSnapshot;
    void *pAddr;
    U32 nSize;
    if (pszName)
    {
        printf("Snapshot \"%s\" begins\n", pszName);
    }
    else
    {
        printf("Snapshot 0x%08x begins\n", pSnapshot);
    }
    for ( ; p; )
    {
        mld_observer_SnapshotVisit(p, &pAddr, &nSize, &p);
        printf("{ addr = 0x%08x, size = %10u }\n", pAddr, nSize);
    }
    if (pszName)
    {
        printf("Snapshot \"%s\" ends\n", pszName);
    }
    else
    {
        printf("Snapshot 0x%08x ends\n", pSnapshot);
    }
}
