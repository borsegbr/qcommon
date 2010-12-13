/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : line.cpp
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-09  quanben     Created.
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "line.h"

/*---------------------------------- Macros ----------------------------------*/

/* .. Modes */

#define LINE_MIDPOINT       0
#define LINE_BRESENHAM      1

#define CIRCLE_MIDPOINT     0
#define CIRCLE_BRESENHAM    1


/* .. Mode selection switches */

#define LINE_MODE           LINE_MIDPOINT
#define CIRCLE_MODE         CIRCLE_MIDPOINT

/*-------------------------------- Namespaces --------------------------------*/

using namespace qgl;

/*---------------------------------- Types -----------------------------------*/

/* 
 * .. (Straight) Line Drawing
 */

typedef FixedLiner<int>     FixedLiner_Int;
typedef AlterLiner<int>     AlterLiner_Int;

struct LineVisitXy
{
    QglPixie *pixie;

    LineVisitXy (QglPixie *px) : pixie(px)
    {
    }

    void operator() (const FixedLiner_Int &x, const AlterLiner_Int &y)
    {
        pixie->set(pixie->data, x.value, y.value);
    }
};

struct LineVisitYx
{
    QglPixie *pixie;

    LineVisitYx (QglPixie *px) : pixie(px)
    {
    }

    void operator() (const FixedLiner_Int &y, const AlterLiner_Int &x)
    {
        pixie->set(pixie->data, x.value, y.value);
    }
};

/* 
 * .. Circle Drawing
 */

template <class _IncX, class _IncY>
    struct CircleVisitXy4
{
    QglPixie    *pixie;

    QglInt      cx, cy;

    CircleVisitXy4 (QglPixie *px, QglInt x, QglInt y) : pixie(px), cx(x), cy(y)
    {
    }

    void operator() (const _IncX &x, const _IncY &y)
    {
        void *data = pixie->data;
        pixie->set(data, cx + x.value, cy + y.value);
        pixie->set(data, cx - x.value, cy + y.value);
        pixie->set(data, cx + x.value, cy - y.value);
        pixie->set(data, cx - x.value, cy - y.value);
    }
};

template <class _IncY, class _IncX>
    struct CircleVisitYx4
{
    QglPixie    *pixie;

    QglInt      cy, cx;

    CircleVisitYx4 (QglPixie *px, QglInt y, QglInt x) : pixie(px), cy(y), cx(x)
    {
    }

    void operator() (const _IncY &y, const _IncX &x)
    {
        void *data = pixie->data;
        pixie->set(data, cx + x.value, cy + y.value);
        pixie->set(data, cx - x.value, cy + y.value);
        pixie->set(data, cx + x.value, cy - y.value);
        pixie->set(data, cx - x.value, cy - y.value);
    }
};

template <class _IncX, class _IncY>
    struct CircleVisit8
{
    QglPixie    *pixie;

    QglInt      cx, cy;

    CircleVisit8 (QglPixie *px, QglInt x, QglInt y) : pixie(px), cx(x), cy(y)
    {
    }

    void operator() (const _IncX &x, const _IncY &y)
    {
        void *data = pixie->data;
        pixie->set(data, cx + x.value, cy + y.value);
        pixie->set(data, cx + y.value, cy + x.value);
        pixie->set(data, cx - x.value, cy + y.value);
        pixie->set(data, cx - y.value, cy + x.value);
        pixie->set(data, cx + x.value, cy - y.value);
        pixie->set(data, cx + y.value, cy - x.value);
        pixie->set(data, cx - x.value, cy - y.value);
        pixie->set(data, cx - y.value, cy - x.value);
    }
};

#if CIRCLE_MODE == CIRCLE_BRESENHAM

template <int inc>
    struct CircleInc
{
    typedef int _VTp;

    int value;

    CircleInc (int v) : value(v)
    {
    }

    void operator++ ()
    {
        value += inc;
    }

    _VTp delta ()
    {
        return (value * inc * 2 + 1);
    }

};

typedef CircleInc<1>    CircleIncX;
typedef CircleInc<-1>   CircleIncY;

struct CirclePermit
{
    bool operator() (const CircleIncX &x, const CircleIncY &y) const
    {
        return (y.value >= 0);   
    }
};

#elif CIRCLE_MODE == CIRCLE_MIDPOINT

template <int inc>
    struct CircleInc
{
    typedef int _VTp;

    int value;
    int delta_value;

    CircleInc (int v, int d) : value(v), delta_value(d)
    {
    }

    void operator++ ()
    {
        value += inc;
        delta_value += 2;
    }

    _VTp delta ()
    {
        return delta_value;
    }

};

typedef CircleInc<1>    CircleIncX;
typedef CircleInc<-1>   CircleIncY;

struct CirclePermit
{
    bool operator() (const CircleIncX &x, const CircleIncY &y) const
    {
        return (x.value <= y.value);
    }
};

#endif  /* if CIRCLE_MODE == CIRCLE_BRESENHAM */


/* 
 * .. Ellipse Drawing
 */

template <int inc, int d_inc>
    struct EllipseInc : public CircleInc<inc>
{
    int hetero_shape;

    EllipseInc (int v, int d, int hs) : CircleInc<inc>(v, d), hetero_shape(hs*hs)
    {
    }

    void operator++ ()
    {
        value += inc;
        delta_value += 8 * hetero_shape * d_inc;
    }
};

typedef EllipseInc<1, 1>   EllipseIncX;
typedef EllipseInc<-1, 1>   EllipseIncY;

struct EllipsePermit1
{
    bool operator() (const EllipseIncX &x, const EllipseIncY &y) const
    {
        return (2 * x.hetero_shape * (x.value + 1) 
            < y.hetero_shape * (2 * y.value - 1));
    }
};

struct EllipsePermit2
{
    bool operator() (const EllipseIncY &y, const EllipseIncX &x) const
    {
        return (y.value >= 0);
    }
};


struct EllipseJudge2
{
    bool operator() (QglInt d)
    {
        return !(d < 0);
    }
};


/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

/*------------------------- Static Member Functions --------------------------*/

/*------------------- Functions with C Calling Convention --------------------*/

/* 
 * .. (Straight) Line Drawing
 */

void Qgl_Line (QglInt x0, QglInt y0, QglInt x1, QglInt y1, QglPixie *pixie)
{

    QglInt dx = x1 - x0;
    QglInt dy = y1 - y0;

    QglInt yIni, yEnd, xIni, xEnd;

    dx = (dx<0)?-dx:dx;
    dy = (dy<0)?-dy:dy;

    if (dx < dy)
    {

        if (y1 < y0)
        {
            yIni = y1;
            yEnd = y0;
            xIni = x1;
            xEnd = x0;
        }
        else
        {
            yIni = y0;
            yEnd = y1;
            xIni = x0;
            xEnd = x1;
        }

        QglInt xInc = (xEnd < xIni)? -1 : 1;

        LineVisitYx   visit(pixie);
        AlterLiner_Int x(xIni, xInc);
        FixedLiner_Int y(yIni);

#if LINE_MODE == LINE_BRESENHAM    
        QglInt a = 0;
        typedef BresenhamLine<FixedLiner_Int, AlterLiner_Int, int, LineVisitYx> Line;
        Line::Draw(y, x, yEnd, dy*2, dx*2, dy, a, visit);
#elif LINE_MODE == LINE_MIDPOINT
        QglInt d = 2 * dx - dy;
        typedef MidpointLine<FixedLiner_Int, AlterLiner_Int, int, LineVisitYx>  Line;
        Line::Draw(y, x, yEnd, dy*2, dx*2, d, visit);
#endif
    }
    else
    {
        if (x1 < x0)
        {
            xIni = x1;
            xEnd = x0;
            yIni = y1;
            yEnd = y0;
        }
        else
        {
            xIni = x0;
            xEnd = x1;
            yIni = y0;
            yEnd = y1;
        }

        QglInt yInc = (yEnd < yIni)? -1 : 1;

        LineVisitXy   visit(pixie);
        FixedLiner_Int x(xIni);
        AlterLiner_Int y(yIni, yInc);

#if LINE_MODE == LINE_BRESENHAM
        QglInt a = 0;
        typedef BresenhamLine<FixedLiner_Int, AlterLiner_Int, int, LineVisitXy> Line;
        Line::Draw(x, y, xEnd, dx*2, dy*2, dx, a, visit);
#elif LINE_MODE == LINE_MIDPOINT
        QglInt d = 2 * dy - dx;
        typedef MidpointLine<FixedLiner_Int, AlterLiner_Int, int, LineVisitXy>  Line;
        Line::Draw(x, y, xEnd, dx*2, dy*2, d, visit);
#endif
    }
}


/* 
 * .. Circle Drawing
 */

/* 
 * .. Circle Drawing
 */

#if CIRCLE_MODE == CIRCLE_BRESENHAM

void Qgl_Circle (QglInt x0, QglInt y0, QglInt radius, QglPixie *pixie)
{
    typedef CircleVisitXy4<CircleIncX, CircleIncY>      Visit;
    typedef BresenhamCircle<CircleIncX, CircleIncY, CirclePermit, Visit>    Circle;

    Visit           visit(pixie, x0, y0);
    CirclePermit    permit;

    QglInt d = 2 * (1 - radius);

    CircleInc<1>    x(0);
    CircleInc<-1>   y(radius);

    Circle::Draw(x, y, d, permit, visit);

}

#elif CIRCLE_MODE == CIRCLE_MIDPOINT

void Qgl_Circle (QglInt x0, QglInt y0, QglInt radius, QglPixie *pixie)
{
    typedef CircleVisit8<CircleIncX, CircleIncY>                        Visit;
    typedef MidpointCircle<CircleIncX, CircleIncY, CirclePermit, Visit>  Circle;

    Visit           visit(pixie, x0, y0);
    CirclePermit    permit;


    CircleIncX x(0, 3);
    CircleIncY y(radius, 2 - radius - radius);

    QglInt d = 1 - radius;

    Circle::Draw(x, y, d, permit, visit);
}


/* 
 * .. Ellipse Drawing
 */

void Qgl_Ellipse (QglInt x0, QglInt y0, QglInt a, QglInt b, QglPixie *pixie)
{
    typedef CircleVisitXy4<EllipseIncX, EllipseIncY>                            VisitXy;
    typedef CircleVisitYx4<EllipseIncY, EllipseIncX>                            VisitYx;
    typedef MidpointCircle<EllipseIncX, EllipseIncY, EllipsePermit1, VisitXy>    Ellipse1;
    typedef MidpointCircle<EllipseIncY, EllipseIncX, EllipsePermit2, VisitYx, EllipseJudge2>    Ellipse2;

    VisitXy         visitXy(pixie, x0, y0);
    VisitYx         visitYx(pixie, y0, x0);
    EllipsePermit1  permit1;
    EllipsePermit2  permit2;

    EllipseIncX x(0, 4 * b * b * (2 * 0 + 3), b);
    EllipseIncY y(b, 4 * a * a * (-2 * b + 2), a);

    QglInt d = 4 * b * b + a * a * (-4 * b + 1);

    Ellipse1::Draw(x, y, d, permit1, visitXy);

    d = (b * (2 * x.value + 1)) * (b * (2 * x.value + 1));
    d += 4 * a * a * ((y.value - 1) * (y.value - 1) - b * b);
    x.delta_value = 4 * b * b * (2 * x.value + 2);
    y.delta_value = 4 * a * a * (-2 * y.value + 3);

    Ellipse2::Draw(y, x, d, permit2, visitYx);

}


#endif
/*------------------------------ End of Module -------------------------------*/


