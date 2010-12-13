/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : fill.h
 *
 * Description  : <Description>
 *
 * Annotation   : <Annotation>
 *
 *------------------------------------------------------------------------------
 * History
 *------------------------------------------------------------------------------
 *
 * 1.00     2007-06-13  quanben     Created.
 *          2007-06-29  quanben     Roughly debugged and tested.
 *
 ******************************************************************************/

#ifndef _fill_H
#define _fill_H

/*--------------------------------- Includes ---------------------------------*/

#include <xutility>
#include <stack>

#include "qgl.h"

#include <qul/debug/trace.h>      /* Debug */


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

_QGL_BEGIN


/*--------------------------------- Classes ----------------------------------*/

/*
 * Following is a polygon edge fill engine, it requires further implementation 
 * of edge formation.
 *
 * Implementation requirement:
 * ...
 *
 */


template <class _Traits>
    class EdgeFill
{
protected:
    typedef typename _Traits::_Polygon  _Polygon;
    typedef typename _Traits::_PTp      _PTp;
    typedef typename _Traits::_VTp      _VTp;
    typedef typename _Traits::_Ctrl     _Ctrl;      /* Reference is allowed and recommended */
    typedef typename _Traits::_EtRoot   _EtRoot;
    typedef typename _Traits::_Aet      _Aet;       
    typedef typename _Traits::_Edge     _Edge;

    typedef typename _Polygon::_Vertex  _Vertex;

    typedef const _VTp &                _VCref;
    typedef _VTp &                      _VRef;
    typedef const _PTp &                _PCref;
    typedef _PTp &                      _PRef;


public:
    static void Fill (const _Polygon &polygon, _Ctrl ctrl)
    {
        _PTp    y, yNext, yBegin, yEnd;

        _Aet    aet;
        _EtRoot root;

        /* Create edge-table-root according to the polygon parameter */
        ctrl.create_root(polygon, root, yBegin, yEnd);

        /* Initialize edge table root, typically the root should initialize its `y' iterator */
        root.init();

        for (y = yBegin; y < yEnd; y = yNext)
        {
            _Edge    edge0, edge1;

            yNext = y + 1;

            if (root.fall(y))
            {
                Merge(aet, root);
                root.next_y();
            }
            else
            {
                Reorder(aet);
            }

            edge0 = aet.begin();
            ctrl.init();

            do 
            {
                _PTp x = 0, x1 = 0;
                edge1 = aet.next(edge0);

                ctrl.limit(edge0, edge1, x, x1);

                for ( ; x < x1; ++x)
                {
                    ctrl.visit(x, y);
                }

                ctrl.update(aet, edge0, y);
                edge0 = aet.next(edge1);
                ctrl.update(aet, edge1, y);

            } while (!(edge0 == aet.end()));

        }
    }
 
    static void CreateRoot (const _Polygon &polygon, _EtRoot &root, _PRef yBegin, _PRef yEnd, _Ctrl ctrl)
        /*
        * Only a reference
        */
    {
        _VTp x0, y0, x1, y1;
        _VTp yMin, yMax;
        _Vertex vtx;

        vtx = polygon.last();
        x0 = vtx.get_x();
        y0 = vtx.get_y();

        vtx = polygon.first();
        x1 = vtx.get_x();
        y1 = vtx.get_y();
        yMin = std::min(y0, y1);
        yMax = std::max(y0, y1);
        ctrl.add_edge_first(root, x0, y0, x1, y1);   /* Add first edge */

        vtx = polygon.next(vtx);
        x1 = vtx.get_x();
        y1 = vtx.get_y();
        yMin = std::min(yMin, y1);
        yMax = std::max(yMax, y1);
        ctrl.add_edge_second(root, x1, y1);

        while (1)
        {
            /* update */
            vtx = polygon.next(vtx);
            x1 = vtx.get_x();
            y1 = vtx.get_y();
            yMin = std::min(yMin, y1);
            yMax = std::max(yMax, y1);

            if (vtx == polygon.last())
            {
                ctrl.add_edge_last(root, x1, y1);
                break;
            }
            ctrl.add_edge(root, x1, y1);
        }

        yBegin = (_PTp)yMin;
        yEnd = ((_PTp)yMax) + 1;

    }

protected:
    static void Bubble (_Aet &aet, _Edge &edgePrev, _Edge &edgeNext)
    {
        while (edgePrev != aet.end() && !(edgePrev < edgeNext))
        {
            aet.swap(edgePrev, edgeNext);
            edgeNext = edgePrev;
            edgePrev = aet.prev(edgePrev);
        }
    }

    static void Merge (_Aet &aet, _EtRoot &root)
    {
        _Edge aetEdge = aet.begin();
        _Edge etEdge = root.begin();

        while (1)
        {
            bool bEtOver = (etEdge == root.end());
            bool bAetOver = (aetEdge == aet.end());

            if (!bEtOver && (bAetOver || etEdge < aetEdge))
            {
                _Edge nextEdge = root.next(etEdge);
                root.erase(etEdge);                     /* erase before insert */
                aet.insert(aetEdge, etEdge);
                aetEdge = etEdge;
                etEdge = nextEdge;
            }
            else if (!bAetOver)
            {
                _Edge aetEdgeNext = aet.next(aetEdge);

                if (!(aetEdgeNext == aet.end()))
                {
                    _Edge edgeNext = aetEdgeNext;
                    Bubble(aet, aetEdge, edgeNext);
                }
                aetEdge = aetEdgeNext;
            }
            else
            {
                break;
            }
        }
    }

    static void Reorder (_Aet &aet)
    {
        /* Very strict assertion that AET is always nonempty and contains even number of elements.*/

        _Edge edge0 = aet.begin();
        _Edge edge1 = aet.next(edge0);

        do 
        {  
            _Edge edgeNext = edge1;
            Bubble(aet, edge0, edgeNext);
            edge0 = edge1;
            edge1 = aet.next(edge1);
        } while(!(edge1 == aet.end()));
    }
    
private:
    
};      /* class EdgeFill */



template <class _PelTp, class _VisitTp>
    struct SeedFillSimpleTraits
{
    typedef _PelTp          _PTp;
    typedef const _PTp &    _PCref;

    typedef _VisitTp        _Visit;

    struct _Vertex
    {
        _PTp    x, y;

        _Vertex (_PCref __x, _PCref __y) : x(__x), y(__y) {}

        _PCref   get_x () const { return x; }
        _PCref   get_y () const { return y; }

    };

    typedef std::stack<_Vertex>     _Stack;

};

template <class _Traits>
    class SeedFill
{
protected:
    typedef typename _Traits::_Stack    _Stack;
    typedef typename _Traits::_Vertex   _Vertex;
    typedef typename _Traits::_PTp      _PTp;
    typedef typename _Traits::_Visit    _Visit;

    typedef typename _PTp &             _PRef;
    typedef typename const _PTp &       _PCref;

public:
    static void Fill (_Visit visit, _PRef x, _PRef y)
    {
        _Stack  vtxStk; /* Constructor does necessary initialization */
        
        _PTp    x0, xl, xr;

        vtxStk.push(_Vertex(x, y));
              
        while (!vtxStk.empty())
        {
            _Vertex &vtx = vtxStk.top();
            vtxStk.pop();

            x = vtx.get_x();
            y = vtx.get_y();
            visit.put(x, y);

            x0 = x + 1;
            while (!visit.get_border(x0, y))
            {
                visit.put(x0, y);
                ++x0;
            }
            xr = x0 - 1;

            x0 = x - 1;
            while (!visit.get_border(x0, y))
            {
                visit.put(x0, y);
                --x0;
            }
            xl = x0 + 1;
            
            ++y;
            for (int i = 0; i < 2; i++, y -= 2)
            {
                x0 = xl;
                while (x0 <= xr)
                {
                    bool found = false;
                    while (1)
                    {
                        if (visit.get_either(x0, y))
                        {
                            break;
                        }

                        found = true;
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
                        _PTp new_x = x0;
                        if (visit.get_either(x0, y))
                        {
                            --new_x;
                        }

                        vtxStk.push(_Vertex(new_x, y));
                    }

                    do 
                    {
                        ++x0;	
                    } while (visit.get_either(x0, y) && x0 <= xr);
                }
            }
        }
    }
};      /* class SeedFill */


_QGL_END

#endif  /* ifndef _fill_H */

/*------------------------------ End of Module -------------------------------*/
