/**
 * === Containers Library ===
 *  Unit Name:  btree
 *  Author:     Lincoln Yu
 *  Purpose:    B- tree.
 *  Reference:  Data Structures (C Version), Yan Wei-min, Wu Wei-min
 * == History ==        
 *  2005-11-26: Created.
 *  2007-07-09: Removing and 3ing finished. N.T.T.
 *  2007-07-10: [:merge:] on both nodes should return a pointer to merged node.
 *	2007-12-29: A bug in [:btree_node::merge:] found and fixed.
 * == Comments ==
 *
 */

#ifndef _btree_H
#define _btree_H

#include <cassert>
#include <new>

#include "../all/predef.h"
#include "allocators.h"


_QTL_BEGIN


/**
 * the allocator type _Alloc is in standard style
 * and the following methods and types are necessary
 *  (for the prototypes of which the sample allocator above can serve as a reference)
 *  allocate
 *  deallocate
 *  construct
 *  destroy
 *  rebind<T>
 */
template <int _Order, class _Key, class _Val, class _Alloc>
    struct btree_node
{
    typedef btree_node<_Order, _Key, _Val, _Alloc>  _Myt;
    typedef _Myt *              _Ptr;

    typedef _Key &              _KRef;
    typedef const _Key &        _KCref;
    typedef _Val &              _VRef;
    typedef const _Val &        _VCref;

    typedef typename _Alloc::template rebind<_Myt>::other   _MyAlloc;

    /*
     * keys:         K[0]      K[1]      ...      K[n-1]
     *        first_ptr   A[0]      A[1] ... A[2] ...    A[n-1]
     *                                                   
     * keynum = n <= _Order - 1
     */

    struct Pack
    {
        _Ptr    ptr;    /* the pointer on the right of the key */
        _Val    val;

        Pack () : ptr(0)
        {
        }

        Pack (const _Val &v, _Myt *p)
        {
            val = v;
            ptr = p;
        }
    };

    struct iterator
    {
        int index;

        void operator++ ()
        {
            ++index;
        }
    };

    _Ptr        parent;

    _Ptr        first_ptr;
    Pack        packs[_Order];
    int         keynum;

    btree_node ()
        : keynum(0), first_ptr(NULL), parent(NULL)
    {
    }

    btree_node(const btree_node &regular_copy)
    {
        parent = regular_copy.parent;
        keynum = regular_copy.keynum;
        first_ptr = regular_copy.first_ptr;
        packs[0].val = regular_copy.packs[0].val;
        packs[0].ptr = regular_copy.packs[0].ptr;
        if (first_ptr)
        {
            first_ptr->parent = this;
        }
        if (packs[0].ptr)
        {
            packs[0].ptr->parent = this;
        }
    }

    btree_node (_Ptr left, _VCref val, _Ptr right)
    {
        parent = NULL;
        keynum = 1;
        first_ptr = left;
        packs[0].val = val;
        packs[0].ptr = right;
        if (left)
        {
            left->parent = this;
        }
        if (right)
        {
            right->parent = this;
        }
    }
    
    ~btree_node ()
    {
    }

    void clear(_MyAlloc &allocator)
    {
        if (first_ptr)
        {
            allocator.destroy(first_ptr);
            allocator.deallocate(first_ptr);
        }
        for (int i = 0; i < keynum; i++)
        {
            if (packs[i].ptr)
            {
                _Ptr p = packs[i].ptr;
                allocator.destroy(p);
                allocator.deallocate(p);
            }
        }
    }

    _VRef get_val (const iterator &itr)
    {
        return packs[itr.index].val;
    }

    _Ptr get_ptr (const iterator &itr)
    {
        if (itr.index == -1)
        {
            return first_ptr;
        }
        else if (itr.index < keynum)
        {
            return packs[itr.index].ptr;
        }
        else
        {
            return NULL;
        }
    }

    bool is_too_large ()
    {
        return !(keynum < _Order);
    }

    bool is_too_small ()
    {
        return (keynum + 1 < (_Order + 1) / 2);
    }

    bool is_decreasible ()
    {
        return (keynum + 1 > (_Order + 1) / 2);
    }

    bool is_too_small_as_root ()
    {
        return (keynum < 1);
    }   

    _Ptr get_parent ()
    {
        return parent;
    }

    void split_to (_Ptr &ap, _MyAlloc &allocator)
    {
        /* ap is right half of q */
        ap = allocator.allocate(1);
        allocator.construct(ap, _Myt());

        int knm = _Order / 2;
        int srcIdx = knm + 1;
        int apknm = (_Order - 1) / 2;
		assert(keynum == _Order);
        for (int i = 0; i < apknm; ++i, ++srcIdx)
        {
            ap->packs[i] = packs[srcIdx];
            if (packs[srcIdx].ptr)
            {
                packs[srcIdx].ptr->parent = ap;
            }
        }
        ap->first_ptr = packs[knm].ptr;
        if (ap->first_ptr)
        {
            ap->first_ptr->parent = ap;
        }

        ap->keynum = apknm;
        keynum = knm;
    }

    /*
     * If match it returns the actual key index,
     * o.w. the index of child minus one.
     */
    bool search (_KCref k, iterator &itr) const
    {
        if (keynum == 0)
        {
            itr.index = -1;
            return false;
        }

        if (packs[0].val > k)
        {
            itr.index = -1;
            return false;
        }

        /* 
         * find i where packs[i].val <= k < packs[i+1].val 
         * with logerithmic search
         */
        int b = 0;
        int e = keynum;
        while (e > b + 1)
        {
            int m = keynum / 2;
            if (packs[m].val > k)
            {
                e = m;
            }
            else
            {
                b = m;
            }
        }
        itr.index = b;  // the index should be set to i at this point no matter if
                        // the search is successful

        return (packs[b].val == k);
    }

    void insert (const iterator &itr, _VCref v, _Ptr p = NULL)
    {
        /* assert(index >= 0); */
        int index = itr.index;
        index++;    /* the actual insert position is one step after. */
        for (int i = keynum - 1; i >= index; i--)
        {
            packs[i+1] = packs[i];
        }
        packs[index].val = v;
        packs[index].ptr = p;
        if (p)
        {
            p->parent = this;
        }
        keynum++;
    }

    void remove (const iterator &itr)
    {
        int index = itr.index;
        for (int i = index; i < keynum - 1; i++)
        {
            packs[i] = packs[i+1];
        }
        keynum--;
    }
    
    static bool split (_Ptr &p, _MyAlloc allocator)
    {
        /* center key is at index [:floor(_Order/2):] */
        _Ptr ap;
        _Val &v = p->packs[_Order/2].val;
        _Key &x = (_Key)v;
        iterator    itr;
        
        p->split_to(ap, allocator);
        
        _Ptr q = p->parent;
        if (q)
        {
            q->search(x, itr);
            q->insert(itr, v, ap);
            q->packs[itr.index+1].val = v;
            p = q;
            return true;
        }
        else
        {
            _Ptr oldp = p;
            p = allocator.allocate(1);
            allocator.construct(p, _Myt(oldp, v, ap));
            p->packs[0].val = v;
            return false;
        }
    }

    void get_siblings(_Ptr p, iterator &self, iterator &left, iterator &right)
    {
        if (p == first_ptr)
        {
            self.index = -1;
            left.index = keynum;
            right.index = 0;
            return;
        }

        for (int i = 0; i < keynum; i++)
        {
            if (packs[i].ptr == p)
            {
                self.index = i;
                right.index = i+1;
                left.index = i-1;
            }
        }
    }

    static void merge_right (const iterator &left, _Ptr p, const iterator &right)
    {
        _Ptr p_right = p->get_ptr(right);
        _Ptr p_left = p->get_ptr(left);

        p_left->packs[p_left->keynum].val = p->get_val(right);
        p_left->packs[p_left->keynum].ptr = p_right->first_ptr;
        p_left->keynum++;

        p->packs[right.index].val = p_right->packs[0].val;
        p_right->first_ptr = p_right->packs[0].ptr;

        p_right->keynum--;
        for (int i = 0; i < p_right->keynum; i++)
        {
            p_right->packs[i] = p_right->packs[i+1];
        }
        
    }

    static void merge_left (const iterator &left, _Ptr p, const iterator &right)
    {
        _Ptr p_right = p->get_ptr(right);
        _Ptr p_left = p->get_ptr(left);

        for (int i = 0; i < p_right->keynum; i++)
        {
            p_right->packs[i+1] = p_right->packs[i];
        }
        p_right->keynum++;

        p_right->packs[0].ptr = p_right->first_ptr;
        p_right->packs[0].val = p->get_val(right);
        p_right->first_ptr = p_left->packs[p_left->keynum - 1].ptr;

        p->packs[right.index].val = p_left->packs[p_left->keynum - 1].val;

        p_left->keynum--;

    }

    static _Ptr merge (const iterator &left, _Ptr p, const iterator &right, _MyAlloc &allocator)
    {
        _Ptr p_right = p->get_ptr(right);
        _Ptr p_left = p->get_ptr(left);
        int rightknm = p_right->keynum;
		int i;
        
        p_left->packs[p_left->keynum].val = p->get_val(right);
        p_left->packs[p_left->keynum].ptr = p_right->first_ptr;
        for (i = 0; i < p_right->keynum; i++)
        {
            p_left->packs[p_left->keynum + 1 + i] = p_right->packs[i];
        }
        p_left->keynum += rightknm + 1;

        allocator.destroy(p_right);
        allocator.deallocate(p_right);
		
        p->keynum--;
		for (i = right.index; i < p->keynum; i++)
		{
			p->packs[i] = p->packs[i+1];
		}

        return p_left;

    }

    static void lift (_Ptr &node, iterator &itr)
    {
        _Ptr p = node->get_ptr(itr);    /* always lift from right child */
        while (1)
        {
            if (p->first_ptr == NULL)
            {
                node->packs[itr.index].val = p->packs[0].val;

                node = p;
                itr.index = 0;

                break;
            }
            else
            {
                p = p->first_ptr;
            }
        }
    }

};

struct btree_exception
{
    enum Type
    {
        k_ItemNotFound,
    } type;

    btree_exception (Type t) : type(t)
    {
    }
};


template <int _Order, class _KeyTp, class _ValTp>
    struct simple_btree_traits
{
    typedef _KeyTp                          _Key;
    typedef _ValTp                          _Val;
    typedef malloc_allocator<int>           _DummyAlloc;
    typedef btree_node<_Order, _Key, _Val, _DummyAlloc>  _Node;
    typedef typename _Node::_MyAlloc        _Alloc;
};

/**
 * <remarks>
 *  requirements on _KeyTp and _ValTp:
 *  _ValTp must support:
 *    bool operator>(const _KeyTp &);
 *    bool operator==(const _KeyTp &);
 *    operator _KeyTp();
 *  the former two performs comparisons between the embedded key within the value
 *  and the value passed as an parameter; the latter extracts key out of the value
 * </remarks>
 */

template <int _Order, class _KeyTp, class _ValTp, 
    class _Traits = simple_btree_traits<_Order, _KeyTp, _ValTp> >
    class btree : _Traits::_Alloc
{
protected:
    typedef typename _Traits::_Node     _Node;
    typedef typename _Traits::_Key      _Key;
    typedef typename _Traits::_Val      _Val;
    typedef typename _Traits::_Alloc    _Alloc;

    typedef typename _Node::_Ptr        _Ptr;
    typedef typename _Node::iterator    _PackItr;

    typedef const _Key &                _KCref;
    typedef _Val &                      _VRef;
    typedef const _Val &                _VCref;

    typedef _Val *                      _VPtr;
    typedef const _Val *                _VCptr;

protected:
    _Alloc & get_allocator()
    {
        return static_cast<_Alloc&>(*this);
    }

protected:
    bool search (_KCref key, _Ptr &p, _PackItr &itr) const
    {   /* search from node p */
        _Ptr    q = NULL; /* parent of p */

        while (p)
        {
            if (p->search(key, itr))
            {   /* found the key */
                return true;
            }
            else
            {   /* check children */
                q = p;
                p = p->get_ptr(itr);
            }
        }
        p = q;  /* not found the key, but locate children and the iterator for the caller */
        return false;
    }

    void insert (_VCref val, _Ptr &q, const _PackItr &itr)
    {
        _Alloc & allocator = get_allocator();
        if (q)
        {
            q->insert(itr, val);
            while (1)
            {
                if (!q->is_too_large())
                {
                    break;
                }
                else if (!_Node::split(q, allocator))
                {   /* The node being splitted is the root. */
                    m_Root = q;
                    break;
                }
            } 
        }
        else
        {   /* The tree is empty or the root is splitted */
            q = m_Root = allocator.allocate(1);
            allocator.construct(m_Root, _Node(NULL, val, NULL));
        }
    }

    void remove (_Ptr q, const _PackItr &itr)
    {
        _Ptr p = NULL;  /* child of q concerned */
        _Alloc & allocator = get_allocator();
        q->remove(itr);
        while (1)
        {
            if (q == m_Root)
            {
                if (q->is_too_small_as_root())
                {
                    allocator.destroy(m_Root);
                    allocator.deallocate(m_Root);
                    m_Root = p;
                }
                break;
            }

            if (!q->is_too_small())
            {
                break;
            }
            else
            {
                _PackItr    self, left, right;
                _Ptr        p_left, p_right;
                p = q;
                q = q->get_parent();
                q->get_siblings(p, self, left, right);
                p_left = q->get_ptr(left);
                p_right = q->get_ptr(right);

                if (p_right && p_right->is_decreasible())
                {
                    _Node::merge_right(self, q, right);
                    break;
                }
                else if (p_left && p_left->is_decreasible())
                {
                    _Node::merge_left(left, q, self);
                    break;
                }
                else
                {
                    if (p_left)
                    {
                        p = _Node::merge(left, q, self, allocator); /* returns the merged node */
                    }
                    else
                    {
                        p = _Node::merge(self, q, right, allocator);
                    }
                }
            }
        }
    }

public:
    btree () : m_Root(NULL)
    {
    }

    ~btree ()
    {
        if (m_Root)
        {
            _Alloc & allocator = get_allocator();
            m_Root->clear(allocator);
            allocator.destroy(m_Root);
            allocator.deallocate(m_Root);
        }
    }

    bool search (_KCref key, _VRef val) const
    {
        _PackItr    itr;
        _Ptr        node = m_Root;
        if (search(key, node, itr))
        {
            val = node->get_val(itr);
            return true;
        }
        return false;
    }

    void insert (_KCref key, _VCref val)
    {
        _Ptr        node = m_Root;
        _PackItr    itr;
        if (!search(key, node, itr))
        {   /* insert */
            insert(val, node, itr);
        }
        else
        {
            node->get_val(itr) = val;
        }
    }

    bool remove (_KCref key)
    {
        _PackItr    itr;
        _Ptr        node = m_Root;
        _Alloc & allocator = get_allocator();

        if (!search(key, node, itr))
        {
            return false;
        }
        if (node->get_ptr(itr) != NULL)
        {
            _Node::lift(node, itr);
        }
        remove(node, itr);
        return true;
    }

    _VRef operator[] (_KCref key)
    {
        _Ptr        node = m_Root;
        _PackItr    itr;
        if (!search(key, node, itr))
        {   /* insert */
            insert((_Val)key, node, itr);
            search(key, node, itr); /* re-search */
        }
        return node->get_val(itr);
    }

    _VCref operator[] (_KCref key) const
    {
        _Ptr        node = m_Root;
        _PackItr    itr;
        if (search(key, node, itr))
        {
            return node->get_val(itr);
        }
        else
        {
            throw btree_exception(btree_exception::k_ItemNotFound);
        }
    }


protected:
    _Ptr    m_Root;

};  /* class btree */

_QTL_END

#endif  // ifndef _btree_H
