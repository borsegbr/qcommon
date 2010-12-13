/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : line.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-xx  quanben     Created.
 *
 ******************************************************************************/

#ifndef _line_H
#define _line_H

/*--------------------------------- Includes ---------------------------------*/

#include "qgl.h"

#include <cassert>

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/



template <class _PelTp = int, bool dec = false>
    struct FixedLiner
{
    typedef     _PelTp  _PTp;

    _PTp        value;

    FixedLiner (_PTp val = 0)
        : value(val)
    {
    }

    void operator++ ()
    {
        if (dec)
        {
            --value;
        }
        else
        {
            ++value;
        }
    }

    bool operator!=  (_PTp v)
    {
        return (value != v);
    }
};


template <class _PelTp = int>
    struct AlterLiner
{
    typedef     _PelTp  _PTp;

    _PTp        incr;
    _PTp        value;

    AlterLiner (_PTp val = 0, _PTp inc = 1)
        : value(val), incr(inc)
    {
    }

    void operator++ ()
    {
        value += incr;
    }

    bool operator!=  (_PTp v)
    {
        return (value != v);
    }
};

template <class _XTp, class _YTp>
    struct LineVisit
{
    virtual void operator() (const _XTp &x, const _YTp &y)
    {
        /* 
         * It is just an example. Inheriting from this class is not recommended.
         */
        assert(0);
    }
};

template <class _XTp = FixedLiner<>, class _YTp = AlterLiner<>, class _VTp = int, class _Visit = LineVisit<_XTp, _YTp> >
    class BresenhamLine
{
public:
    typedef const _XTp &        _XCref;
    typedef _XTp &              _XRef;
    typedef const _YTp &        _YCref;
    typedef _YTp &              _YRef;

    typedef typename _XTp::_PTp _PTp;
    typedef const _PTp &        _PCref;
    typedef _PTp &              _PRef;

    typedef const _VTp &        _VCref;
    typedef _VTp &              _VRef;


public:
    static void Draw (_XRef x, _YRef y, _PCref xf, _VCref dx, _VCref dy, _VCref thr, _VRef a, _Visit visit)
    {
        assert(dy >= 0);
        assert(dx >= dy);
        assert(!(a > thr));

        for (; x != xf; ++x)
        {
            visit(x, y);
            a += dy;
            if (a > thr)   /* only when above */
            {
                ++y;
                a -= dx;
            }
        }
    }
};      /* class BresenhamLine */


template <class _XTp = FixedLiner<>, class _YTp = AlterLiner<>, class _VTp = int, class _Visit = LineVisit<_XTp, _YTp> >
    class MidpointLine
{
public:
    typedef const _XTp &        _XCref;
    typedef _XTp &              _XRef;
    typedef const _YTp &        _YCref;
    typedef _YTp &              _YRef;

    typedef typename _XTp::_PTp _PTp;
    typedef const _PTp &        _PCref;
    typedef _PTp &              _PRef;

    typedef const _VTp &        _VCref;
    typedef _VTp &              _VRef;

public:
    static void Draw (_XRef x, _YRef y, _PCref xf, _VCref dx, _VCref dy, _VRef d, _Visit visit)
    {
        assert(dy >= 0);
        assert(dx >= dy);

        _VTp d_move = dy - dx;

        for ( ; x != xf; ++x)
        {
            visit(x, y);

            if (d > 0)
            {   /* [:y:] moves on */
                ++y;
                d += d_move;
            }
            else
            {   /* [:y:] stays */
                d += dy;
            }
        }
    }
};      /* class MidpointLine */


template <class _XTp, class _YTp, class _Permit, class _Visit = Visit<_XTp, _YTp> >
    class BresenhamCircle
{
public:
    typedef const _XTp &        _XCref;
    typedef _XTp &              _XRef;
    typedef const _YTp &        _YCref;
    typedef _YTp &              _YRef;

    typedef typename _XTp::_VTp _VTp;
    typedef _VTp &              _VRef;
    typedef const _VTp &        _VCref;

public:
    static void Draw (_XRef x, _YRef y, _VRef d, _Permit permit, _Visit visit)
    {
        enum Dir
        {
            k_H,
            k_D,
            k_V
        } dir;

        while (permit(x, y))
        {
            visit(x, y);

            if (d < (_VTp)0)
            {
                _VTp dHd = d * 2 - y.delta();   /* In principle, [: 2 * y.ry * y.ay + y.ay * y.ay :] */     
                 dir = (dHd > 0)? k_D : k_H;
            }
            else if (d > (_VTp)0)
            {
               _VTp dDv = d * 2 - x.delta();    /* In principle, [: 2 * y.rx * y.ax + y.ax * y.ax :] */
                dir = (dDv > 0)? k_V : k_D;
            }
            else
            {
                dir = k_D;
            }

            switch(dir) 
            {
            case k_H:
                ++x;
                d += x.delta();
                break;
            case k_D:
                ++x;
                ++y;
                d += x.delta();
                d += y.delta();
                break;
            case k_V:
                ++y;
                d += y.delta();
                break;
            }
        }
    }
};      /* BresenhamCircle */

template <class _D>
    struct CircleDeltaJudge
{
    bool operator() (const _D &d)
    {
        return (d < 0);
    }
};

template <class _XTp, class _YTp, class _Permit, class _Visit = Visit<_XTp, _YTp>, class _Judge = CircleDeltaJudge<int> >
    class MidpointCircle
{
public:
    typedef const _XTp &        _XCref;
    typedef _XTp &              _XRef;
    typedef const _YTp &        _YCref;
    typedef _YTp &              _YRef;

    typedef typename _XTp::_VTp _VTp;
    typedef _VTp &              _VRef;
    typedef const _VTp &        _VCref;

public:
    static void Draw (_XRef x, _YRef y, _VRef d, _Permit permit, _Visit visit, _Judge judge = _Judge())
    {
        /*
         * Typically, for circle, [: d = 1 - radius :], [: x.d = 3 :], [: y.d = 2 - radius * 2 :], 
         * for ellipse, 
         */

        while(permit(x, y))
        {
            visit(x, y);

            if (judge(d))
            {
                d += x.delta();
                ++x;
            }
            else
            {
                d += x.delta() + y.delta();
                ++x;
                ++y;
            }
        }
    }
};      /* class MidpointCircle */


_QGL_END

#endif  /* ifndef _line_H */

/*------------------------------ End of Module -------------------------------*/
