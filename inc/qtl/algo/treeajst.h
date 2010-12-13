/**
 * === Standard Algorithm Library ===
 *  Unit Name:  treeajst
 *  Author:     Lincoln Yu
 *  Purpose:    Balance adjuster for binary search tree (e.g. AVL tree).
 *  Reference:  Data Structures and Program Design in C++, by Robert L. Kruse
 *              and Alexander J. Ryba
 *              Data Structure (in C Programming Language), by Yan Wei-min and Wu Wei-min, 
 *              published by Tsinghua University Press.
 * == History ==        
 *  2005-12-23: Created.
 *  2005-12-26: Roughly debugged.
 *  2009-04-12: Partly re-implemented (mainly the replacement routines) and roughly tested.
 * == Comments ==
 *
 */

#ifndef _treeadjst_H
#define _treeadjst_H

#include "../all/predef.h"

_QTL_BEGIN


template <class _Traits>
    class bstree_adjuster : public _Traits
    {
protected:
    typedef typename _Traits::node_itr      NodeItr;
    typedef typename _Traits::data_type     _DataTyp;

public:
    template <class _OtherTraits>
    struct rebind
        {
        typedef bstree_adjuster<_OtherTraits> other;
        };

// common

    NodeItr leftmost (NodeItr __node)
        {
        while (!_isnil(_left(__node)))
            __node = _left(__node);
        return (__node);
        }

    NodeItr rightmost (NodeItr __node)
        {
        while (!_isnil(_right(__node)))
            __node = _right(__node);
        return (__node);
        }


    #define _TREEAJST_DEEPMOST(a, b)                                                    \
        while (true)                                                                    \
            {                                                                           \
            if (!_isnil(_##a##(__node)))                                                \
                __node = _##a##(__node);                                                \
            else if (!_isnil(_##b##(__node)))                                           \
                __node = _##b##(__node);                                                \
            else                                                                        \
                break;                                                                  \
            }                                                                           \
        return (__node);


    NodeItr leftdeepmost (NodeItr __node)
        {
        _TREEAJST_DEEPMOST(left, right)
        }

    NodeItr rightdeepmost (NodeItr __node)
        {
        _TREEAJST_DEEPMOST(right, left)
        }

//  with parent

    #define _TREEAJST_PREORDER(a, b)                                                    \
        if (_isnil(__node))                                                             \
            ;                                                                           \
        else if (!_isnil(_##a##(__node)))                                               \
            __node = _##a##(__node);                                                    \
        else if (!_isnil(_##b##(__node)))                                               \
            __node = _##b##(__node);                                                    \
        else                                                                            \
            {                                                                           \
            NodeItr __parent;                                                           \
            while (!_isnil(__parent = _parent(__node)))                                 \
                {                                                                       \
                if (__node == _##a##(__parent))                                         \
                    if (!_isnil(__node = _##b##(__parent)))                             \
                        return;                                                         \
                __node = __parent;                                                      \
                }                                                                       \
            __node = __parent;                                                          \
            }


    void preorder_inc (NodeItr &__node)
        {
        _TREEAJST_PREORDER(left, right)
        }

    void preorder_dec (NodeItr &__node)
        {
        _TREEAJST_PREORDER(right, left)
        }

    #define _TREEAJST_INORDER(a, b)                                                     \
        if (_isnil(__node))                                                             \
            ;                                                                           \
        else if (!_isnil(_##b##(__node)))                                               \
            __node = ##a##most(_##b##(__node));                                         \
        else                                                                            \
            {                                                                           \
            NodeItr __parent;                                                           \
            while (!_isnil(__parent = _parent(__node))                                  \
                && __node == _##b##(__parent))                                          \
                __node = __parent;                                                      \
            __node = __parent;                                                          \
            }


    void inorder_inc (NodeItr &__node)
        {
        _TREEAJST_INORDER(left, right)
        }

    void inorder_dec (NodeItr &__node)
        {
        _TREEAJST_INORDER(right, left)
        }

    #define _TREEAJST_POSTORDER(a, b)                                                   \
        NodeItr __parent = _parent(__node);                                             \
        if (!_isnil(__parent))                                                          \
            {                                                                           \
            if ( __node == _##b##(__parent) || _isnil(_##b##(__parent)) )               \
                {                                                                       \
                __node = __parent;                                                      \
                }                                                                       \
            else                                                                        \
                {                                                                       \
                __node = ##a##deepmost(_##b##(__parent));                               \
                }                                                                       \
            }                                                                           \
        else __node = __parent;

    void postorder_inc (NodeItr &__node)
        {
        _TREEAJST_POSTORDER(left, right)
        }

    void postorder_dec (NodeItr &__node)
        {
        _TREEAJST_POSTORDER(right, left)
        }

    /*
     * tree rotation
     */

    #define _TREEAJST_ROTATE(a, b)                                                      \
        NodeItr __##b##_tree = _##b##(__subroot);                                       \
        _##b##(__subroot) = _##a##(__##b##_tree);                                       \
        _##a##(__##b##_tree) = __subroot;                                               \
        if (WithParent) {                                                               \
            NodeItr &__parent = _parent(__##b##_tree) = _parent(__subroot);             \
            if(!_isnil(_##b##(__subroot)))                                              \
                _parent(_##b##(__subroot)) = __subroot;                                 \
            _parent(__subroot) = __##b##_tree;                                          \
            if(!_isnil(__parent))                                                       \
                {                                                                       \
                if (__subroot == _##a##(__parent))                                      \
                    {                                                                   \
                    _##a##(__parent) = __##b##_tree;                                    \
                    }                                                                   \
                else                                                                    \
                    {                                                                   \
                    _##b##(__parent) = __##b##_tree;                                    \
                    }                                                                   \
                }                                                                       \
            }                                                                           \
        __subroot = __##b##_tree;


    template <bool WithParent>
    void rotate_left (NodeItr &__subroot)
        {
        _TREEAJST_ROTATE(left, right)
        }
    template <bool WithParent>
    void rotate_right (NodeItr &__subroot)
        {
        _TREEAJST_ROTATE(right, left)
        }

    int search (NodeItr &__node, const _DataTyp &__data)
        {
        assert(!_isnil(__node));
        int __cmp;
        NodeItr __parent;
        do
            {
            __cmp = _comp(__data, _data(__node));
            __parent = __node;
            if ( __cmp < 0 )
                __node = _left(__node);
            else if ( __cmp > 0 )
                __node = _right(__node);
            else
                return 0;
            } while (!_isnil(__node));
        __node = __parent;
        return ((__cmp > 0)? 1:(-1));
        }

    template <class _KeyTp, class _Comp>
    int search(NodeItr &node, const _KeyTp &__key)
        {
        assert(!_isnil(__node));
        int __cmp;
        NodeItr __parent;
        do
            {
            __cmp = _Comp(__key, _data(__node));
            __parent = __node;
            if ( __cmp > 0 )
                __node = _left(__node);
            else if ( __cmp < 0 )
                __node = _right(__node);
            else
                return 0;
            } while (!_isnil(__node));
        __node = __parent;
        return ((__cmp > 0)? 1:(-1));
        }

    };  // class bstree_adjuster

template <class _Traits>
    class avl_adjuster : public bstree_adjuster<_Traits>
    {
protected:
    typedef typename _Traits::node_itr          NodeItr;
    typedef typename _Traits::data_type         _DataTyp;
    typedef typename _Traits::balance_base      _BalBase;
    typedef typename _Traits::balance_factor    _BalFctr;
    typedef typename _Traits::allocator         _Alloc;

public:
    template <class _OtherTraits>
    struct rebind
        {
        typedef avl_adjuster<_OtherTraits> other;
        };

    /*
     * .. Non-recursive approaches
     */

    /*
     * <summary>
     *  adjust around the node (after node insertion or removal)
     *  Pre: node balance factor from __subroot down to its leaves must have been 
     *       correctly obtained, and the __subroot is adjustable in AVL-tree sense
     *       i.e. with degree of imbalance on it being 2 and that on all its subnodes
     *       being no more than 1.
     * </summary>
     */

    #define _TREEAJST_AVL_BALANCE(a, b)                                                 \
        NodeItr &__##a##_tree = _##a##(__subroot);                                      \
        NodeItr __subtree;                                                              \
        switch ( _get_balance(__##a##_tree) )                                           \
            {                                                                           \
            case _BalBase::##a##_higher:                                                \
                _set_balance(__subroot, _BalBase::equal_height);                        \
                _set_balance(__##a##_tree, _BalBase::equal_height);                     \
                rotate_##b<WithParent>(__subroot);                                      \
                break;                                                                  \
            case _BalBase::equal_height:                                                \
                _set_balance(__subroot, _BalBase::##a##_higher);                        \
                _set_balance(__##a##_tree, _BalBase::##b##_higher);                     \
                rotate_##b<WithParent>(__subroot);                                      \
                break;                                                                  \
            case _BalBase::##b##_higher:                                                \
                __subtree = _##b##(__##a##_tree);                                       \
                switch ( _get_balance(__subtree) )                                      \
                    {                                                                   \
                    case _BalBase::equal_height:                                        \
                        _set_balance(__subroot, _BalBase::equal_height);                \
                        _set_balance(__##a##_tree, _BalBase::equal_height);             \
                        break;                                                          \
                    case _BalBase::##b##_higher:                                        \
                        _set_balance(__subroot, _BalBase::equal_height);                \
                        _set_balance(__##a##_tree, _BalBase::##a##_higher);             \
                        break;                                                          \
                    case _BalBase::##a##_higher:                                        \
                        _set_balance(__subroot, _BalBase::##b##_higher);                \
                        _set_balance(__##a##_tree, _BalBase::equal_height);             \
                        break;                                                          \
                    }                                                                   \
                _set_balance(__subtree, _BalBase::equal_height);                        \
                rotate_##a<WithParent>(__##a##_tree);                                   \
                rotate_##b<WithParent>(__subroot);                                      \
                break;                                                                  \
            }

    template <bool WithParent>
    void left_balance (NodeItr &__subroot)
        {
        _TREEAJST_AVL_BALANCE(left, right)
        }
    template <bool WithParent>
    void right_balance (NodeItr &__subroot)
        {
        _TREEAJST_AVL_BALANCE(right, left)
        }

    /*
     * <summary>
     *  add a node to a tree with parent pointers
     *  adjust after modifications if necessary
     *  Requirements:
     *   parent node must be present
     *   balance factor required
     *  arguments:
     *   __new_node: the node that has been newly added
     *  Pre: it is the __new_node's duty to set its initial balance factor (to _BalBase::equal_height)
     * </summary>
     * <remarks>
     *  the modification of balance factor does not further propogate upward after rotation
     * </remarks>
     */
    void insert_adjust (NodeItr __new_node)
        {
        NodeItr __son = __new_node, __cur;
        bool __taller = true;
        for ( ; !_isnil(__cur = _parent(__son)) && __taller; __son = __cur)
            {
            bool __is_left = (__son == _left(__cur));
            switch (_get_balance(__cur))
                {
                case _BalBase::left_higher:
                    if (__is_left) left_balance<true>(__cur);
                    else _set_balance(__cur, _BalBase::equal_height);
                    __taller = false;
                    break;
                case _BalBase::equal_height:
                    _set_balance(__cur, (__is_left)?_BalBase::left_higher:_BalBase::right_higher);
                    __taller = true;
                    break;
                case _BalBase::right_higher:
                    if (!__is_left) right_balance<true>(__cur);
                    else _set_balance(__cur, _BalBase::equal_height);
                    __taller = false;
                    break;
                }
            }
        }

    /*
     * <summary>
     *  remove a node from a tree with parent pointers
     *  adjust after modifications if necessary
     *  Requirements:
     *   parent node must be present
     *  arguments:
     *   __sad_parent:  node one of whose children has gone
     *   __is_left:     if the child is on the left
     *  Pre:
     *   with a child removed off, the balance factor of the node __sad_parent
     *   is no longer valid, and is updated here (after a possible adjustment)
     * </summary>
     */
    void remove_adjust (NodeItr __sad_parent, bool __is_left)
        {
        assert(!_isnil(__sad_parent));

        NodeItr __son, __cur = __sad_parent;
        bool __shorter = true;
        while(1)
            {
            switch (_get_balance(__cur))
                {
                case _BalBase::left_higher:
                    if (__is_left)
                        {
                        _set_balance(__cur, _BalBase::equal_height);
                        __shorter = true;
                        }
                    else
                        {
                        __shorter = (_get_balance(_left(__cur)) != _BalBase::equal_height);
                        left_balance<true>(__cur);
                        }
                    break;
                case _BalBase::equal_height:
                    _set_balance(__cur, (__is_left)?_BalBase::right_higher:_BalBase::left_higher);
                    __shorter = false;
                    break;
                case _BalBase::right_higher:
                    if (!__is_left)
                        {
                        _set_balance(__cur, _BalBase::equal_height);
                        __shorter = true;
                        }
                    else
                        {
                        __shorter = (_get_balance(_right(__cur)) != _BalBase::equal_height);
                        right_balance<true>(__cur);
                        }
                    break;
                }
                __son = __cur;
                __cur = _parent(__son);
                if (_isnil(__cur) || !__shorter)
                    break;
                __is_left = (__son == _left(__cur));
            }
        }

    /*
     * .... node replacement for removal, `resignment' algorithm
     */

    /*
     * <summary>
     *  replace __start node with the node left to it by index (pointer operation)
     *  __start node is removed from the tree
     *  requirements:
     *   parent pointer required
     *  arguments:
     *   __start -- as input:  the node to be removed, which is taken off the tree structure 
     *                         by this routine
     *              as output: the node that takes its place
     *   __sad_parent -- the node where the re-balancing is started from
     *   __is_left -- whether the removed node is the left child of the sad parent
     *     the latter two arguments are exactly those fed to the routine remove_adjust
     * </summary>
     * <remarks>
     *  at the end of the routine, the properties of the node removed are copied to the
     *  node that takes its place by invoking the routine _image_copy
     * </remarks>
     */

    #define _TREEAJST_REPLACE_BYPOINTER(a, b)                               \
        NodeItr __p = _##a(__start);                                        \
        if (_isnil(__p)) { __start = _nil(); return; }                      \
        for ( ; !_isnil(_##b(__p)); __p = _##b(__p) );                      \
        if (_isnil(_##a(__p)))                                              \
            {                                                               \
            if (_parent(__p) == __start)                                    \
                {                                                           \
                __sad_parent = __p;                                         \
                __is_##a = true;                                            \
                _##a(__p) = _nil();                                         \
                _##b(__p) = _##b(__start);                                  \
                if (!_isnil(_##b(__p)))                                     \
                    _parent(_##b(__p)) = __p;                               \
                }                                                           \
            else                                                            \
                {                                                           \
                __sad_parent = _parent(__p);                                \
                __is_##a = false;                                           \
                _##b(__sad_parent) = _nil();                                \
                _##a(__p) = _##a(__start);                                  \
                _parent(_##a(__p)) = __p;                                   \
                _##b(__p) = _##b(__start);                                  \
                if (!_isnil(_##b(__p)))                                     \
                    _parent(_##b(__p)) = __p;                               \
                }                                                           \
            }                                                               \
        else                                                                \
            {                                                               \
            __sad_parent = _##a(__p);                                       \
            __is_##a = true;                                                \
            _image_copy(__sad_parent, __p);                                 \
            if (_parent(__p) == __start)                                    \
                {                                                           \
                _##b(__p) = _##b(__start);                                  \
                if (!_isnil(_##b(__p)))                                     \
                    _parent(_##b(__p)) = __p;                               \
                }                                                           \
            else                                                            \
                {                                                           \
                _parent(__sad_parent) = _parent(__p);                       \
                _##b(_parent(__sad_parent)) = __sad_parent;                 \
                _##a(__p) = _##a(__start);                                  \
                _parent(_##a(__p)) = __p;                                   \
                _##b(__p) = _##b(__start);                                  \
                if (!_isnil(_##b(__p)))                                     \
                _parent(_##b(__p)) = __p;                                   \
                }                                                           \
            }                                                               \
        _parent(__p) = _parent(__start);                                    \
        if (!_isnil(_parent(__p))) {                                        \
            if (_left(_parent(__p))==__start) _left(_parent(__p)) = __p;    \
            else _right(_parent(__p)) = __p;                                \
            }                                                               \
        _image_copy(__p, __start); __start = __p;

    void replace_left_bypointer(NodeItr &__start, NodeItr &__sad_parent, bool &__is_left)
        {
        _TREEAJST_REPLACE_BYPOINTER(left, right)
        }
    void replace_right_bypointer(NodeItr &__start, NodeItr &__sad_parent, bool &__is_left)
        {
        bool __is_right;
        _TREEAJST_REPLACE_BYPOINTER(right, left)
        __is_left = !__is_right;
        }

    /*
     * <summary>
     *  replace __start node with the node left to it by index (pointer operation)
     *  __start node is removed from the tree
     *  requirements:
     *   parent pointer required
     *  arguments:
     *   __start -- as input the node to be started from
     *              as output the node to be removed
     *   __sad_parent -- the node where the rebalancing is started from
     *   __is_left -- whether the removed node is the left child of the sad parent
     *     the latter two arguments are exactly those fed to the routine remove_adjust
     * </summary>
     * <remarks>
     *  no pointer operation and imaging, but data copying
     * </remarks>
     */

    #define _TREEAJST_REPLACE_BYVALUE(a, b)                                 \
        NodeItr __p = _##a(__start);                                        \
        if (_isnil(__p)) { return; }                                        \
        for ( ; !_isnil(_##b(__p)); __p = _##b(__p) );                      \
        if (_isnil(_##a(__p)))                                              \
            {                                                               \
            __sad_parent = _parent(__p);                                    \
            __is_##a = _parent(__p) == __start;                             \
            if (__is_##a) _##a(_parent(__p)) = _nil();                      \
            else _##b(_parent(__p)) = _nil();                               \
            _data(__start) = _data(__p);                                    \
            __start = __p;  /* to remove __p */                             \
            }                                                               \
        else                                                                \
            {                                                               \
            __sad_parent = __p;                                             \
            __is_##a = true;                                                \
            _data(__start) = _data(__p);                                    \
            _data(__p) = _data(_##a(__p));                                  \
            _##a(__p) = _nil();                                             \
            __start = _##a(__p);    /* to remove _##a(__p) */               \
            }

    void replace_left_byvalue(NodeItr &__start, NodeItr &__sad_parent, bool &__is_left)
        {
        _TREEAJST_REPLACE_BYVALUE(left, right)
        }
    void replace_right_byvalue(NodeItr &__start, NodeItr &__sad_parent, bool &__is_left)
        {
        bool __is_right;
        _TREEAJST_REPLACE_BYVALUE(right, left)
        __is_left = !__is_right;
        }


    /*
     * .. Recursive approaches
     */

    /*
     * <summary>
     *  find a right place to insert and maintain the balance of the tree all the way 
     *  through the recursion
     * </summary>
     */

    #define _TREEAJST_INSERT_ADJUST(a, b)                                               \
        __result = insert_recursive<WithParent>(_##a##(__subroot), __new_data, __taller, __alloc);  \
        _parent(_##a##(__subroot)) = __subroot;                                         \
        if ( __taller == true ) {                                                       \
            switch ( _get_balance(__subroot) ) {                                        \
                case _BalBase::##a##_higher:                                            \
                    ##a##_balance<WithParent>(__subroot);                               \
                    __taller = false;   /* Rebalancing always shortens the tree. */     \
                    break;                                                              \
                case _BalBase::equal_height:                                            \
                    _set_balance(__subroot, _BalBase::##a##_higher);                    \
                    break;                                                              \
                case _BalBase::##b##_higher:                                            \
                    _set_balance(__subroot, _BalBase::equal_height);                    \
                    __taller = false;                                                   \
                    break;                                                              \
                }                                                                       \
            }

    template <bool WithParent>
    bool insert_recursive (NodeItr &__subroot, const _DataTyp &__new_data, bool &__taller, _Alloc &__alloc)
        {
        bool __result = true;
        if ( _isnil(__subroot) ) {
            _new_node(__subroot, __new_data, __alloc);
            _set_balance(__subroot, _BalBase::equal_height);    // the constructor may have already set the factor to this value by default
            __taller = true;
            }
        else
            {
            int __cmp = _comp(__new_data, _data(__subroot));
            if (__cmp < 0) {    // Insert in left subtree
                _TREEAJST_INSERT_ADJUST(left, right)
                }
            else if (__cmp > 0) {   // Insert in right subtree
                _TREEAJST_INSERT_ADJUST(right, left)
                }
            else {  // already exists
                __taller = false;
                return false;
                }
            }
        return __result;
        }


    /*
     * <summary>
     *  remove the node with specific data from the tree maintain the balance of the 
     *  tree all the way through the recursion
     * </summary>
     */
    #define _TREEAJST_REMOVE_FOUND(a)                                                   \
        replace_##a##_exile<WithParent && ByPointer>(__subroot);                        \
        __result = remove_recursive<WithParent, ByPointer>(_##a##(__subroot), __old_data, __shorter, __alloc);  \
        goto __adjust_##a##;    /* adjust on 'a' side */

    #define _TREEAJST_REMOVE_YETTOFIND(a, b)                                            \
        __result = remove_recursive<WithParent, ByPointer>(_##a##(__subroot), __old_data, __shorter, __alloc);  \
__adjust_##a##:                                                                         \
        if ( __shorter == true )                                                        \
            switch ( _get_balance(__subroot) ) {                                        \
                case _BalBase::##b##_higher:                                            \
                    __shorter = (_get_balance(_##b##(__subroot)) != _BalBase::equal_height);\
                    ##b##_balance<WithParent>(__subroot); /* rebalancing */             \
                    break;                                                              \
                case _BalBase::equal_height:                                            \
                    _set_balance(__subroot, _BalBase::##b##_higher);                    \
                    __shorter = false;                                                  \
                    break;                                                              \
                case _BalBase::##a##_higher:                                            \
                    _set_balance(__subroot, _BalBase::equal_height);                    \
                    break;                                                              \
                }

    template<bool WithParent, bool ByPointer>
    bool remove_recursive (NodeItr &__subroot, const _DataTyp &__old_data, bool &__shorter, _Alloc &__alloc)
        {
        typedef _Traits::balance_base   _BalBase;

        __shorter = false;  /* initialized to be false */

        if (_isnil(__subroot))
        {
            return false;
        }
        bool __result = true;

        int __cmp = _comp(__old_data, _data(__subroot));
        if (__cmp < 0)
            {   // Removal occurs in left subtree.
            _TREEAJST_REMOVE_YETTOFIND(left, right)
            }
        else if (__cmp > 0)
            {   // Removal occurs in right subtree.
            _TREEAJST_REMOVE_YETTOFIND(right, left)
            }
        else
            {   // the node to be removed is found
            if ( !_isnil(_left(__subroot)) )
                {
                _TREEAJST_REMOVE_FOUND(left)
                }
            else if ( !_isnil(_right(__subroot)) )
                {
                _TREEAJST_REMOVE_FOUND(right)
                }
            else {
                NodeItr __todel = __subroot;
                if (WithParent)
                    {
                    if (!_isnil(_parent(__subroot)))
                        {
                        if (_left(_parent(__subroot)) == __subroot) _left(_parent(__subroot)) = _nil();
                        else _right(_parent(__subroot)) = _nil();
                        }
                    }
                _delete_node(__todel, __alloc);
                __subroot = _nil();
                __shorter = true;
                }
            }
        return __result;
        }

protected:

    /*
     * <summary>
     *  node replacement (for removing a non-leaf node IN RECURSIVE MODE)
     *
     *  push the node to be deleted down to the place where is the node that takes its place, 
     *  which will be removed at a later time
     * </summary>
     */
    #define _TREEAJST_REPLACE_EXILE_BYPOINTER(a, b)                     \
        NodeItr __p = _##a(__start); NodeItr __pstart = __start;        \
        for ( ; !_isnil(_##b(__p)); __p = _##b(__p) );                  \
        if (_parent(__p)==__pstart) {                                   \
            _##a(__pstart) = _##a(__p);                                 \
            _##a(__p) = __pstart; swap(_##b(__p), _##b(__pstart));      \
            if (!_isnil(_##a(__pstart))) _parent(_##a(__pstart)) = __pstart;    \
            if (!_isnil(_##b(__p))) _parent(_##b(__p)) = __p;           \
            _parent(__p) = _parent(__pstart);                           \
            _parent(__pstart) = __p;                                    \
            }                                                           \
        else                                                            \
            {                                                           \
            swap(_##a(__p), _##a(__pstart));                            \
            _##b(__p) = _##b(__pstart);                                 \
            _##b(__pstart) = _nil();                                    \
            _parent(_##a(__p)) = __p;                                   \
            if (!_isnil(_##b(__p)))                                     \
                _parent(_##b(__p)) = __p;                               \
            if (!_isnil(_##a(__pstart)))                                \
                _parent(_##a(__pstart)) = __pstart;                     \
            swap(_parent(__p), _parent(__pstart));                      \
            _##b(_parent(__pstart)) = __pstart;                         \
            }                                                           \
        if (!_isnil(_parent(__p))) {                                    \
            if (_left(_parent(__p))==__pstart)_left(_parent(__p))=__p;  \
            else _right(_parent(__p))=__p;                              \
            }                                                           \
        _image_swap(__p, __pstart);                                     \
        __start = __p;

    #define _TREEAJST_REPLACE_EXILE_BYVALUE(a, b)                       \
        NodeItr __p = _##a(__start);                                    \
        for ( ; !_isnil(_##b(__p)); __p = _##b(__p) );                  \
        swap(_data(__p), _data(__start));                               \

    template <bool ByPointer>
    void replace_left_exile(NodeItr &__start)
        {
        if (ByPointer)
            {
            _TREEAJST_REPLACE_EXILE_BYPOINTER(left, right)
            }
        else
            {
            _TREEAJST_REPLACE_EXILE_BYVALUE(left, right)
            }
        }
    template <bool ByPointer>
    void replace_right_exile(NodeItr &__start)
        {
        if (ByPointer)
            {
            _TREEAJST_REPLACE_EXILE_BYPOINTER(right, left)
            }
        else
            {
            _TREEAJST_REPLACE_EXILE_BYVALUE(right, left)
            }
        }

    };  // class adjuster


_QTL_END

#endif  // ifndef _treeadjst_H
