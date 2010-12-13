/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : fill.c
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     xxxx-xx-xx  <Persons>   Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include <malloc.h>

#include "qgl.h"

/*---------------------------------- Macros ----------------------------------*/

/* .. Edge filling */

#define HASH_SIZE   13

/*---------------------------------- Types -----------------------------------*/

/*
 * .. Edge filling 
 */

typedef struct _Edge
 {
     QglInt     yBottom;

     /* Initial `x' */
     QglInt     xInit;

     /* Current `x' */
     QglInt     x;

     /* Accumulator */
     QglInt     acc;

     /* Carry the slope */
     QglSint    dx;
     QglSint    dy;

     struct _Edge *pNext;
 } Edge;

 typedef struct _EdgeTable
 {
     QglInt             yTop;
     Edge               *pFirstEdge;
     struct _EdgeTable *pNext;  /* For hash collision */
 } EdgeTable;

 typedef EdgeTable *EtHash;

 typedef enum _Comp
 {
     k_Lt = -1,
     k_Eq = 0,
     k_Gt = 1
 } CompRes;

 /* 
  * .. Seed filling 
  */

 typedef struct _VertexUnit
 {
     QglVertex2i        vertex;
     struct _VertexUnit *pNext;
 } VertexUnit;

 typedef struct _VertexStack
 {
     VertexUnit *pTop;
 } VertexStack;

/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/* 
 * .. Edge filling 
 */

static EdgeTable *g_EtHash[HASH_SIZE];

/* 
 * .. Seed filling 
 */


/*----------------------------- Static Functions -----------------------------*/

/* 
 * .. Edge filling 
 */

/*******************************************************************************
 * Prototype    : static EdgeTable * SearchHash (EdgeTable **ppEtHash, 
 *                                               QglInt yTop)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static EdgeTable * SearchHash (EdgeTable **ppEtHash, QglInt yTop)
{
    QglInt      p;
    EdgeTable   *pEt;

    p = yTop % HASH_SIZE;
    pEt = ppEtHash[p];

    while (pEt != NULL && pEt->yTop != yTop)
    {
        pEt = pEt->pNext;
    }
    return pEt;
}

/*******************************************************************************
 * Prototype    : static EdgeTable * PopHash (EdgeTable **ppEtHash, QglInt yTop)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static EdgeTable * PopHash (EdgeTable **ppEtHash, QglInt yTop)
{
    QglInt      p;
    EdgeTable   *pEt;
    EdgeTable   **ppPrevNext;

    p  = yTop % HASH_SIZE;
    pEt = ppEtHash[p];
    ppPrevNext = ppEtHash + p;

    while (pEt != NULL)
    {
        if (pEt->yTop == yTop)
        {
            *ppPrevNext = pEt->pNext;
            break;
        }
        ppPrevNext = &(pEt->pNext);
        pEt = pEt->pNext;
    }
    return pEt;
}

/*******************************************************************************
 * Prototype    : static void InsertHash (EdgeTable **ppEtHash, EdgeTable *pNew)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void InsertHash (EdgeTable **ppEtHash, EdgeTable *pNew)
{
    QglInt      yTop;
    QglInt      p;
    EdgeTable   **ppEt;

    yTop = pNew->yTop;
    p = yTop % HASH_SIZE;
    ppEt = ppEtHash + p;

    while (*ppEt != NULL)
    {
        ppEt = &((*ppEt)->pNext);
    }

    *ppEt = pNew;
}

/*******************************************************************************
 * Prototype    : static CompRes CompareEdge (Edge *edge0, Edge *edge1)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static CompRes CompareEdge (Edge *edge0, Edge *edge1) 
{
    if (edge0->x < edge1->x)
    {
        return k_Lt;
    }
    else if (edge0->x > edge1->x)
    {
        return k_Gt;
    }
    else /* pNew->x == pEdge->x */
    {
        QglSint left;
        QglSint right;

        if (edge0->dx < 0)
        {
            if (edge1->dx >= 0)
            {
                return k_Lt;
            }
        }
        else if (edge0->dx == 0)
        {
            if (edge1->dx > 0)
            {
                return k_Lt;
            }
            else if (edge1->dx < 0)
            {
                return k_Gt;
            }
            else
            {
                return k_Eq;    /* Is it possible? */
            }
        }
        else
        {
            if (edge1->dx <= 0)
            {
                return k_Gt;
            }
        }

        left = edge0->dy * edge1->dx;
        right = edge1->dy * edge0->dx;

        if (left > right)
        {
            return k_Lt;
        }
        else if (left < right)
        {
            return k_Gt;
        }
        else
        {
            return k_Eq;    /* Is it possible? */
        }
    }
}

/*******************************************************************************
 * Prototype    : static void AddEdge (EdgeTable **etHash, QglInt yTop, 
 *                                     QglInt yBottom, QglInt xTop, 
 *                                     QglInt xBottom)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void AddEdge (EdgeTable **etHash, QglInt yTop, QglInt yBottom, 
                     QglInt xTop, QglInt xBottom)
{
    EdgeTable   *pEt;

    QglBool     insAhead;

    Edge        *pNew;
    Edge        *pEdge;
    Edge        **ppPrevNext;

    pEt = SearchHash(etHash, yTop);
    if (pEt == NULL)
    {
        pEt = (EdgeTable *)malloc(sizeof(EdgeTable));
        pEt->yTop = yTop;
        pEt->pFirstEdge = NULL;
        pEt->pNext = NULL;
        InsertHash(etHash, pEt);
    }

    pEdge = pEt->pFirstEdge;
    ppPrevNext = &(pEt->pFirstEdge);

    pNew = (Edge *)malloc(sizeof(Edge));
    pNew->x = xTop;
    pNew->xInit = xTop;
    pNew->acc = 0;
    pNew->yBottom = yBottom;
    pNew->dx = xBottom - xTop;
    pNew->dy = yBottom - yTop;

    while (1)
    {
        if (pEdge == NULL)
        {
            /* Insert the new edge at the end */
            *ppPrevNext = pNew;
            pNew->pNext = NULL;
            break;
        }

        insAhead = (CompareEdge(pNew, pEdge) == k_Lt);

        if (insAhead)
        {   /* Insert the new edge ahead */
            *ppPrevNext = pNew;
            pNew->pNext = pEdge;
            break;
        }
        ppPrevNext = &(pEdge->pNext);
        pEdge = pEdge->pNext;
    }

}

/*******************************************************************************
 * Prototype    : static void CreateEdgeTable (const QglPolygoni *polygon, 
 *                                             EdgeTable **etHash, 
 *                                             QglInt *yMin, QglInt *yMax)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void CreateEdgeTable (const QglPolygoni *polygon, EdgeTable **etHash, 
                             QglInt *yMin, QglInt *yMax)
{
    QglInt i;
    QglInt x0, y0, x1, y1;
    QglInt yTop, yBottom;
    QglInt xTop, xBottom;

    QglInt count = polygon->count;

    *yMax = *yMin = polygon->vertices[0].y;

    x1 = polygon->vertices[count - 1].x;
    y1 = polygon->vertices[count - 1].y;

    for (i = 0; i < count; i++)
    {
        x0 = x1;
        y0 = y1;

        x1 = polygon->vertices[i].x;
        y1 = polygon->vertices[i].y;

        if (*yMax < y0)
        {
            *yMax = y0;
        }
        else if (*yMin > y0)
        {
            *yMin = y0;
        }

        if (y0 > y1)
        {
            xTop = x1;
            yTop = y1;
            xBottom = x0;
            yBottom = y0;
        }
        else if (y0 < y1)
        {
            xTop = x0;
            yTop = y0;
            xBottom = x1;
            yBottom = y1;
        }
        else
        {   /* Horizontal line */
            continue;
        }

        AddEdge(etHash, yTop, yBottom, xTop, xBottom);

    }
}

/*******************************************************************************
 * Prototype    : static void Merge (Edge **ppAet, EdgeTable *pEt)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void Merge (Edge **ppAet, EdgeTable *pEt)
{
    Edge **ppPrevNext = ppAet;
    Edge *pEdge = *ppAet;
    Edge *pEdgeEt = pEt->pFirstEdge;

    while (pEdgeEt != NULL)
    {
        if (pEdge == NULL || CompareEdge(pEdgeEt, pEdge) == k_Lt)
        {
            pEt->pFirstEdge = pEdgeEt->pNext;

            *ppPrevNext = pEdgeEt;
            pEdgeEt->pNext = pEdge;

            ppPrevNext = &(pEdgeEt->pNext);

            pEdgeEt = pEt->pFirstEdge;
        }
        else
        {
            ppPrevNext = &(pEdge->pNext);
            pEdge = pEdge->pNext;
        }
    }

    free(pEt);  /* It will never be used */

}

/*******************************************************************************
 * Prototype    : static void ReorderEdges (Edge **ppAet)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void ReorderEdges (Edge **ppAet)
{
    Edge **ppPrevNext = ppAet;
    Edge *pEdge0 = *ppAet;
    Edge *pEdge1 = pEdge0->pNext;

    /* Very strict assertion that AET is always nonempty and contains even number of elements.*/
    do
    {
        if (pEdge0->x > pEdge1->x)
        {
            *ppPrevNext = pEdge1;
            pEdge0->pNext = pEdge1->pNext;
            pEdge1->pNext = pEdge0;
            ppPrevNext = &(pEdge1->pNext);
            pEdge1 = pEdge0->pNext;
        }
        else
        {
            ppPrevNext = &(pEdge0->pNext);
            pEdge0 = pEdge1;
            pEdge1 = pEdge1->pNext;
        }
    } while (pEdge1 != NULL);
}

/*******************************************************************************
 * Prototype    : static void RefreshEdge (Edge *pEdge)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void RefreshEdge (Edge *pEdge)
{
    QglSint dx, dy;
    QglInt  thr;
    QglBool xForward = k_True;

    dx = pEdge->dx;
    dy = pEdge->dy; /* Always positive */

    if (dx < 0)
    {
        xForward = k_False;
        dx = -dx;
    }

    if (dx > dy)
    {   /* Bresenham scan with `x' */
        QglSint xInc;

        pEdge->acc += dx;
        xInc = pEdge->acc / dy;

        pEdge->x = pEdge->xInit;

        if (!xForward)
        {
            xInc = -xInc;
        }
        pEdge->x += xInc;
    }
    else
    {   /* Bresenham scan with `y' */
        thr = dy / 2;

        pEdge->acc += dx;
        if (pEdge->acc > thr)
        {
            if (xForward)
            {
                pEdge->x++;
            }
            else
            {
                pEdge->x--;
            }
            pEdge->acc -= dy;
        }
    }
}

/*******************************************************************************
 * Prototype    : static void ClearAet (Edge *pAet)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void ClearAet (Edge *pAet)
{
    while (pAet != NULL)
    {
        Edge *pEt = pAet;
        pAet = pAet->pNext;
        free(pEt);
    }
}

/* 
 * .. Seed filling 
 */

/*******************************************************************************
 * Prototype    : static void StackInit (VertexStack *stack)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void StackInit (VertexStack *stack)
{
    stack->pTop = NULL;
}

/*******************************************************************************
 * Prototype    : static void StackPush (VertexStack *stack, 
 *                                       const QglVertex2i *vertex)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void StackPush (VertexStack *stack, const QglVertex2i *vertex)
{
    VertexUnit *pNew = (VertexUnit *)malloc(sizeof(VertexUnit));

    pNew->vertex = *vertex;

    pNew->pNext = (stack->pTop != NULL)? stack->pTop : NULL;

    stack->pTop = pNew;
}

/*******************************************************************************
 * Prototype    : static QglBool StackTop (VertexStack *stack, 
 *                                         QglVertex2i *vertex)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static QglBool StackTop (VertexStack *stack, QglVertex2i *vertex)
{
    QglBool notEmpty = (stack->pTop != NULL);
    if (notEmpty && vertex != NULL)
    {
        *vertex = stack->pTop->vertex;
    }
    return notEmpty;
}

/*******************************************************************************
 * Prototype    : static void StackPop (VertexStack *stack)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void StackPop (VertexStack *stack)
{
    if (stack->pTop != NULL)
    {
        VertexUnit *pDel = stack->pTop;
        stack->pTop = pDel->pNext;
        free(pDel);
    }
}

/*---------------------------- External Functions ----------------------------*/

/*------------------------------ End of Module -------------------------------*/

/* 
 * .. Edge filling 
 */
#if 0
/*******************************************************************************
 * Prototype    : void Qgl_PolygonFill (const QglPolygoni *polygon, 
 *                                      QglPixie *pixie)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
void Qgl_PolygonFill (const QglPolygoni *polygon, QglPixie *pixie)
{
    QglInt      y, yNext, yMin, yMax;
    QglInt      count = polygon->count;
    EdgeTable   *etHash[HASH_SIZE] = {0};
    Edge        *pAet = NULL;


    CreateEdgeTable(polygon, etHash, &yMin, &yMax);

    for (y = yMin; y <= yMax; y = yNext)
    {
        EdgeTable *pEt;
        Edge *pEdge0;
        Edge *pEdge1;
        Edge **ppPrevNext;

        yNext = y + 1;

        /* .. Check the edge table */
        
        pEt = PopHash(etHash, y);
        
        if (pEt != NULL)
        {
            Merge(&pAet, pEt);
        }
        else
        {
            ReorderEdges(&pAet);
        }
        
        pEdge0 = pAet;  /* AET is not expected to be empty */
        ppPrevNext = &pAet;

        do
        {
            QglInt  x, xMin, xMax;

            pEdge1 = pEdge0->pNext;

            xMin = pEdge0->x;
            xMax = pEdge1->x;

            for (x = xMin; x <= xMax; x++)
            {
                pixie->set(pixie, x, y);
            }

            if (pEdge0->yBottom == yNext && yNext != yMax)
            {
                /* Remove edge 0 from AET */
                *ppPrevNext = pEdge0->pNext;
                free(pEdge0);
            }
            else
            {
                ppPrevNext = &(pEdge0->pNext);
                RefreshEdge(pEdge0);
            }

            if (pEdge1->yBottom == yNext && yNext != yMax)
            {
                /* Remove edge 1 from AET */
                *ppPrevNext = pEdge1->pNext;
                free(pEdge1);
            }
            else
            {
                ppPrevNext = &(pEdge1->pNext);
                RefreshEdge(pEdge1);
            }

            pEdge0 = *ppPrevNext;

        } while(pEdge0 != NULL);
    }

    /* ET is expected to be empty here. */

    /* Clear AET */
    ClearAet(pAet);
}
#endif

/* 
 * .. Seed filling 
 */

/*******************************************************************************
 * Prototype    : void Qgl_SeedFill (QglSmartPixie *pixie, QglInt x, QglInt y)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : Set filling with signal [:k_QglSeedSetFill:], 
 *                get filled with signal [:k_QglSeedGetFilled:] (unused), 
 *                get border with signal [:k_QglSeedGetBorder:], 
 *                get either with signal [:k_QglSeedGetEither:].
 ******************************************************************************/
void Qgl_SeedFill (QglSmartPixie *pixie, QglInt x, QglInt y)
{
    enum
    {
        k_Dummy = 0
    };

    QglInt      x0, xl, xr;
    QglVertex2i v;
    VertexStack stack;
    QglInt      i;
    QglBool     found;

    
    StackInit(&stack);

    v.x = x; 
    v.y = y;
    StackPush(&stack, &v);


    while (StackTop(&stack, &v))
    {
        StackPop(&stack);

        x = v.x;
        y = v.y;
        pixie->set(pixie, x, y, k_QglSeedSetFill, k_Dummy);

        x0 = x + 1;
        while (!pixie->get(pixie, x0, y, k_QglSeedGetBorder))
        {
            pixie->set(pixie, x0, y, k_QglSeedSetFill, k_Dummy);
            x0++;
        }
        xr = x0 - 1;    /* Rightmost pixel */
        
        x0 = x - 1;
        while (!pixie->get(pixie, x0, y, k_QglSeedGetBorder))
        {
            pixie->set(pixie, x0, y, k_QglSeedSetFill, k_Dummy);
            x0--;
        }
        xl = x0 + 1;    /* Leftmost pixel */
    
        y = y + 1;
        for (i = 0; i < 2; i++, y -= 2)
        {
            x0 = xl;

            while (x0 <= xr)
            {
                found = k_False;

                while (1)
                {
                    if (pixie->get(pixie, x0, y, k_QglSeedGetEither))
                    {
                        break;
                    }

                    found = k_True;

                    if (x0 < xr)
                    {
                        x0++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (found)
                {
                    v.y = y;
                    v.x = x0;
                    if (pixie->get(pixie, x0, y, k_QglSeedGetEither))
                    {
                        v.x--;
                    }
                    StackPush(&stack, &v);
                }

                do
                {
                    x0++;
                } while (pixie->get(pixie, x0, y, k_QglSeedGetEither) && x0 <= xr);

            }
        }
    }
}

/*------------------------------ End of Module -------------------------------*/
