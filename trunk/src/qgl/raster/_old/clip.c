/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : clip.c
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

#define LINE_CLIP_COHEN_SUTHERLAND  0
#define LINE_CLIP_LIANG_BARSKEY     1   
#define LINE_CLIP_MODE              LINE_CLIP_LIANG_BARSKEY


/*---------------------------------- Types -----------------------------------*/

/* 
 * .. Line Clipping
 */

/* 
 * .. Polygon Clipping
 */

/*    Sutherland - Hodgeman */

typedef struct _Edge
 {
     QglVertex2f    v0;
     QglVertex2f    v1;
 } Edge;

/*-------------------------------- Constants ---------------------------------*/

/* 
 * .. Line Clipping 
 */

#if LINE_CLIP_MODE == LINE_CLIP_COHEN_SUTHERLAND

/* .... Cohen - Sutherland */

enum
{
    k_CsLeft    = 1,
    k_CsRight   = 2,
    k_CsTop     = 4,
    k_CsBottom  = 8
};


#elif LINE_CLIP_MODE == LINE_CLIP_LIANG_BARSKEY

/* .... Liang - Barskey */

#else

#   error "Bad line mode!"

#endif

/* 
 * .. Polygon Clipping
 */


/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Static Functions -----------------------------*/

/* 
 * .. Line Clipping 
 */

#if LINE_CLIP_MODE == LINE_CLIP_COHEN_SUTHERLAND

/* .... Cohen - Sutherland */

/*******************************************************************************
 * Prototype    : static QglInt CsEncode (QglReal *x, QglReal *y, 
 *                                        QglReal *xL, QglReal *xR, 
 *                                        QglReal *yT, QglReal *yB)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : [:QglInt:] type.
 * Annotation   : N/A
 ******************************************************************************/

static QglInt CsEncode (QglReal *x, QglReal *y, QglReal *xL, QglReal *xR, 
                        QglReal *yT, QglReal *yB)
{
    QglInt c = 0;

    if (*x < *xL)
    {
        c |= k_CsLeft;
    }
    else if (*x > *xR)
    {
        c |= k_CsRight;
    }

    if (*y < *yT)
    {
        c |= k_CsTop;
    }
    else if (*y > *yB)
    {
        c |= k_CsBottom;
    }

    return c;
}

#elif LINE_CLIP_MODE == LINE_CLIP_LIANG_BARSKEY

/* .... Liang - Barskey */

 /*******************************************************************************
 * Prototype    : static QglBool LbClipTest (QglReal p, QglReal q, 
 *                                           QglReal *u0, QglReal *u1)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *  p           QglReal        in
 *  q           QglReal        in
 *  u0          QglReal *      out
 *  u1          QglReal *      out
 *  
 * Return Value : [:QglBool:] type.
 * Annotation   : N/A
 ******************************************************************************/
static QglBool LbClipTest (QglReal p, QglReal q, QglReal *u0, QglReal *u1)
{ 
    QglReal r;

    if (p < 0)
    { 
        r = q / p;

        if (r > *u1)
        {
            return k_False;
        }
        else if (r > *u0)
        { 
            *u0 = r;
            return k_True;
        }
    }
    else if (p > 0)
    { 
        r = q / p;

        if (r < *u0)
        {
            return k_False;
        }
        else if (r < *u1)
        { 
            *u1 = r;
            return k_True;
        }
    }
    else if (q < 0)
    {
        return k_False;
    }

    return k_True;
}

#else

#   error "Bad line mode!"

#endif


/* 
 * .. Polygon Clipping
 */

/*    Sutherland - Hodgeman */

/*******************************************************************************
 * Prototype    : static void Intersect (const QglVertex2f *vS,
 *                                       const QglVertex2f *vP,
 *                                       const Edge *edge, QglVertex2f *vI)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void Intersect (const QglVertex2f *vS, const QglVertex2f *vP, 
                       const Edge *edge, QglVertex2f *vI)
{
    if (edge->v0.y == edge->v1.y)
    {   /* Horizontal */
        vI->x = vS->x + (edge->v0.y - vS->y) * (vP->x - vS->x) / (vP->y - vS->y);
        vI->y = edge->v0.y;
    }
    else
    {   /* Vertical */
        vI->x = edge->v1.x;
        vI->y = vS->y + (edge->v0.x - vS->x) * (vP->y - vS->y) / (vP->x - vS->x);
    }
}

/*******************************************************************************
 * Prototype    : static QglBool IsInside (const QglVertex2f *test, 
 *                                         const Edge *edge)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static QglBool IsInside (const QglVertex2f *test, const Edge *edge)
{
    /* [:edge:] is one of the boundaries of an counter-clockwise rectangle */
    if (edge->v0.x < edge->v1.x)
    {   /* bottom */
        return (test->y < edge->v0.y);
    }
    else if (edge->v0.x > edge->v1.x)
    {   /* top */
        return (test->y > edge->v0.y);
    }
    else if (edge->v0.y < edge->v1.y)
    {   /* left */
        return (test->x > edge->v0.x);
    }
    else /* edge->v0.y > edge->v1.y */
    {   /* right */
        return (test->x < edge->v0.x);
    }

    return k_False;
}

/*******************************************************************************
 * Prototype    : static void ShPolygonClipOnEdge (const QglPolygonf *polygonIn, 
 *                                                 QglPolygonf *polygonOut, 
 *                                                 const Edge *edge)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/
static void ShPolygonClipOnEdge (const QglPolygonf *polygonIn, 
                                 QglPolygonf *polygonOut, const Edge *edge)
{
    QglVertex2f *vS, *vP, vI;

    QglInt      i;
    QglInt      countIn = polygonIn->count;
    QglInt      countOut = 0;

    vS = polygonIn->vertices + countIn - 1;
    for (i = 0; i < countIn; i++)
    {
        vP = polygonIn->vertices + i;

        if (IsInside(vP, edge))
        {
            if (IsInside(vS, edge))
            {   /* Both S, P are inside */
                polygonOut->vertices[countOut++] = *vP;
            }
            else
            {   /* Only P is inside */
                Intersect(vS, vP, edge, &vI);
                polygonOut->vertices[countOut++] = vI;
                polygonOut->vertices[countOut++] = *vP;
            }
        }
        else if (IsInside(vS, edge))
        {   /* Only S is inside */
            Intersect(vS, vP, edge, &vI);
            polygonOut->vertices[countOut++] = vI;
        }

        vS = vP;

    }
    polygonOut->count = countOut;

}

/*---------------------------- External Functions ----------------------------*/

/* 
 * .. Line Clipping 
 */

/*******************************************************************************
 * Prototype    : QglBool Qgl_LineClip (QglReal *x0, QglReal *y0,  
 *                                      QglReal *x1, QglReal *y1, 
 *                                      QglReal xL, QglReal xR, 
 *                                      QglReal yT, QglReal yB)
 * Description  : Line clipping with several different implementations.
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/

#if LINE_CLIP_MODE == LINE_CLIP_COHEN_SUTHERLAND

/* .... Cohen - Sutherland */

QglBool Qgl_LineClip (QglReal *x0, QglReal *y0, QglReal *x1, QglReal *y1, 
                      QglReal xL, QglReal xR, QglReal yT, QglReal yB)
{
    QglInt      code0, code1, code;
    QglReal    x, y;
        
    code0 = CsEncode(x0, y0, &xL, &xR, &yT, &yB);
    code1 = CsEncode(x1, y1, &xL, &xR, &yT, &yB);
    
    while (code0 != 0 ||code1 != 0)
    { 
        if ((code0 & code1) != 0)
        {
            return k_False;
        }
    
        code = code0;
    
        if (code0 == 0)
        {
            code = code1;
        }
    
        if (code & k_CsLeft)
        { 
            x = xL;
            y = *y0 + (*y1 - *y0) * (xL - *x0) / (*x1 - *x0);
        }
        else if (code & k_CsRight)
        { 
            x = xR;
            y = *y0 + (*y1 - *y0) * (xR - *x0) / (*x1 - *x0);
        }
        else if (code & k_CsBottom)
        { 
            y = yB;
            x = *x0 + (*x1 - *x0) * (yB - *y0) / (*y1 - *y0);
        }
        else if (code & k_CsTop)
        { 
            y = yT;
            x = *x0 + (*x1 - *x0) * (yT - *y0) / (*y1 - *y0);
        }
    
        if (code == code0)
        {  
            *x0 = x;
            *y0 = y; 
            code0 = CsEncode(&x, &y, &xL, &xR, &yT, &yB);
        }
        else
        { 
            *x1 = x;
            *y1 = y; 
            code1 = CsEncode(&x, &y, &xL, &xR, &yT, &yB);
        }
    }

    return k_True;
}

#elif LINE_CLIP_MODE == LINE_CLIP_LIANG_BARSKEY

/* .... Liang - Barskey */

QglBool Qgl_LineClip (QglReal *x0, QglReal *y0, QglReal *x1, QglReal *y1,
        QglReal xL, QglReal xR, QglReal yT, QglReal yB)
{ 
    QglReal dx, dy, u0, u1;

    dx = *x1 - *x0;
    dy = *y1 - *y0;

    u0 = 0;
    u1 = 1;

    if (LbClipTest(-dx, *x0 - xL, &u0, &u1))
    {
       if (LbClipTest(dx, xR - *x0, &u0, &u1))
       {
           if (LbClipTest(-dy, *y0 - yT, &u0, &u1))
           {
               if (LbClipTest(dy, yB - *y0, &u0, &u1))
               {
                   *x1 = *x0 + u1 * dx;
                   *y1 = *y0 + u1 * dy;
                   *x0 += u0 * dx;
                   *y0 += u0 * dy;
                   return k_True;
               }
           }
        }
    }
    return k_False;
}

#else

#   error "Bad mode!"

#endif

/* 
 * .. Polygon Clipping
 */

/*    Sutherland - Hodgeman */



/*******************************************************************************
 * Prototype    : void Qgl_PolygonClip (const QglPolygonf *polygonIn, 
 *                                      QglPolygonf *polygonOut, 
 *                                      QglReal xL, QglReal xR, 
 *                                      QglReal yT, QglReal yB)
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : Notice the fact that the number of vertices of the output 
 *                polygon is no more than double of that of input.
 ******************************************************************************/
void Qgl_PolygonClip (const QglPolygonf *polygonIn, QglPolygonf *polygonOut, 
                      QglReal xL, QglReal xR, QglReal yT, QglReal yB)
{   
    QglPolygonf polygonTemp;
    Edge        eB = {{xL, yB}, {xR, yB}};
    Edge        eR = {{xR, yB}, {xR, yT}};
    Edge        eT = {{xR, yT}, {xL, yT}};
    Edge        eL = {{xL, yT}, {xL, yB}};

    polygonTemp.count = 0;
    polygonTemp.vertices = (QglVertex2f *)alloca(polygonIn->count * 2 * sizeof(QglVertex2f));
    ShPolygonClipOnEdge(polygonIn, &polygonTemp, &eB);
    ShPolygonClipOnEdge(&polygonTemp, polygonOut, &eR);
    ShPolygonClipOnEdge(polygonOut, &polygonTemp, &eT);
    ShPolygonClipOnEdge(&polygonTemp, polygonOut, &eL);

}

/*------------------------------ End of Module -------------------------------*/


