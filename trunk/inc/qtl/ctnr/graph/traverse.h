/**
 * === Containers Library ===
 *  Unit Name:  graph/traverse
 *  Author:     Lincoln Yu
 *  Purpose:    An TRIAL implementation of traverse for graphs.
 *	Reference:	Data Structures and Program Design in C++, by Robert L. Kruse
 *				and Alexander J. Ryba
 *				Basic Computer Network, Gu Shang Jie, Xue Zhi
 * == History ==        
 *  2008-02-28: Created.
 * == Comments ==
 *
 */


#ifndef _traverse_H
#define _traverse_H

#include "../../all/predef.h"


_QTL_BEGIN

namespace graph {

template <class _Graph, class _Stack, class _VisitRef>
static void depth_first (typename _Graph::Vertex &s, _Graph &g, _VisitRef visit)
{
	typedef typename _Graph::Vertex	Vertex;
	typedef typename _Graph::VtxItr	VtxItr;

    Vertex	v = s;
    VtxItr	itr;
    _Stack  stack;
    bool    b;

    visit(v);
    g.set_visited(v);

    if (!(b = g.first_adjacent(v, itr)))
    {
        return;
    }

    while (1)
    {
    go_deep:
        while (b)
        /* the ?_adjacent can be implemented as finding only the unvisited adjacent */
        {
            Vertex a = g.get_vertex(itr);
            if (!g.visited(a))
            {
                visit(a);
                g.set_visited(a);

                b = g.next_adjacent(v, itr);
                VtxItr aitr;
                if (g.first_adjacent(a, aitr))
                {
                    if (b)
                    {
                        stack.push(g, v, itr);
                    }
                    /* [:push:] may check into [:g:] to verify [:a:] is necessary to be pushed */
                    v = a;
                    itr = aitr;
                    b = true;
                    goto go_deep;
                }
            }
            else
            {
                b = g.next_adjacent(v, itr);
            }
        }
        if (!stack)
        {
            break;
        }
        stack.pop(v, itr);
        b = true;
    }
    
}

template <class _Graph, class _Queue, class _VisitRef>
static void breadth_first (typename _Graph::Vertex &s, _Graph &g, _VisitRef visit)
{
	typedef typename _Graph::Vertex	Vertex;
	typedef typename _Graph::VtxItr	VtxItr;

    Vertex	v = s;
    VtxItr	itr;
    _Queue  q;

    visit(v);
    g.set_visited(v);

    if (!g.first_adjacent(v, itr))
    {
        return;
    }

    while (1)
    {
        for (bool b = g.first_adjacent(v, itr); b; b = g.next_adjacent(v, itr))
        {
            Vertex a = g.get_vertex(itr);
            if (!g.visited(a))
            {
                visit(a);
                g.set_visited(a);
                q.push(g, a);
            }
        }
        if (!q)
        {
            break;
        }
        q.pop(v);
    }
}

}   /* namespace graph { */

_QTL_END

#endif  /* ifndef _traverse_H */


/* A sample for test */

#if defined(GRAPH_TRAVERSE_DEBUG) && GRAPH_TRAVERSE_DEBUG

#include <cstdio>
#include <queue>
#include <stack>

using namespace std;

struct Graph
{
	struct Vertex
	{
		int index;
		Vertex () : index(0) {}
		Vertex (int i) : index(i) {}
		Vertex (const Vertex &cpy) : index(cpy.index) {}
	};

	typedef int VtxItr;

    const static int _g[7][7];

    #define VCOUNT  sizeof(_g[0])/sizeof(int)
    bool    _visited[VCOUNT];
    int     _vcount;

    Graph () { reset(); }
    void reset () { _vcount = VCOUNT; memset(_visited, 0, sizeof(bool)*_vcount); }

    bool visited (const Vertex &v) const { return _visited[v.index]; }
    void set_visited (const Vertex &v) { _visited[v.index] = true; }

    bool first_adjacent (const Vertex &v, VtxItr &itr)
    {
        for (int i = 0; i < _vcount; i++)
        {
            if (_g[v.index][i])
            {
                itr = i;
                return true;
            }
        }
        return false;
    }

    bool next_adjacent (const Vertex &v, VtxItr &itr)
    {
        for (int i = itr + 1; i < _vcount; i++)
        {
            if (_g[v.index][i])
            {
                itr = i;
                return true;
            }
        }
        return false;
    }

    Vertex get_vertex (const VtxItr &itr) { return Vertex(itr); }

};  /* class Graph */

const int Graph::_g[7][7] = 
{
    {0,1,1,0,0,0,0},
    {1,0,1,1,1,1,0},
    {0,0,0,1,0,0,0},
    {1,0,0,0,1,0,1},
    {0,0,1,0,0,1,1},
    {0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0},
};

struct Stack
{
	typedef Graph::Vertex	Vertex;
	typedef Graph::VtxItr	VtxItr;

	struct vipair
    {
        Vertex  v;
        VtxItr  itr;
        vipair() {}
        vipair(const vipair &cpy) { v=cpy.v; itr=cpy.itr; }
    };

    stack<vipair>   _stk;

    operator bool() { return !_stk.empty(); }
    void push (const Graph &g, const Vertex &v, const VtxItr &itr) 
    { 
        vipair vp;
        vp.v = v; vp.itr = itr;
        _stk.push(vp); 
    }
    void pop (Vertex &v, VtxItr &itr) 
    { 
        vipair vp = _stk.top(); 
        v = vp.v; itr = vp.itr;
        _stk.pop(); 
    }
};

struct Queue
{
	typedef Graph::Vertex	Vertex;

    queue<Vertex>   _q;

    operator bool() { return !_q.empty(); }
    void push (const Graph &g, const Vertex &v) 
    { 
        _q.push(v);
    }
    void pop (Vertex &v) 
    {
        v = _q.front();
        _q.pop();
    }
};

struct Visit
{
	typedef Graph::Vertex	Vertex;
    void operator() (const Vertex &v)
    {
        printf("visiting %d\n", v.index);
    }
};

int main (void)
{
    using namespace qtl::graph;

	typedef Graph::Vertex	Vertex;

    Visit   visit;
    Vertex  s(0);
    Graph   g;

    printf("depth first\n");
    depth_first<Graph, Stack, Visit&>(s,g,visit);

#if 1
    g.reset();
    printf("breadth first\n");
    breadth_first<Graph, Queue, Visit&>(s,g,visit);
#endif

    return 0;
}

#endif
