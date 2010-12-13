/**
 * === Containers Library ===
 *  Unit Name:  stltree
 *  Author:     Lincoln Yu
 *  Purpose:    Encapsulated PJ-STL red-black tree for simple usage
 *
 * == History ==        
 *  2005-11-26: Created.
 * == Comments ==
 *	
 */

#ifndef _QTL_stltree_H
#define _QTL_stltree_H

#include "../all/predef.h"
#include <xtree>			// P.J.
#include <functional>

_QTL_BEGIN

using namespace std;

template < class _Key, class _Val = _Key, class _LessKey = less<_Key>, class _LessVal = less<_Val> >
	struct stl_tree
	{
		struct _Stl_Tree_Traits
			{
		typedef _LessKey	key_compare;
		typedef _LessVal	value_compare;
		typedef _Key		key_type;
		typedef	_Val		value_type;
		typedef typename allocator<_Key>::template rebind<value_type>::other	allocator_type;
		typedef _POINTER_X(value_type, allocator_type)		_ITptr;
		typedef _REFERENCE_X(value_type, allocator_type)	_IReft;
		
		enum
			{	// make multi parameter visible as an enumeration constant
			_Multi = false};

		_Stl_Tree_Traits()
			: comp()
			{	// construct with default comparator
			}

		_Stl_Tree_Traits(key_compare __comp)
			: comp(__comp)
			{	// consturct with assigned comparator
			}

		static const _Key& _Kfn(const value_type &__val)
			{	// extract key from element value
			return (__val);
			}
		
		key_compare comp;			

			};	// struct _Stl_Tree_Traits

	class embody : public _Tree< _Stl_Tree_Traits >
		{
		typedef _Tree< _Stl_Tree_Traits >		_Super;
		typedef typename _Super::key_compare	key_compare;

	public:
		embody(const key_compare &__comp = key_compare())
			: _Super(__comp, allocator_type())
			{	// construct empty tree
			}	

		};	// class tree

	};	// class stl_tree




_QTL_END

#endif	// ifndef _QTL_stltree_H
