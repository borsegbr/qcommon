/**
 * === Containers Library ===
 *  Unit Name:  graph/shrtpath
 *  Author:     Lincoln Yu
 *  Purpose:    An TRIAL implementation of shortest path search for graphs.
 *	Reference:	Data Structures and Program Design in C++, by Robert L. Kruse
 *				and Alexander J. Ryba
 *				Basic Computer Network, Gu Shang Jie, Xue Zhi
 * == History ==        
 *  2008-02-26: Created.
 *  2008-02-28: Use iterator of vertex to make _Graph structure reenterable.
 *  2008-03-01: Merge N system in dijkstra algorithm into one class.
 * == Comments ==
 *				Pay attention to some of the structures' reenterability.
 *				These all three algorithms are applicable for directional graphs with attention
 *              to some specifications.
 */

#ifndef _QTL_shrtpath_H
#define _QTL_shrtpath_H

#if defined(GRAPH_SHRTPATH_DEBUG) && GRAPH_SHRTPATH_DEBUG
#	include <cstdio>
#endif

#include "../../all/predef.h"


_QTL_BEGIN

namespace graph {

/* ford-fulkerson */

template <class _Graph, class _Q, class _Result>
void shpath_ford_fulkerson (const typename _Graph::Vertex &source, _Graph &g, _Result &r)
{
	typedef typename _Graph::Dist	Dist;
	typedef typename _Graph::Vertex	Vertex;

	_Q		q;
	Vertex	v;
	
	q.init(g, source);	/**
							 * set dist of [:source:] to 0
							 * set all dist of pts other than [:source:] to Infinity 
							 * push source into the queue
							 */

    /* for directional g, the direction is either source to any point or opposite
     * which should be indicated and kept in accordance in functions:
     * 1. get_weight(n, f)
     * 2. link(n, f)
     * -- [:n:] to [:f:], [:f:] to [:n:] or bidirection as defined initially
     * where [:n:] is along the path the point closer to source, whereas f is further 
     * away from it.
     */

	while (q)
	{
		bool	more_adj;
		typename _Graph::AdjItr	adjitr;
		q.pop(v);
		for (more_adj = g.first_adjacent(v, adjitr); more_adj; more_adj = g.next_adjacent(v, adjitr))
		{
			Vertex	adj = g.get_adjacent(adjitr);
			if (r.get_dist(v) != _Graph::Infinity())
			{
				Dist d = r.get_dist(v) + g.get_weight(v, adj);
				if (d < r.get_dist(adj))
				{
					r.set_dist(adj, d);
					r.link(v, adj);
					q.push(adj);
				}
			}
		}
	}
}

/* dijkstra */

/*
 *	class _N is unreenterable.
 *	class _N must have the following methods implemented:
 *	- void attach(_Graph &)							// attaching to the graph
 *	- void init_push(Vertex &)						// push vertex to N and last_temp
 *	- bool ext_nonempty()								// having exterior vertices
 *	- void last_temp_push(Vertex &)					// push vertex to last_temp
 *	- bool last_temp_first(Vertex &)				// get first element of last_temp
 *	- bool last_temp_next(Vertex &)					// get next of last_temp
 *	- void temp_update()							// transfer temp to N and last_temp and then clear it
 *	- bool ext_first_adjacent(Vertex &, Vertex &)	// find the first adjacent vertex in exterior graph for vertex
 *	- bool ext_next_adjacent(Vertex &, Vertex &)	// find the next ...
 *	- void temp_clear()								// clear temp
 *	- void temp_push(Vertex &)						// push a vertex to temp
 *	- void temp_push_again(Vertex &)				// push again a vertex with same qualification, or do nothing to leave it for next turn
 */

template < class _Graph, class _N, class _Result >
void shpath_dijkstra (typename _Graph::Vertex &s, _Graph &g, _Result &r)
{
	typedef typename _Graph::Vertex		Vertex;
	typedef typename _Graph::Dist		Dist;

	_N	n;

	n.attach(g);
	n.init_push(s);		/* push s to last temp and n */

	r.set_dist(s, 0);

	Vertex	last, adj;

    /* for directional g, the direction is either s to any point or opposite
     * which should be indicated and kept in accordance in functions:
     * 1. get_weight(n, f)
     * 2. link(n, f)
     * -- [:n:] to [:f:], [:f:] to [:n:] or bidirection as defined initially
     * where [:n:] is along the path the point closer to s, whereas f is further 
     * away from it.
     */

	while (n.ext_nonempty())
	{	/* [:n:] doesn't contain all the vertices of the graph */
		for (bool more_last = n.last_temp_first(last); more_last; more_last = n.last_temp_next(last))
		{
			Dist	min = _Graph::Infinity();
			/* refresh adjacents to [:last:] outside [:n:] */
			for (bool more_adj = n.ext_first_adjacent(last, adj); more_adj; more_adj = n.ext_next_adjacent(last, adj))
			{
				if (r.get_dist(last) == _Graph::Infinity())
				{
					continue;
				}
				Dist d = r.get_dist(last) + g.get_weight(last, adj);
				if (d < r.get_dist(adj))
				{
					r.link(last, adj);
					r.set_dist(adj, d);
				}
				if (r.get_dist(adj) < min)
				{
					min = r.get_dist(adj);
					n.temp_clear();
					n.temp_push(adj);
				}
				else if (r.get_dist(adj) == min)
				{
					n.temp_push_again(adj);	/* do not implement this if process one on each pass */
				}
			}
			n.temp_update();	/* temp is transferred to last temp and pushed to n and then cleared */
		}
	}
}

/*
 * floyd-warshall
 *
 * [:s:] is unreenterable due to indexing i and j.
 */

template <class _Graph, class _S, class _R>
void shpath_floyd_warshall (_Graph &g, _S &s, _R &r)
{
	typedef typename _Graph::VtxItr	VtxItr;
	typedef typename _Graph::Vertex	Vertex;
	typedef typename _Graph::Dist	Dist;

	s.init(g);
	r.init(g);

	bool more_vtx;

#if defined(GRAPH_SHRTPATH_DEBUG) && GRAPH_SHRTPATH_DEBUG
		printf("s=\n");
		s.view();
		printf("r=\n");
		r.view();
#endif

	VtxItr	kitr;
	for (more_vtx = g.first_vertex(kitr); more_vtx; more_vtx = g.next_vertex(kitr))
	{
		bool	b;
		Vertex	i, j;
		Vertex	k = g.get_vertex(kitr);

#if defined(GRAPH_SHRTPATH_DEBUG) && GRAPH_SHRTPATH_DEBUG
		printf("k=%d\n", k+1);
#endif

		for (b = s.first_vertex_pair(i,j); b; b = s.next_vertex_pair(i,j))	/* always j < i */
		{
			/* min(s[i][j], s[i][k] + s[k][j]) */
            Dist sik = s.get(i,k);
            Dist skj = s.get(k,j);
			if (sik != _Graph::Infinity() && skj != _Graph::Infinity())
			{
                Dist sij = s.get(i,j);
				Dist d = sik + skj;
				if (d < sij)
				{
                    s.set(i,j,d);
					r.set(i,j,k);
				}
			}
		}

#if defined(GRAPH_SHRTPATH_DEBUG) && GRAPH_SHRTPATH_DEBUG
		printf("s=\n");
		s.view();
		printf("r=\n");
		r.view();
#endif
	}
}

} /* namespace graph { */

_QTL_END

#endif	// ifndef _QTL_shrtpath_H


/* A sample for test */

#if defined(GRAPH_SHRTPATH_DEBUG) && GRAPH_SHRTPATH_DEBUG

#include <cstdlib>
#include <cstdio>

#include <vector>
#include <queue>

using namespace std;

#define GRAPH_SIZE	10


struct Digraph
{
	typedef int		Vertex;
	typedef unsigned long Dist;
	typedef Vertex	VtxItr;
	typedef Vertex	AdjItr;

	Digraph (int vcnt = 0)
	{
		vcount = vcnt;
		for (int i = 0; i < vcount; i++)
		{
			for (int j = 0; j < vcount; j++)
			{
				adjacency[i][j] = (i != j)? Infinity() : 0;
			}
		}
	}

	static Dist Infinity ()
	{
		return 0xFFFFFFFF;
	}

	void init_dist (const Vertex &src)
	{
		for (Vertex v = 0; v < vcount; v++)
		{
			if (v != src)
			{
				dist[v] = Infinity();
			}
		}
		dist[src] = 0;
	}

	Dist get_dist (const Vertex &vtx) const
	{
		return dist[vtx];
	}

	void set_dist (const Vertex &vtx, const Dist &d)
	{
		dist[vtx] = d;
	}

	Dist get_weight (const Vertex &v1, const Vertex &v2) const
	{
		return adjacency[v1][v2];
	}

	void set_weight (const Vertex &v1, const Vertex &v2, Dist w)
	{
		adjacency[v1][v2] = adjacency[v2][v1] = w;
	}

	void link (const Vertex &v1, const Vertex &v2)
	{
		links[v2] = v1;
	}

	bool first_adjacent (const Vertex &vtx, AdjItr &adj)
	{
		// for NNc
		adj = -1;
		return next_adjacent(vtx, adj);
	}

	bool next_adjacent (const Vertex &vtx, AdjItr &adj)
	{
		// for NNc
		for (adj++ ; adj < vcount; adj++)
		{
			if (adjacency[vtx][adj] != Infinity() && vtx != adj)
			{
				return true;
			}
		}
		return false;
	}

	bool first_vertex (VtxItr &v)
	{
		v = -1;
		next_vertex(v);
	}

	bool next_vertex (VtxItr &v)
	{
		v++;
		return (v < vcount);
	}

	Vertex & get_vertex (VtxItr &v) { return v; }

	Vertex & get_adjacent (AdjItr &a) { return a; }	

	Dist	adjacency[GRAPH_SIZE][GRAPH_SIZE];
	Dist	dist[GRAPH_SIZE];
	Vertex	links[GRAPH_SIZE];
	int		vcount;
};

//////////
// for ford

struct Q
{
	typedef Digraph::Vertex	Vertex;

	bool	q[GRAPH_SIZE];
	int		vcount;
	int		qcount;

	void init (Digraph &graph, const Vertex &source)
	{
		qcount = 0;
		graph.init_dist(source);
		vcount = graph.vcount;
		for (int i = 0; i < vcount; i++)
		{
			q[i] = false;
		}
		push(source);
	}

	void push (const Vertex &v) 
	{ 
		if (q[v] == false)
		{
			qcount++;
		}
		q[v] = true;
	}

	void pop (Vertex &v) 
	{
		for (v = 0; v < vcount; v++)
		{
			if (q[v])
			{
				q[v] = false;
				qcount--;
				break;
			}
		}
	}

	operator bool () { return qcount > 0; }

	void view ()
	{
		printf("qcount = %d\n", qcount);
		for (int i = 0; i < vcount; i++)
		{
			if (q[i])
			{
				printf("%d ", i+1);
			}
		}
		printf("\n");
	}
};

//////////
// for dijkstra

/*
 *	class _N is unreenterable.
 *	class _N must have the following methods implemented:
 *	- void attach(_Graph &)							// attaching to the graph
 *	- void init_push(Vertex &)						// push vertex to N and last_temp
 *	- bool ext_nonempty()							// having exterior vertices
 *	- void last_temp_push(Vertex &)					// push vertex to last_temp
 *	- bool last_temp_first(Vertex &)				// get first element of last_temp
 *	- bool last_temp_next(Vertex &)					// get next of last_temp
 *	- void temp_update()							// transfer temp to N and last_temp and then clear it
 *	- bool ext_first_adjacent(Vertex &, Vertex &)	// find the first adjacent vertex in exterior graph for vertex
 *	- bool ext_next_adjacent(Vertex &, Vertex &)	// find the next ...
 *	- void temp_clear()								// clear temp
 *	- void temp_push(Vertex &)						// push a vertex to temp
 *	- void temp_push_again(Vertex &)				// push again a vertex with same qualification, or do nothing to leave it for next turn
 */

struct N
{
	typedef	Digraph::Vertex	Vertex;
	vector<Vertex>	last_temp, temp;
	int				cur;
	bool			found[GRAPH_SIZE];
	int				found_count;
	Digraph			*pGraph;

	void attach (Digraph &g)
	{
		pGraph = &g;
		for (int i = 0; i < g.vcount; i++)
		{
			found[i] = false;
			g.dist[i] = Digraph::Infinity();
		}
		found_count = 0;
	}
	bool ext_nonempty ()
	{
		return (found_count < pGraph->vcount);
	}
	void init_push (Vertex &s)
	{
		found[s] = true;
		found_count = 1;
		last_temp.push_back(s);
	}
	bool last_temp_first (Vertex &v)
	{
		cur = -1;
		return last_temp_next(v);
	}
	bool last_temp_next (Vertex &v)
	{
		cur++;
		if (cur >= last_temp.size()) { return false; }
		v = last_temp[cur];
		return true;
	}
	bool ext_first_adjacent (const Vertex &vtx, Vertex &adj)
	{
		adj = -1;
		return ext_next_adjacent(vtx, adj);
	}
	bool ext_next_adjacent (const Vertex &vtx, Vertex &adj)
	{
		for (adj++ ; adj < pGraph->vcount; adj++)
		{
			if (!found[adj] && adj != vtx)
			{
				if (pGraph->adjacency[vtx][adj] != Digraph::Infinity())
				{
					return true;
				}
			}
		}
		return false;
	}
	void temp_update ()
	{
		last_temp = temp;
		temp.clear();

		for (int i = 0; i < last_temp.size(); i++)
		{
			found[last_temp[i]] = true;
		}
		found_count += last_temp.size();
	}
	void temp_clear ()
	{
		temp.clear();
	}
	void temp_push(Vertex &v)
	{
		temp.push_back(v);
	}
	void temp_push_again(Vertex &v)
	{
		temp_push(v);
	}
};

/////////
// floyd

struct S
{
	typedef Digraph::Dist	Dist;
	typedef Digraph::Vertex	Vertex;

	Dist	_s[GRAPH_SIZE][GRAPH_SIZE];
	int		vcount;

	void init (const Digraph &g)
	{
		vcount = g.vcount;
		for (int i = 0; i < vcount; i++)
		{
			for (int j = 0; j < vcount; j++)
			{
				_s[i][j] = g.adjacency[i][j];
			}
		}
	}

	bool first_vertex_pair (Vertex &i, Vertex &j)
	{
		if (vcount == 0)
		{
			return false;
		}
		i = 1, j = 0;
		return true;
	}

	bool next_vertex_pair (Vertex &i, Vertex &j)
	{
		if (j < i - 1)
		{
			j++;
			return true;
		}
		else if (i < vcount - 1)
		{
			j = 0;
			i++;
			return true;
		}
		else
		{
			return false;
		}
	}

	void set (int i, int j, const Dist &w)
	{
		_s[i][j] = w;	/* bi-directional */
		_s[j][i] = w;
	}

	Dist & get (int i, int j)
	{
		return _s[i][j];
	}

	void view ()
	{
		for (int i = 0; i < vcount; i++)
		{
			for (int j = 0; j < vcount; j++)
			{
				printf("%2d ", _s[i][j]);
			}
			printf("\n");
		}
	}
};

struct R
{
	typedef Digraph::Vertex	Vertex;

	Vertex	_r[GRAPH_SIZE][GRAPH_SIZE];
	int		vcount;

	void init (const Digraph &g)
	{
		vcount = g.vcount;
		for (int i = 0; i < vcount; i++)
		{
			for (int j = 0; j < vcount; j++)
			{
				if (g.adjacency[i][j] == Digraph::Infinity() || g.adjacency[i][j] == 0)
				{
					_r[i][j] = -1;	
				}
				else
				{
					_r[i][j] = j;
				}
			}
		}
	}

	void set (Vertex i, Vertex j, Vertex k)
	{
		_r[i][j] = _r[i][k];
		_r[j][i] = _r[j][k];
	}

	void view ()
	{
		for (int i = 0; i < vcount; i++)
		{
			for (int j = 0; j < vcount; j++)
			{
				printf("%2d ", _r[i][j] + 1);
			}
			printf("\n");
		}
	}
};

/////////
// test base

void SHOW_DIST (Digraph &graph, Digraph::Vertex source)
{
	source--;
	for (int v = 0; v < graph.vcount; v++)
	{
		if (v != source )
		{
			printf("(p,d)[%d] = (%d,%d)\n", v+1, graph.links[v] + 1, graph.dist[v]);
		}
	}
}

int main ()
{
	using namespace qtl::graph;

	typedef Digraph::Vertex	Vertex;

	#define SET_GRAPH_WEIGHT(v1,v2,w)	g.set_weight(v1-1,v2-1,w)
	#define SEARCH_DIJKSTRA(v)			{ Vertex s = v-1; shpath_dijkstra< Digraph, N, Digraph >(s,g,g); }
	#define SEARCH_FORD(v)				shpath_ford_fulkerson< Digraph, Q, Digraph >((Vertex)(v-1),g,g)

	Digraph	g(6);
	SET_GRAPH_WEIGHT(1,2,2);
	SET_GRAPH_WEIGHT(1,4,1);
	SET_GRAPH_WEIGHT(2,3,3);
	SET_GRAPH_WEIGHT(2,4,2);
	SET_GRAPH_WEIGHT(4,3,3);
	SET_GRAPH_WEIGHT(1,3,5);
	SET_GRAPH_WEIGHT(4,5,1);
	SET_GRAPH_WEIGHT(3,5,1);
	SET_GRAPH_WEIGHT(3,6,5);
	SET_GRAPH_WEIGHT(5,6,2);

#if 1
	printf("testing dikstra...\n");
	SEARCH_DIJKSTRA(1);
	SHOW_DIST(g, 1);
#endif

#if 0
	printf("testing ford...\n");
	SEARCH_FORD(1);
	SHOW_DIST(g, 1);
#endif

#if 0
	S s;
	R r;
	printf("testing floyd...\n");
	shpath_floyd_warshall<Digraph, S, R>(g, s, r);
	printf("s = \n");
	s.view();
	printf("r = \n");
	r.view();
#endif
	return 0;
}


#endif	/* if defined(GRAPH_SHRTPATH_DEBUG) && GRAPH_SHRTPATH_DEBUG */

