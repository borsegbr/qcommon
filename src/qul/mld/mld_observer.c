#include "mld_observer.h"

#define HASH_SIZE       16
#define HASH_MASK       HASH_SIZE


typedef struct tagMldAllocated  MldAllocated;
struct tagMldAllocated
{
    void    *pAddr;
    U32     nSize;      // size in bytes

    MldAllocated   *pNext;
};

typedef struct tagMldObserver
{
    U32 nAllocated;
    // TODO: ...

    MldAllocated    *hash[HASH_SIZE];
} MldObserver;

MldObserver g_Observer;


/*
 * <summary>
 *  this routine search for item with pAddr in the hashing table
 * </summary>
 */
static MldAllocated ** HashSearch(void *pAddr)
{
    int index = ((U32)pAddr) % HASH_SIZE;

    MldAllocated **pp = &g_Observer.hash[index];
    MldAllocated *p = *pp;

    for ( ; p ; pp = &(p->pNext), p = p->pNext)
    {
        if (p->pAddr == pAddr)
        {
            break;
        }
    }

    return pp;
}

/*
 * <summary>
 *  this routine initialize the observer by zeroing all the variables
 * </summary>
 * <remarks>
 *  the routine and all of the following procedures need mutual exclusion protection
 * </remarks>
 */
int mld_observer_Init(void)
{
    int i;
    if (g_Observer.nAllocated != 0)
    {   /* try deinitialization on the observer with remaining data */
        mld_observer_Deinit();
    }
    g_Observer.nAllocated = 0;
    for (i = 0; i < HASH_SIZE; i++)
    {
        g_Observer.hash[i] = NULL;
    }
    return MLD_RES_SUCCESS;
}

/*
 * <summary>
 *  this routine deinitialize the observer
 *  it deallocates the memory assiged for hash entries and zeros the variables
 * </summary>
 */
int mld_observer_Deinit(void)
{
    int i;
    g_Observer.nAllocated = 0;
    for (i = 0; i < HASH_SIZE; i++)
    {
        MldAllocated *p;
        for ( p = g_Observer.hash[i]; p ; )
        {
            g_Observer.hash[i] = p->pNext;
            sys_free(p);
            p = g_Observer.hash[i];
        }
    }
}

/*
 * <summary>
 *  this routine register the specified memory address from to observer  
 *  with the size of the allocation
 * </summary>
 */
int mld_observer_Register(void *pAddr, U32 nSize)
{
    MldAllocated **pp = HashSearch(pAddr);
    MldAllocated *pNew;
    if (*pp)
    {
        return MLD_RES_POINTER_ALREADY_REGISTERED;
    }

    pNew = (MldAllocated *)sys_malloc(sizeof(MldAllocated));
    pNew->pAddr = pAddr;
    pNew->nSize = nSize;
    pNew->pNext = NULL;

    *pp = pNew;
    g_Observer.nAllocated++;

    return MLD_RES_SUCCESS;
}

/*
 * <summary>
 *  this routine unregister the specified memory address from the observer
 * </summary>
 */
int mld_observer_Unregister(void *pAddr)
{
    MldAllocated **pp = HashSearch(pAddr);
    MldAllocated *pDel = *pp;

    if (pDel == NULL)
    {
        return MLD_RES_NOT_FOUND;
    }
    *pp = pDel->pNext;
    sys_free(pDel);

    g_Observer.nAllocated--;

    return MLD_RES_SUCCESS;
}

/*
 * <summary>
 *  this routine creates a snapshot of current overall memory allocation
 * </summary>
 */
int mld_observer_SnapshotCreate(void **ppSnapshot)
{
    int i;
    MldAllocated *pHead = NULL;

    for (i = 0; i < HASH_SIZE; i++)
    {
        MldAllocated *p = g_Observer.hash[i];
        for ( ; p; p = p->pNext)
        {
            MldAllocated *pNew = (MldAllocated *)sys_malloc(sizeof(MldAllocated));
            pNew->pAddr = p->pAddr;
            pNew->nSize = p->nSize;
            MldAllocated **pp = &pHead;
            /* Performance concern! It is o(n^2) in complexity */
            for ( ; *pp && (*pp)->pAddr < p->pAddr; pp = &(*pp)->pNext)
            {
            }
            if (*pp)
            {
                if ((*pp)->pAddr == p->pAddr)
                {
                    return MLD_RES_DUPLICATED_ADDR;
                }
                pNew->pNext = *pp;
            }
            else
            {
                pNew->pNext = NULL;
            }
            *pp = pNew;
        }
    }
    *ppSnapshot = pHead;
    return MLD_RES_SUCCESS;
}

/*
 * <summary>
 *  this routine retrieve the content of the specified snapshot entry
 * </summary>
 */
int mld_observer_SnapshotVisit(void *p, void **ppAddr, U32 *pSize, void **ppNext)
{
    MldAllocated *pCurr = p;
    if (p == NULL)
    {
        return MLD_RES_NULL_PTR;
    }

    if (ppAddr)
    {
        *ppAddr = pCurr->pAddr;
    }
    if (pSize)
    {
        *pSize = pCurr->nSize;
    }
    if (ppNext)
    {
        *ppNext = pCurr->pNext;
    }

    return MLD_RES_SUCCESS;
}

/*
 * <summary>
 *  this routine destroys the memory allocation snapshot
 * </summary>
 */
int mld_observer_SnapshotDestroy(void *pSnapshot)
{
    MldAllocated *p = pSnapshot;
    for ( ; p; )
    {
        p = p->pNext;
        sys_free(pSnapshot);
        pSnapshot = p;
    }
    return MLD_RES_SUCCESS;
}

