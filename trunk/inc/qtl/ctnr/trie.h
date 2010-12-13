/**
 * === Containers Library ===
 *  Unit Name:  trie
 *  Author:     Lincoln Yu
 *  Purpose:    An TRIAL implementation of trie as exercise.
 *	Reference:	Data Structures and Program Design in C++, by Robert L. Kruse
 *				and Alexander J. Ryba
 * == History ==        
 *  2005-11-26: Created.
 * == Comments ==
 *
 */


#ifndef _QTL_trie_H
#define _QTL_trie_H

#include "../all/predef.h"

#include <vector>


_QTL_BEGIN

using namespace std;

template <class _Tp> class trie
	{
protected:
	struct node
		{
		typedef vector<node *> branches;

		_Tp		    _M_entry;
		branches	_M_branches;

		node() {}

		node(_Tp __data) : _M_entry(__data) {}

		};

	typedef typename node::branches						Branches;
	typedef typename Branches::iterator					BranchesItr;
	typedef typename Branches::reverse_iterator			BranchesRItr;
	typedef typename Branches::const_iterator			BranchesCItr;
	typedef typename Branches::const_reverse_iterator	BranchesCRItr;

    node	*_M_root;

    void clear(node *__p)
		{

		for(BranchesItr it = __p->_M_branches.begin(); it < __p->_M_branches.end(); ++it)
			if(*it)
				clear(*it);
		delete __p;
		}

public:
	trie(node *pNode = NULL) : _M_root(pNode) {}

	virtual ~trie() 
		{
        if(_M_root != NULL)
            clear(_M_root);
	    }
	};

_QTL_END

#endif	// ifndef _QTL_trie_H
