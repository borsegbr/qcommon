/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : clip.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-30  quanben     Created.
 *          2007-07-01  quanben     Roughly debugged.
 *
 ******************************************************************************/

#ifndef _clip_H
#define _clip_H

/*--------------------------------- Includes ---------------------------------*/

#include "qgl.h"

/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN

/*--------------------------------- Classes ----------------------------------*/



/* Sutherland - Hodgeman Line Clipping */

template <class _VTp>
class ShLineClip
{
protected:
    typedef _VTp &          _VRef;
    typedef const _VTp &    _VCref;

    enum
    {
        k_None      = 0x0,
        k_Left      = 0x1,
        k_Right     = 0x2,
        k_Top       = 0x4,
        k_Bottom    = 0x8
    };

    typedef int     Code;

protected:
    static Code Encode (_VRef x, _VRef y, _VCref xL, _VCref xR, _VCref yT, _VCref yB)
    {
        Code code = k_None;

        if (x < xL)
        {
            code |= k_Left;
        }
        else if (x > xR)
        {
            code |= k_Right;
        }

        if (y < yT)
        {
            code |= k_Top;
        }
        else if (y > yB)
        {
            code |= k_Bottom;
        }

        return code;
    }

public:
    static bool Clip (_VRef x0, _VRef y0, _VRef x1, _VRef y1, 
                      _VCref xL, _VCref xR, _VCref yT, _VCref yB)
    {
        
        Code code0 = Encode(x0, y0, xL, xR, yT, yB);
        Code code1 = Encode(x1, y1, xL, xR, yT, yB);

        while (code0 != k_None || code1 != k_None)
        {
            _VTp    x, y;

            if (code0 & code1)
            {
                return false;
            }

            Code code = code0;

            if (code0 == k_None)
            {
                code = code1;
            }
            
            if (code & k_Left)
            { 
                x = xL;
                y = y0 + (y1 - y0) * (xL - x0) / (x1 - x0);
            }
            else if (code & k_Right)
            { 
                x = xR;
                y = y0 + (y1 - y0) * (xR - x0) / (x1 - x0);
            }
            else if (code & k_Bottom)
            { 
                y = yB;
                x = x0 + (x1 - x0) * (yB - y0) / (y1 - y0);
            }
            else if (code & k_Top)
            { 
                y = yT;
                x = x0 + (x1 - x0) * (yT - y0) / (y1 - y0);
            }

            if (code == code0)
            {  
                x0 = x;
                y0 = y; 
                code0 = Encode(x, y, xL, xR, yT, yB);
            }
            else
            { 
                x1 = x;
                y1 = y; 
                code1 = Encode(x, y, xL, xR, yT, yB);
            }

        }

        return true;
    }
    
};      /* class ShClip */

/* Liang - Barskey Line Clipping */

template <class _VTp>
    class LbClip
{
protected:
    typedef _VTp &          _VRef;
    typedef const _VTp &    _VCref;

protected:
    static bool Test (_VTp p, _VTp q, _VRef u0, _VRef u1)
    {
        _VTp r;

        if (p < 0)
        { 
            r = q / p;

            if (r > u1)
            {
                return false;
            }
            else if (r > u0)
            { 
                u0 = r;
                return true;
            }
        }
        else if (p > 0)
        { 
            r = q / p;

            if (r < u0)
            {
                return false;
            }
            else if (r < u1)
            { 
                u1 = r;
                return true;
            }
        }
        else if (q < 0)
        {
            return false;
        }

        return true;
    }


public:
    static bool Clip (_VRef x0, _VRef y0, _VRef x1, _VRef y1, 
                      _VCref xL, _VCref xR, _VCref yT, _VCref yB)
    {
        _VTp dx = x1 - x0;
        _VTp dy = y1 - y0;

        _VTp u0(0);
        _VTp u1(1);

        if (Test(-dx, x0 - xL, u0, u1))
        {
            if (Test(dx, xR - x0, u0, u1))
            {
                if (Test(-dy, y0 - yT, u0, u1))
                {
                    if (Test(dy, yB - y0, u0, u1))
                    {
                        x1 = x0 + u1 * dx;
                        y1 = y0 + u1 * dy;
                        x0 += u0 * dx;
                        y0 += u0 * dy;
                        return true;
                    }
                }
            }
        }
        return false;
    }


};      /* class ShClip */


/* Sutherland - Hodgeman Polygon Clipping */


template <class _PolygonIn, class _PolygonOut = _PolygonIn>
    class ShPolygonClip
{
protected:
    typedef typename _PolygonIn::_VTp       _VTp;
    typedef typename _PolygonIn::_Vertex    _Vertex;

    typedef _VTp &                          _VRef;
    typedef const _VTp &                    _VCref;


    struct Edge
    {
        _VTp    x0, y0;
        _VTp    x1, y1;

        Edge (_VCref __x0, _VCref __y0, _VCref __x1, _VCref __y1)
            : x0(__x0), y0(__y0), x1(__x1), y1(__y1)
        {
        }

        bool IsInside (const _Vertex &test) const
        {
            /* The edge ([:this:]) is one of the boundaries of an counter-clockwise rectangle */
            if (x0 < x1)
            {   /* bottom */
                return (test.get_y() < y0);
            }
            else if (x0 > x1)
            {   /* top */
                return (test.get_y() > y0);
            }
            else if (y0 < y1)
            {   /* left */
                return (test.get_x() > x0);
            }
            else /* y0 > y1 */
            {   /* right */
                return (test.get_x() < x0);
            }
            return false;
        }

        void Intersect (const _Vertex &vS, const _Vertex &vP, _Vertex &vI) const
        {
            if (y0 == y1)
            {   /* Horizontal */
                vI.set_x(vS.get_x() + (y0 - vS.get_y()) * (vP.get_x() - vS.get_x()) / (vP.get_y() - vS.get_y()));
                vI.set_y(y0);
            }
            else
            {   /* Vertical */
                vI.set_x(x1);
                vI.set_y(vS.get_y() + (x0 - vS.get_x()) * (vP.get_y() - vS.get_y()) / (vP.get_x() - vS.get_x()));
            }
        }
    };

public:
    template <class _PolygonIn, class _PolygonOut, class _Edge>
        static void ClipOnEdge (const _PolygonIn &polygonIn, _PolygonOut &polygonOut, const _Edge &edge)
    {
        _Vertex vS = polygonIn.last();
        _Vertex vP = polygonIn.first();

        polygonOut.clear();

        /*
         * We don't claim definite size for [:polygonOut:] here, as it is better assigned
         * before this function or the polygon may be increased dynamically
         */

        _Vertex vI;
        vI.create();

        while (1)
        {
            if (edge.IsInside(vP))
            {
                if (edge.IsInside(vS))
                {   /* Both S, P are inside */
                    polygonOut.push_back(vP);
                }
                else
                {   /* Only P is inside */
                    edge.Intersect(vS, vP, vI);
                    polygonOut.push_back(vI);   /* Make sure that [:push_back:] doesn't release the possible allocation. */
                    polygonOut.push_back(vP);
                }
            }
            else if (edge.IsInside(vS))
            {   /* Only S is inside */
                edge.Intersect(vS, vP, vI);
                polygonOut.push_back(vI);
            }

            if (vP == polygonIn.last())
            {
                break;
            }

            vS = vP;
            vP = polygonIn.next(vP);
        }
        vI.destroy();
    }

    static void Clip (const _PolygonIn &polygonIn, _PolygonOut &polygonTemp, 
                      _PolygonOut &polygonOut, _VCref xL, _VCref xR, 
                      _VCref yT, _VCref yB)
    {
        Edge        eB(xL, yB, xR, yB);
        Edge        eR(xR, yB, xR, yT);
        Edge        eT(xR, yT, xL, yT);
        Edge        eL(xL, yT, xL, yB);

        ClipOnEdge<_PolygonIn,  _PolygonOut, Edge>(polygonIn, polygonTemp,  eB);
        if (polygonTemp.count() == 0)
        {
            polygonOut = polygonTemp;
            return;
        }
        ClipOnEdge<_PolygonOut, _PolygonOut, Edge>(polygonTemp, polygonOut, eR);
        if (polygonOut.count() == 0)
        {
            return;
        }
        ClipOnEdge<_PolygonOut, _PolygonOut, Edge>(polygonOut, polygonTemp, eT);
        if (polygonTemp.count() == 0)
        {
            polygonOut = polygonTemp;
            return;
        }
        ClipOnEdge<_PolygonOut, _PolygonOut, Edge>(polygonTemp, polygonOut, eL);

    }

};      /* class ShPolygonClip */

_QGL_END

#endif  /* ifndef _clip_H */

/*------------------------------ End of Module -------------------------------*/
