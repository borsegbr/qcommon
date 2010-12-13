/*******************************************************************************
 *                    Quanben Software, All Rights Reserved                    *
 *******************************************************************************
 *
 * Module Name  : fill.cpp
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
 *          2007-06-29  quanben     [1] Bresenham line compatibility.
 *                                  See topic on Bresenham line coherence on both direction:
 *                                  http://blog.csdn.net/quanben/archive/2007/06/29/1671596.aspx
 *                                  
 *
 ******************************************************************************/

/*--------------------------------- Includes ---------------------------------*/

#include "fill.h"

#include <list>
#include <map>


/*---------------------------------- Macros ----------------------------------*/

/*-------------------------------- Namespaces --------------------------------*/

using namespace qgl;
using namespace std;

/*---------------------------------- Types -----------------------------------*/

/*
 * Polygon Edge-filling
 */


struct Polygon
{
    typedef QglInt  _Count;

    struct _Vertex
    {
        const QglVertex2i    *state;

        _Vertex (const QglVertex2i *p = NULL) : state(p)
        {
        }
        
        bool operator== (const _Vertex &other) const
        {
            return state == other.state;
        }

        QglInt get_x () const
        {
            return state->x;
        }

        QglInt get_y () const
        {
            return state->y;
        }
    };

    QglPolygoni *state;
    
    Polygon (QglPolygoni *p = NULL) : state(p)
    {
    }

    _Vertex first () const
    {
        return _Vertex(state->vertices + 0);
    }

    _Vertex last () const
    {
        return _Vertex(state->vertices + state->count - 1);
    }

    _Vertex next (const _Vertex &cur) const
    {
        return _Vertex(cur.state + 1);
    }
};


struct EdgeState;

struct Edge
{
    EdgeState   *state;

    Edge (EdgeState *p = NULL);
    virtual ~Edge ();

    void operator= (EdgeState *p);
    bool operator== (const EdgeState *p) const;
    bool operator!= (const EdgeState *p) const;

    void Create (QglInt x0, QglInt y0, QglInt x1, QglInt y1);
 
    void Destroy ();
    void Update ();
 
	void unionize (const Edge &other);

    bool operator== (const Edge &other) const;
    bool operator!= (const Edge &other) const;
    bool operator< (const Edge &other) const;
 
    bool bottom (QglInt y);
 
};

typedef list<Edge>          EdgeList;
typedef EdgeList::iterator  EdgeItr;

struct EdgeState
{
    QglInt  yBottom;

    /* Current `x' */
    QglInt  xBegin, xEnd;           /* XXXXXXX0000 */
                                    /* ^      ^    */
                                    /* left   right */
    /* Accumulator */
    QglInt  acc;

    /* Carry the slope */
    QglSint dx;
    QglInt  dy;                     /* always non-negative */

    EdgeItr     itr;

    EdgeState   *follower;          /* the adjacent edge following */

};



struct EdgeFillVisit
{
    QglPixie *pixie;

    EdgeFillVisit (QglPixie *px) : pixie(px)
    {
    }

    void operator() (QglInt x, QglInt y)
    {
        pixie->set(pixie->data, x, y);
    }
};

struct EdgeTableState
{
    EdgeList    data;

    EdgeTableState ()
    {
    }

    virtual ~EdgeTableState ()
    {
        for (EdgeItr itr = data.begin(); itr != data.end(); ++itr)
        {
            itr->Destroy();
        }
    }
};

struct EdgeTable
{
    EdgeTableState  *state;

    EdgeTable () : state(NULL)
    {
    }

    virtual ~EdgeTable ()
    {
    }

    void Release ()
    {
        state = NULL;
    }

    bool operator== (const EdgeTable &other) const
    {
        return state == other.state;
    }

    bool operator!= (const EdgeTable &other) const
    {
        return state != other.state;
    }

    void Create ()
    {
        state = new EdgeTableState();
        TRACE(": new EdgeTableState at 0x%x\n", (QglUint32)state);
    }

    void Destroy ()
    {
        if (state)
        {
            TRACE(": delete EdgeTableState at 0x%x\n", (QglUint32)state);
            delete state;
            state = NULL;
        }
    }

    Edge begin () const
    {
        if (state == NULL)
        {
            return Edge();
        }
        EdgeItr itr = state->data.begin();
        return (itr != state->data.end()) ? (*itr) : Edge();
    }

    Edge end () const
    {
        return Edge();
    }

    Edge next (const Edge &cur) const
    {
        EdgeItr itr = cur.state->itr;
        ++itr;
        Edge nextEdge;
        if (itr != state->data.end())
        {
            nextEdge.state = itr->state;
        }
        return nextEdge;
    }

    Edge prev (const Edge &cur) const
    {
        EdgeItr itr = cur.state->itr;
        Edge prevEdge;
        if (itr != state->data.begin())
        {
            --itr;
            prevEdge.state = itr->state;
        }
        return prevEdge;
    }

    void insert (Edge &edge)
    {
        if (state->data.empty())
        {
            edge.state->itr = state->data.insert(state->data.end(), edge);
            return;
        }

        for (EdgeItr itr = state->data.begin(); ; ++itr)
        {
            if (itr == state->data.end() || edge < *itr)
            {
                edge.state->itr = state->data.insert(itr, edge);
                break;
            }
        }
    }

    void insert (const Edge &at, Edge &edge)
    {
        if (at.state == NULL)
        {
            edge.state->itr = state->data.insert(state->data.end(), edge);
        }
        else
        {
            edge.state->itr = state->data.insert(at.state->itr, edge);
        }
    }

    void erase (const Edge &edge)
    {
        state->data.erase(edge.state->itr);
    }

    void swap (Edge &edge0, Edge &edge1)
    {
        EdgeItr itr0 = edge0.state->itr;
        EdgeItr itr1 = edge1.state->itr;
        
        Edge temp = *itr0;
        *itr0 = *itr1;
        *itr1 = temp;

        itr0->state->itr = itr0;
        itr1->state->itr = itr1;
    }
};

struct Aet : public EdgeTable
{
    Aet ()
    {
        Create();
    }

    ~Aet ()
    {
        Destroy();
    }
};

struct EtRoot
{
    struct EdgeLine : public EdgeTable
    {
        QglInt  y;

        EdgeLine (QglInt __y) : y(__y) 
        {
        }

        void Create ()
        {
            EdgeTable::Create();
        }

        virtual ~EdgeLine ()
        {
            Destroy();
        }
    };

    typedef list<EdgeLine>  Data;
    typedef Data::iterator  DataItr;

    Data    data;
    DataItr itr;

    EtRoot ()
    {
    }

    Edge begin () const
    {
        return itr->begin();
    }

    Edge end () const
    {
        return itr->end();
    }

    Edge next (const Edge &edge) const
    {
        return itr->next(edge);
    }

    void erase (Edge &edge)
    {
        return itr->erase(edge);
    }

    bool fall (QglInt y)
    {
        return (itr != data.end() && y == itr->y);
    }

    void next_y ()
    {
        ++itr;
    }

    void init ()
    {
        itr = data.begin();
    }

    void Add (QglInt y, EdgeList &candidates)
    {
        while(!candidates.empty())
        {
            Edge edge = candidates.back();
            candidates.pop_back();
            Add(y, edge);
        }
    }

    void Add (QglInt y, Edge &edge)
    {
        if (data.empty())
        {
            EdgeLine line(y);
            line.Create();
            line.insert(edge);
            data.push_back(line);
            line.Release();
            return;
        }
        for (DataItr itr = data.begin(); ; ++itr)
        {
            if (itr == data.end() || y < itr->y)
            {
                EdgeLine line(y);
                line.Create();
                line.insert(edge);
                data.insert(itr, line);
                line.Release();
                return;
            }
            else if (itr->y == y)
            {
                itr->insert(edge);
                return;
            }
        }
    }
};


struct EdgeController;

struct EdgeFillTraits
{
    typedef Polygon         _Polygon;
    typedef EdgeFillVisit   _Visit;
    typedef QglInt          _PTp;
    typedef QglInt          _VTp;
    typedef EtRoot          _EtRoot;
    typedef EdgeController  &_Ctrl;

    typedef Aet             _Aet;
    typedef Edge            _Edge;
};

struct EdgeController
{
    enum Style
    {
        k_BorderInc,
        k_BorderExc
    } style;

    enum { 
        k_MinInt = 0x80000000,
        k_MaxInt = 0x7fffffff
    };

    QglInt          last;
    EdgeFillVisit   visit;

    EdgeController (QglPixie *pixie, Style __style = k_BorderInc) : style(__style), last(0), visit(pixie)
    {
    }

    bool hori_edge_creatable ()
    {
        switch (style)
        {
        case k_BorderInc:
            return false;
        case k_BorderExc:
            return true;
        default:
            return false;
        }
    }

    void init ()
    {
        switch (style)
        {
        case k_BorderInc:
            last = k_MaxInt;
            break;
        case k_BorderExc:
            last = k_MinInt;
            break;
        }
    }

    void limit (const Edge &edge0, const Edge &edge1, QglInt &x0, QglInt &x1)
    {
        switch (style)
        {
        case k_BorderInc:
            x0 = min(last, edge0.state->xBegin);
            x1 = max(edge0.state->xEnd, edge1.state->xEnd);
            if (x1 > last)
            {
                last = x1;
            }
            break;
        case k_BorderExc:
            x0 = max(last, edge0.state->xEnd);
            x1 = edge1.state->xBegin;
            last = max(x0, edge1.state->xEnd);
            break;
        }
    }

    void update (EdgeTable &aet, Edge &edge, QglInt y)
    {
        y++;
        if (y < edge.state->yBottom)
        {
            edge.Update();
        }
        else if (y > edge.state->yBottom)
        {
            EdgeItr cellItr = edge.state->itr;
            Edge    follower = edge.state->follower;

            while (1)
            {
                if (follower != NULL)
                {
                    if (follower.state->dy == 0)
                    {
                        Edge del = follower;
                        follower = follower.state->follower;
                        del.Destroy();
                    }
                    else
                    {
                        *cellItr = follower;
                        follower.state->itr = cellItr;
                        follower.Update();
                        break;
                    }
                }
                else
                {
                    aet.erase(edge);
                    break;
                }
            }
            edge.Destroy();
            edge = follower;    /* unnecessary */
        }
		else    /* y == edge.state->yBottom */
        {
			edge.Update();
            Edge follower = edge.state->follower;
            if (follower != NULL)
            {
                edge.unionize(follower);
                
                while (follower.state->dy == 0)
                {
                    follower = follower.state->follower;
                    if (follower == NULL)
                    {
                        break;
                    }
                    edge.unionize(follower);
                }
            }
        }
    }

    EdgeList    candidates;

    Edge lastEdge;
    Edge prevEdge;
    bool lastEdgeHathpace;
    bool needTracing;
    QglInt  xLast0, yLast0, xLast1, yLast1;
    QglInt  xOld0, yOld0, xOld1, yOld1;

    void Connect (Edge &edge, EdgeList &candidates)
    {
        Edge cur = edge;
        while (!candidates.empty())
        {
            Edge candidate = candidates.back();
            candidates.pop_back();
            cur.state->follower = candidate.state;
            cur = candidate;
        }
    }

    void add_edge_first (EtRoot &root, QglInt x0, QglInt y0, QglInt x1, QglInt y1)
    {
        lastEdge.Create(x0, y0, x1, y1);
        xLast0 = xOld0 = x0;
        yLast0 = yOld0 = y0;
        xLast1 = xOld1 = x1;
        yLast1 = yOld1 = y1;

        needTracing = (y1 == y0);

        prevEdge = lastEdge;
        lastEdgeHathpace = false;
    }

    void add_edge_second (EtRoot &root, QglInt x1, QglInt y1)
    {
        add_edge(root, x1, y1);
    }

    void add_edge (EtRoot &root, QglInt x1, QglInt y1)
    {
        Edge edge1;
        edge1.Create(xOld1, yOld1, x1, y1);

        if (yOld1 < yOld0)
        {
            if (yOld1 < y1)
            {
                root.Add(yOld1, prevEdge);
                root.Add(yOld1, edge1);
            }
            else if (yOld1 == y1)
            {
                candidates.push_back(prevEdge);
                candidates.push_back(edge1);
            }
            else
            {
                edge1.state->follower = prevEdge.state;
            }
        }
        else if (yOld1 == yOld0)
        {   /* trace checking for first several edges are needed */
            if (yOld1 < y1)
            {
                prevEdge.state->follower = edge1.state;
                if (!candidates.empty())
                {
                    root.Add(yOld1, candidates);
                }
                else if (needTracing)
                {
                    lastEdgeHathpace = true;
                    needTracing = false;
                }
            }
            else if (yOld1 == y1)
            {
                prevEdge.state->follower = edge1.state;
            }
            else
            {
                if (candidates.empty())
                {   /* a valley */
                    prevEdge.state->follower = edge1.state->follower = NULL;
                }
                else
                {
                    edge1.state->follower = prevEdge.state;

                    edge1 = candidates.back();
                    prevEdge = candidates.back();
                    candidates.empty();
                    edge1.state->follower = prevEdge.state;
                }
                needTracing = false;
            }
        }
        else    /* yOld1 > yOld0 */
        {
            if (yOld1 <= y1)
            {
                prevEdge.state->follower = edge1.state;
            }
            else
            {
                prevEdge.state->follower = edge1.state->follower = NULL;
            }
        }

        prevEdge = edge1;
        xOld0 = xOld1;
        yOld0 = yOld1;
        xOld1 = x1;
        yOld1 = y1;

    }

    void add_edge_last (EtRoot &root, QglInt x1, QglInt y1)
    {
        add_edge(root, x1, y1);

        /*  yOld0      
         *  y1    = yLast0  = yOld1
         *          yLast1
         */

        if (y1 < yOld0)
        {
            if (y1 < yLast1)
            {
                root.Add(y1, prevEdge);
                root.Add(y1, lastEdge);
            }
            else if (y1 == yLast1)
            {
                if (lastEdgeHathpace)
                {
                    root.Add(y1, prevEdge);
                    root.Add(y1, lastEdge);
                }
                else
                {
                    lastEdge.state->follower = prevEdge.state;
                }
            }
            else
            {
                lastEdge.state->follower = prevEdge.state;
            }
        }
        else if (y1 == yOld0)
        {
            if (y1 < yLast1)
            {
                prevEdge.state->follower = lastEdge.state;
                if (!candidates.empty())
                {
                    root.Add(y1, candidates);
                }
            }
            else if (y1 == yLast1)
            {
                prevEdge.state->follower = lastEdge.state;
                if (lastEdgeHathpace)
                {
                    if (!candidates.empty())
                    {
                        root.Add(y1, candidates);
                    }
                }
            }
            else
            {
                if (candidates.empty())
                {   /* a valley */
                    prevEdge.state->follower = lastEdge.state->follower = NULL;
                }
                else
                {
                    lastEdge.state->follower = prevEdge.state;

                    /* [:lastEdge:] and [:prevEdge:] will never be used */
                    lastEdge = candidates.back();
                    prevEdge = candidates.back();
                    candidates.empty();
                    lastEdge.state->follower = prevEdge.state;
                }
            }
        }
        else    /* y1 > yOld0 */
        {
            if (y1 <= yLast1)
            {
                prevEdge.state->follower = lastEdge.state;
            }
            else
            {
                prevEdge.state->follower = lastEdge.state->follower = NULL;
            }    
        }
    }

    void create_root (const Polygon &polygon, EtRoot &root, QglInt &yBegin, QglInt &yEnd)
    {
        EdgeFill<EdgeFillTraits>::CreateRoot(polygon, root, yBegin, yEnd, *this);
    }

};


/*
 * Seed Filling
 */

struct SeedFillVisit
{
    QglSmartPixie *pixie;

    SeedFillVisit (QglSmartPixie *px) : pixie(px)
    {
    }

    void put (QglInt x, QglInt y)
    {
        enum { k_Dummy = 0 };
        pixie->set(pixie->data, x, y, k_QglSeedSetFill, k_Dummy);
    }

    bool get_border (QglInt x, QglInt y)
    {
        return (pixie->get(pixie->data, x, y, k_QglSeedGetBorder) != 0);
    }

    bool get_either (QglInt x, QglInt y)
    {
        return (pixie->get(pixie->data, x, y, k_QglSeedGetEither) != 0);
    }
};



/*-------------------------------- Constants ---------------------------------*/

/*-------------------------------- Variables ---------------------------------*/

/*----------------------------- Member Functions -----------------------------*/

Edge::Edge (EdgeState *p /* = NULL */) : state(p)
{
}

Edge::~Edge ()
{
}

void Edge::operator= (EdgeState *p)
{
    state = p;
}

bool Edge::operator== (const EdgeState *p) const
{
    return state == p;
}

bool Edge::operator!= (const EdgeState *p) const
{
    return state != p;
}

void Edge::Create (QglInt x0, QglInt y0, QglInt x1, QglInt y1)
{
    state = new EdgeState();
    TRACE(": new EdgeState at 0x%x\n", (QglUint32)state);

    QglInt xTop, yTop;
    QglInt xBottom, yBottom;
    if (y1 < y0)
    {
        yTop = y1; xTop = x1;
        yBottom = y0; xBottom = x0;
    }
    else
    {
        yTop = y0; xTop = x0;
        yBottom = y1; xBottom = x1;
    }

    state->follower = NULL;
    state->dx = xBottom - xTop;
    state->dy = yBottom - yTop;
    state->yBottom = yBottom;

    QglInt absDx = abs(state->dx);
    if (absDx > state->dy)
    {   /* go with `x' */
        state->acc = absDx;

        absDx *= 2;
        state->dx *= 2;
        state->dy *= 2;

        if (state->dy > 0)
        {
            if (state->dx < 0)
            {
                state->acc--;   /* To be compatible with Bresenham line from lower x (left) to higher. [1] */
            }

            QglInt kx = (absDx - state->acc) / state->dy + 1;
            state->acc += kx * state->dy;
            state->acc -= absDx;

            if (state->dx > 0)
            {
                state->xBegin = xTop;
                state->xEnd = state->xBegin + kx;
            }
            else    /* state->dx < 0 */
            {
                state->xEnd = xTop + 1;
                state->xBegin = state->xEnd - kx;
            }
        }
        else
        {
            state->xBegin = min(xTop, xBottom);
            state->xEnd = max(xTop, xBottom);
        }
    }
    else
    {   /* go with `y' */
        state->xBegin = xTop;
        state->xEnd = xTop + 1;
        state->acc = state->dy; /* To be compatible with Bresenham line from lower y (top) to higher. [1] */
        state->dx *= 2;
        state->dy *= 2;
    }
}

void Edge::Destroy ()
{
    TRACE(": delete EdgeState at 0x%x\n", (QglUint32)state);
    delete state;
    state = NULL;
}

void Edge::Update ()
{
    QglInt absDx = abs(state->dx);
    if (absDx > state->dy)
    {   /* go with `x' */

        if (state->dy > 0)
        {
            QglInt kx = (absDx - state->acc) / state->dy + 1;
            state->acc += kx * state->dy;
            state->acc -= absDx;

            if (state->dx > 0)
            {
                state->xBegin = state->xEnd;
                state->xEnd += kx;
            }
            else
            {
                state->xEnd = state->xBegin;
                state->xBegin -= kx;
            }            
        }
    }
    else
    {   /* go with `y' */
        state->acc += absDx;
        if (state->acc > state->dy)
        {
            if (state->dx > 0)
            {
                state->xBegin++;
                state->xEnd++;
            }
            else
            {
                state->xBegin--;
                state->xEnd--;
            }

            state->acc -= state->dy;
        }
    }

}

void Edge::unionize (const Edge &other)
{
	if (state->xBegin > other.state->xBegin)
	{
		state->xBegin = other.state->xBegin;
	}
	if (state->xEnd < other.state->xEnd)
	{
		state->xEnd = other.state->xEnd;
	}
}

bool Edge::operator== (const Edge &other) const
{
    return state == other.state;
}

bool Edge::operator!= (const Edge &other) const
{
    return state != other.state;
}

bool Edge::operator< (const Edge &other) const
{
    if (state->xBegin < other.state->xBegin)
    {
        return true;
    }
    else if (state->xBegin > other.state->xBegin)
    {
        return false;
    }
    else /* state->xBegin == other.state->xBegin */
    {
        QglSint left;
        QglSint right;

        if (state->dx < 0)
        {
            if (other.state->dx >= 0)
            {
                return true;
            }
        }
        else if (state->dx == 0)
        {
            return (other.state->dx > 0);
        }
        else
        {
            if (other.state->dx <= 0)
            {
                return false;
            }
        }

        left = state->dy * other.state->dx;
        right = other.state->dy * state->dx;

        return (left > right);

    }
}

bool Edge::bottom (QglInt y)
{
    return y == state->yBottom;
}

/*------------------------- Static Member Functions --------------------------*/

/*******************************************************************************
 * Prototype    : 
 * Description  : N/A
 * Dependency   : N/A
 * Arguments    : 
 *  name        type            dir     description
 *  ----        ----            ---     -----------
 *
 * Return Value : N/A
 * Annotation   : N/A
 ******************************************************************************/


/*------------------- Functions with C Calling Convention --------------------*/


#if 1   /* Whether using these method implemented upon the qgl template library */

using namespace qgl;

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
 *                2007-06-30, Reimplemented upon qgl template library with 
 *                interface unchanged.
 ******************************************************************************/
void Qgl_PolygonFill (const QglPolygoni *polygon, QglPixie *pixie)
{
    typedef EdgeFill<EdgeFillTraits>    SimpleEdgeFill;

    EdgeController      ctrl(pixie, EdgeController::k_BorderExc);

    Polygon normPoly;
    normPoly.state = new QglPolygoni;
    QglVertex2i *buf = new QglVertex2i[polygon->count];
    normPoly.state->vertices = buf;

    QglInt last_x, last_y;
    last_x = last_y = EdgeController::k_MinInt;
    int j = 0;
    normPoly.state->count = 0;
    for (int i = 0; i < polygon->count; i++)
    {
#if 1
        if (polygon->vertices[i].x == last_x && polygon->vertices[i].y == last_y)
        {   /* Eliminate the repeated vertices, maybe it is unnecessary */
            TRACE("The polygon input has repeated vertex (%d, %d).\n", last_x, last_y);
        }
        else
        {
#endif
            normPoly.state->vertices[j++] = polygon->vertices[i];
            normPoly.state->count++;
#if 1
            last_x = polygon->vertices[i].x;
            last_y = polygon->vertices[i].y;
        }
#endif
    }

    SimpleEdgeFill::Fill(normPoly, ctrl);

    delete[] buf;
    delete normPoly.state;

}

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
 *                2007-06-30, Reimplemented upon qgl template library with 
 *                interface unchanged.
 ******************************************************************************/
void Qgl_SeedFill (QglSmartPixie *pixie, QglInt x, QglInt y)
{
    typedef SeedFill<SeedFillSimpleTraits<QglInt, SeedFillVisit> >    SimpleSeedFill;
    SeedFillVisit visit(pixie);

    SimpleSeedFill::Fill(visit, x, y);
}

#endif

/*------------------------------ End of Module -------------------------------*/


