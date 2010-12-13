#include "mld_memoper.h"


/*
 *  pprev              32
 *  allocated_size     32, with 1st bit being 1
 *  <allocated section>
 *  pprev              32
 *  unallocated_size   32, with 1st bit being 0
 *  <unallocated section>
 * 
 * <notations>
 *  blocksize(n) -- size of the block excluding block header 
 * </notations>
 * <remarks>
 *  as allocated memory of size 0 is not allowed,
 *  we can use a header inidicating that as an EOM
 * </remarks>
 */

#define IS_ALLOCATED(n)     ((n)&0x80000000)
#define GET_SIZE(n)         ((n)&0x7fffffff)
#define ALLOC_SIZE(n)       ((n)|0x80000000)
#define UNALLOC_SIZE(n)     (n)
#define IS_EOM(n)           ((n)==0x80000000)

#define HDRSIZE             2
#define PPREVIDX            0
#define SSIZEIDX            1

#define MIN(a,b)            (((a)<(b))?(a):(b))



static U32      g_nPoolSize;
static U32Ptr   g_pPool;

/*
 * <summary>
 *  This routine copies nLen bytes from block started at pSrc 
 *  to block started at pDst regardless of the order of their 
 *  addresses
 * </summary>
 */
static void MemMove(U32Ptr pDst, U32Ptr pSrc, U32 nLen)
{
    if (pDst < pSrc)
    {   /* copy from tail to head */
        U32Ptr pDstEnd = pDst + nLen;
        for ( ; pDst < pDstEnd; )
        {
            *pDst++ = *pSrc++;
        }
    }
    else if (pSrc > pDst)
    {   /* copy from head to tail */
        U32Ptr pD = pDst + nLen - 1;
        U32Ptr pS = pSrc + nLen - 1;
        for ( ; pD >= pDst; )
        {
            *pD-- = *pS--;
        }
    }
}

/*
 * <summary>
 *  Change the allocated memory into a smaller size
 * </summary>
 */
void Shrink(U32Ptr pCurr, U32Ptr pNext, U32 nSizeAfter, U32 nCurrSize, U32 nSize)
{
    if (nSizeAfter > 0)
    {
        U32Ptr pNext2;
        /*
         *  before shrinking
         *  pCurr               pNext       pNext2
         *    | CurrBlock         |  Unalloc  |
         *  after shrinking
         *  pCurr        pNext              pNext2
         *    | CurrBlock  |     Unalloc      |
         */
        pCurr[SSIZEIDX] = ALLOC_SIZE(nSize);

        pNext = pCurr + nSize + HDRSIZE;
        pNext[PPREVIDX] = pCurr - g_pPool;
        pNext[SSIZEIDX] = UNALLOC_SIZE(nSizeAfter - (nCurrSize - nSize));
        
        pNext2 = pNext + GET_SIZE(pNext[SSIZEIDX]) + HDRSIZE;
        pNext2[PPREVIDX] = pNext - g_pPool;
    }
    else
    {
        U32Ptr pNewNext;
        /*
         *  before shrinking
         *  pCurr                  pNext       
         *    | CurrBlock            |  Used  |
         *  after shrinking
         *  pCurr      pNewNext    pNext
         *    | CurrBlock  | Unused  |  Used  |
         *     (`Unused' exists only when bigger than header)
         */
        if (nSize >= nCurrSize - HDRSIZE)
        {
            return;     // unchanged
        }

        pNewNext = pCurr + HDRSIZE + nSize;
        pNewNext[PPREVIDX] = pCurr - g_pPool;
        pNewNext[SSIZEIDX] = UNALLOC_SIZE(nCurrSize - nSize - HDRSIZE);

        pNext[PPREVIDX] = pNewNext - g_pPool;

        pCurr[SSIZEIDX] = ALLOC_SIZE(nSize);
    }
}

/*
 * <summary>
 *  resize current block by obtaining memory from 
 *  succeeding unallocated part which is sufficient
 *   nAllocable = blocksize(CurrBlock) + blocksize(Unalloc) + HDRSIZE
 *   before resizing: 
 *   pCurr         pNext            pNext2
 *      | CurrBlock  |  Unalloc       |
 *   after resizing:
 *      |    CurrBlock      | Unalloc |
 *                   (exists only if big enough)
 * </summary>
 */

static void Expand1(U32Ptr pCurr, U32Ptr pNext, 
    U32 nSize, U32 nAllocable)
{
    U32Ptr pNext2 = pNext + GET_SIZE(pNext[SSIZEIDX]) + HDRSIZE;

    if (nSize + HDRSIZE >= nAllocable)
    {
        nSize = nAllocable;

        pNext2[PPREVIDX] = pCurr - g_pPool;
    }
    else
    {
        U32Ptr pNewNext = pCurr + nSize + HDRSIZE;
        pNewNext[PPREVIDX] = pCurr - g_pPool;
        pNewNext[SSIZEIDX] = UNALLOC_SIZE(nAllocable - nSize - HDRSIZE);

        pNext2[PPREVIDX] = pNewNext - g_pPool;
    }
    pCurr[SSIZEIDX] = ALLOC_SIZE(nSize);
}

/*
 * <summary>
 *  resize current block by obtaining memory from both 
 *  precedign and succeeding unallocated parts and the total
 *  amount of allocable space is sufficient
 *   nAllocable = blocksize(PrevBlock) + blocksize(CurrBlock) 
 *                + blocksize(Unalloc) + 4
 *  the routine returns the pointer to the starting address
 *  of the data in the newly allocated block
 *   before resizing: 
 *   pPrev          pCurr                          pNext
 *     | PrevUnalloc  | CurrBlock   |  NextUnalloc   |
 *   after resizing:
 *     |    CurrBlock         |    Unalloc           |
 *                   (exists only if big enough)
 * </summary>
 */

static U32Ptr Expand2(U32Ptr pPrev, U32Ptr pCurr, U32Ptr pNext, U32 nSize, U32 nAllocable)
{
    MemMove(pPrev + HDRSIZE, pCurr + HDRSIZE, GET_SIZE(pCurr[SSIZEIDX]));

    if (nSize + HDRSIZE >= nAllocable)
    {
        nSize = nAllocable;
        pNext[PPREVIDX] = pPrev - g_pPool;
    }
    else
    {
        U32Ptr pUnused = pPrev  + nSize + HDRSIZE;
        pUnused[PPREVIDX] = pPrev - g_pPool;
        pUnused[SSIZEIDX] = UNALLOC_SIZE(nAllocable - nSize - HDRSIZE);

        pNext[PPREVIDX] = pUnused - g_pPool;
    }
    pPrev[SSIZEIDX] = ALLOC_SIZE(nSize);

    return (pPrev + HDRSIZE);
}

/*
 * <summary>
 *  initialize the memory pool
 * </summary>
 */
void mld_memoper_Init(U32Ptr pPool, U32 nPoolSize)
{
    g_nPoolSize = nPoolSize;

    // assigning memory for the pool
    g_pPool = pPool;
    
    // initialize the pool
    g_pPool[PPREVIDX] = 0;
    g_pPool[SSIZEIDX] = UNALLOC_SIZE(g_nPoolSize - HDRSIZE * 2);    // points to the EOM marker

    // end-of-memory marker
    g_pPool[nPoolSize - HDRSIZE + PPREVIDX] = 0;
    g_pPool[nPoolSize - HDRSIZE + SSIZEIDX] = ALLOC_SIZE(0);
}

/*
 * <summary>
 *  deinitialize the memory pool
 * </summary>
 */
void mld_memoper_Deinit(void)
{
    g_nPoolSize = 0;
    g_pPool = NULL;
}

/*
 * <summary>
 *  allocate memory
 *  linear search for unallocated block big enough to fulfill
 *  the allocation
 * </summary>
 */
U32Ptr mld_memoper_Alloc(U32 nSize)
{
    U32 nOffset = 0;
    U32 nPrevOffset = 0;
    U32Ptr p;
    U32Ptr pNext;

    for ( ; ; )
    {
        U32 nSecSizeInd = g_pPool[nOffset + SSIZEIDX];
        U32 nSecSize = GET_SIZE(nSecSizeInd);

        if (IS_EOM(nSecSizeInd))
        {   // EOM reached, memory insufficient (or needs defragment?)
            return NULL;
        }

        if (!IS_ALLOCATED(nSecSizeInd) && nSize <= nSecSize)
        {
            if (nSize >= nSecSize - HDRSIZE)
            {
                nSize = nSecSize;
            }

            p = g_pPool + nOffset;
            p[PPREVIDX] = nPrevOffset;
            p[SSIZEIDX] = ALLOC_SIZE(nSize);
            p += HDRSIZE;

            if (nSize < nSecSize)
            {
                pNext = p + nSize;
                pNext[PPREVIDX] = nOffset;
                pNext[SSIZEIDX] = UNALLOC_SIZE(nSecSize - nSize - HDRSIZE);
            }

            return p;
        }

        nPrevOffset = nOffset;
        nOffset += nSecSizeInd + HDRSIZE;
    }
    return NULL;
}

/*
 * <summary>
 *  reallocate memory
 *  try resize the allocated memory by employing unused memory
 *  around it first, if infeasible call mld_memoper_Alloc to
 *  allocate new memory
 * </summary>
 */
U32Ptr mld_memoper_Realloc(U32Ptr p, U32 nSize)
{
    U32Ptr pCurrHead = ((U32Ptr)p) - HDRSIZE;
    U32Ptr pNew;
    U32Ptr pPrevHead;
    U32Ptr pNextHead;
    U32Ptr pNext2Head;

    U32 nCurrSize;
    U32 nPrev;
    U32 nSizeBefore = 0;
    U32 nSizeAfter = 0;
    U32 nAllocable;
    U32 nNewSize;

    if (p == NULL)
    {
        return mld_Alloc(nSize);
    }


    if (!IS_ALLOCATED(pCurrHead[SSIZEIDX]))
    {   // error, `p' is not a valid starting address
        return NULL;
    }

    nCurrSize = GET_SIZE(pCurrHead[SSIZEIDX]);
    nPrev = pCurrHead[PPREVIDX];

    /* acquire prev header */
    pPrevHead = g_pPool + g_pPool[nPrev];
    // check prev
    if (pCurrHead == g_pPool)
    {
        nSizeBefore = 0;
    }
    else if (pPrevHead >= pCurrHead)
    {
        return NULL;
    }
    if (!IS_ALLOCATED(pPrevHead[SSIZEIDX]))
    {
        nSizeBefore = GET_SIZE(pPrevHead[SSIZEIDX]);
    }

    /* acquire next header */
    pNextHead = pCurrHead + nCurrSize + HDRSIZE;
    if (!IS_ALLOCATED(pNextHead[SSIZEIDX]))
    {
        nSizeAfter = GET_SIZE(pNextHead[SSIZEIDX]);
    }

    /* check `nSize' */
    if (nSize == nCurrSize)
    {   // size unchanged
        return p;
    }
    else if (nSize < nCurrSize)
    {   // shrink
        Shrink(pCurrHead, pNextHead, nSizeAfter, nCurrSize, nSize);
        return p;
    }

    pNew = NULL;
    nAllocable = nCurrSize + nSizeAfter + HDRSIZE;
    if (nSizeAfter > 0 && nAllocable >= nSize)
    {
        Expand1(pCurrHead, pNextHead, nSize, nAllocable);
        pNew = p;
    }
    else if (nSizeBefore > 0)
    {
        if (nSizeAfter > 0)
        {
            nAllocable = nCurrSize + nSizeAfter + nSizeBefore + HDRSIZE * 2;
            if (nAllocable >= nSize)
            {
                pNext2Head = pNextHead + GET_SIZE(pNextHead[SSIZEIDX]) + HDRSIZE;
                pNew = Expand2(pPrevHead, pCurrHead, pNext2Head, nSize, nAllocable);
            }
        }
        else 
        {
            nAllocable = nCurrSize + nSizeBefore + HDRSIZE;
            if (nAllocable >= nSize)
            {
                pNew = Expand2(pPrevHead, pCurrHead, pNextHead, nSize, nAllocable);
            }
        }
    }

    if (pNew == NULL)
    {
        pNew = mld_Alloc(nSize);
        MemMove(pNew, p, MIN(nCurrSize, nNewSize));
    }

    return pNew;
}

/*
 * <summary>
 *  free allocated memory
 * </summary>
 */
int mld_memoper_Free(U32Ptr p)
{
    U32Ptr pCurrHead = ((U32Ptr)p) - HDRSIZE;
    U32Ptr pPrevHead;
    U32Ptr pNextHead;
    U32Ptr pNewNext;
    U32Ptr pMod;

    U32 nSize;
    U32 nUnallocSize;
    U32 nPrev;

    if (!IS_ALLOCATED(pCurrHead[SSIZEIDX]))
    {
        return (-1);
    }

    nSize = GET_SIZE(pCurrHead[SSIZEIDX]);
    nPrev = pCurrHead[PPREVIDX];

    pPrevHead = g_pPool + g_pPool[nPrev];

    // check prev
    if (pPrevHead >= pCurrHead && pCurrHead != g_pPool)
    {
        return (-1);
    }

    if (pCurrHead != g_pPool && !IS_ALLOCATED(pPrevHead[SSIZEIDX]))
    {
        pMod = pPrevHead;
        nUnallocSize = nSize + HDRSIZE + GET_SIZE(pPrevHead[SSIZEIDX]);
    }
    else
    {
        pMod = pCurrHead;
        nUnallocSize = nSize;
    }

    // check mod
    if (pMod < g_pPool)
    {
        return (-1);
    }

    pNextHead = pCurrHead + nSize + HDRSIZE;

    if (IS_ALLOCATED(pNextHead[SSIZEIDX]))
    {
        pNewNext = pNextHead;
    }
    else
    {
        U32 nNextSecSize = HDRSIZE + GET_SIZE(pNextHead[SSIZEIDX]);
        nUnallocSize += nNextSecSize;
        pNewNext = pNextHead + nNextSecSize;
    }

    pMod[SSIZEIDX] = UNALLOC_SIZE(nUnallocSize);

    pNewNext[PPREVIDX] = pMod - g_pPool;

    return (0);
}


