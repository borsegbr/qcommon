/**
 * === Containers Library ===
 *  Unit Name:  graph/spantree
 *  Author:     Lincoln Yu
 *  Purpose:    Minimum spanning tree generation for graphs.
 *	Reference:	Data Structures and Program Design in C++, by Robert L. Kruse
 *				and Alexander J. Ryba
 *				Basic Computer Network, Gu Shang Jie, Xue Zhi
 * == History ==        
 *  2008-02-28: Created.
 * == Comments ==
 *  The graph upon which spanning tree is generated must be bi-directional	
 */


#ifndef _spantree_H
#define _spantree_H

#include "../../all/predef.h"

_QTL_BEGIN

namespace graph {


/*
 * _STree is not reenterable
 */

template <class _Graph, class _STree>
void prim (typename _Graph::Vertex &s, _Graph &g, _STree &st)
{
    typedef typename _Graph::Vertex  Vertex;
    typedef typename _Graph::Dist    Dist;

    st.attach(g);
    
    st.init_add(s);
    Vertex  v = s;

    while (st.unfinished())
    {
        bool    more_vtx;
        Dist    min_d = _Graph::Infinity();
        Vertex  vchk;
        Vertex  sel;
        for (more_vtx = st.ext_first_vertex(vchk); more_vtx; more_vtx = st.ext_next_vertex(vchk))
        {
			Dist w = g.get_weight(v, vchk);
			Dist d = st.get_dist(vchk);

			if (w != _Graph::Infinity() && w < d)
			{
				st.set_link(v, vchk, w);
				d = w;
			}
			if (d != _Graph::Infinity() && d < min_d)
			{
				sel = vchk;
				min_d = d;
			}
        }
        st.add(sel);
        v = sel;
    }
}

template <class _Graph, class _STree>
void kruskal (_Graph &g, _STree &st)
{
	typedef typename _Graph::Vertex	Vertex;

	
}

}   /* namespace graph { */

_QTL_END

#endif	/* if _spantree_H */


/* A sample for test */

#if defined(GRAPH_SPANTREE_DEBUG) && GRAPH_SPANTREE_DEBUG

#include <cstdio>

struct Graph
{
	typedef int Vertex;
	typedef int VtxItr;
	typedef unsigned long Dist;

    static const int g[10][10];

    #define VCOUNT  (sizeof(Graph::g[0])/sizeof(int))
    int     vcount;

	static Dist Infinity() { return 0xFFFFFFFF; }

    Graph () { reset(); }
    void reset () { vcount = VCOUNT;  }

	Dist get_weight (const Vertex &v, const Vertex &u) { return (Dist)g[v][u]; }


};  /* class Graph */

const int Graph::g[10][10] = {
    { 0, 6, 3,-1, 9,-1,-1,-1,-1,-1 },
    { 6, 0, 4, 2,-1,-1, 9,-1,-1,-1 },
    { 3, 4, 0, 2, 9, 9,-1,-1,-1,-1 },
    {-1, 2, 2, 0,-1, 8, 9,-1,-1,-1 },
    { 9,-1, 9,-1, 0, 8,-1,-1,-1,18 },
    {-1,-1, 8, 9, 8, 0, 7,-1, 9,10 },
    {-1, 9,-1, 9,-1, 7, 0, 4, 5,-1 },
    {-1,-1,-1,-1,-1,-1, 4, 0, 1, 4 },
    {-1,-1,-1,-1,-1, 9, 5, 1, 0, 3 },
    {-1,-1,-1,-1,18,10,-1, 4, 3, 0 }
};

struct STree
{
	typedef Graph::Dist		Dist;
	typedef Graph::Vertex	Vertex;

	bool	stree[VCOUNT];
	Dist	dist[VCOUNT];
	Vertex	link[VCOUNT];
	int		cur;
	Graph	*pGraph;
	int		stree_count;

	void init_add (Vertex &s)
	{
		stree[s] = true;
		stree_count++;
		link[s] = s;
		dist[s] = 0;
	}
	void add(Vertex &v)
	{
		printf("linking %d, %d\n", link[v], v);
		stree_count++;
		stree[v] = true;
	}
	bool unfinished ()
	{
		return stree_count < pGraph->vcount;
	}
	void attach (Graph &g)
	{
		pGraph = &g;
		stree_count = 0;
		for (int i = 0; i < VCOUNT; i++)
		{
			stree[i] = false;
			link[i] = 0;
			dist[i] = Graph::Infinity();
		}
	}
	bool ext_first_vertex (Vertex &v)
	{
		v = -1;
		return ext_next_vertex(v);
	}
	bool ext_next_vertex (Vertex &v)
	{
		for (v++; v < pGraph->vcount; v++)
		{
			if (!stree[v])
			{
				return true;
			}
		}
		return false;
	}

	Dist get_dist (const Vertex &v) const
	{
		return dist[v];
	}
	void set_link (const Vertex &v, const Vertex &t, Dist d) 
	{
		link[t] = v;
		dist[t] = d;
	}
};

int main ()
{
	Graph	g;
	STree	st;
	Graph::Vertex	v = 0;

	using namespace qtl::graph;

	prim<Graph, STree>(v, g, st);
	return 0;
}


#endif	/* if defined(GRAPH_SPANTREE_DEBUG) && GRAPH_SPANTREE_DEBUG */


