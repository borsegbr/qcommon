/**
 * === Containers Library ===
 *  Unit Name:  avltree
 *  Author:     Lincoln Yu
 *  Purpose:    An TRIAL implementation of AVL tree as exercise, not a good container in STL concept.
 *  Reference:  Data Structures and Program Design in C++, by Robert L. Kruse
 *              and Alexander J. Ryba
 * == History ==        
 *  2005-12-24: Created.
 *  2005-12-26: Roughly debugged.
 *  2007-07-01: Virtual function returns [:not_implemented:] error.
 *  2009-04-12: Polymorphism removed (virtuals) and modified according to the partial 
 *              reimplementation of treeajst for the preparation for setting up an
 *              AVL-tree-based memory management model
 * == Comments ==
 *
 */

#ifndef _QTL_avltree_H
#define _QTL_avltree_H

#include <cassert>
#include <iostream>
#include <queue>


#include "../all/predef.h"
#include "allocators.h"
#include "../algo/treeajst.h"


_QTL_BEGIN

struct tree_err
    {
    enum err_no 
        {
        success         = 0,
        duplicate       = -1,
        not_present     = -2,
        not_implemented = -10,
        };
    };

// struct bstree_adjust_traits

template <class _Tp, class _Alloc = malloc_allocator<_Tp> >
struct bstree_adjust_traits
    {
    struct node;
    typedef node *  node_itr;

    static bool _isnil(node_itr __node)
        {
        return (__node == NULL);
        }

    static node_itr _nil()
        {
        return NULL;
        }

    struct node
        {
        enum balance_factor { left_higher = -1, equal_height = 0, right_higher = 1 };

        _Tp         _M_data;
        node_itr    _M_left;
        node_itr    _M_right;

        node_itr    _M_parent;

        node () : _M_left(_nil()), _M_right(_nil()), _M_parent(_nil())
            {
            }

        node ( const _Tp &__x, node_itr __parent = _nil(), node_itr __left = _nil(), node_itr __right = _nil() ) 
            : _M_data(__x), _M_left(__left), _M_right(__right), _M_parent(__parent)
            {
            }

        void set_balance ( balance_factor __b )
            {
            }

        balance_factor get_balance () const
            {
            return equal_height;
            }

        };  // struct node

    typedef typename _Alloc::template rebind<node>::other allocator;

    static void _new_node(node_itr &__node, allocator &__alloc)
        {
        __node = __alloc.allocate(1);
        __alloc.construct(__node, node());
        }

    static void _new_node(node_itr &__node, const _Tp &__new_data, allocator &__alloc)
        {
        __node = __alloc.allocate(1);
        __alloc.construct(__node, node(__new_data));
        }

    static void _delete_node(node_itr &__node, allocator &__alloc)
        {
        __alloc.destroy(__node);
        __alloc.deallocate(__node);
        __node = _nil();
        }

    static node_itr & _left (node_itr __node)
        {
        return __node->_M_left;
        }

    static node_itr & _right (node_itr __node)
        {
        return __node->_M_right;
        }

    static node_itr & _parent (node_itr __node)
        {
        return __node->_M_parent;
        }

    };  // struct bstree_adjust_traits

template <class _Tp, class _Comp, class _Alloc = malloc_allocator<_Tp> >
    class avl_adjust_traits : public bstree_adjust_traits<_Tp, _Alloc>
    {
    typedef bstree_adjust_traits<_Tp, _Alloc>   _Mybase;
    typedef typename _Mybase::node              Node;

public:
    struct avl_node : Node
        {
        avl_node () : Node()
            {
            }

        avl_node ( const _Tp &__x, Node *__parent = _nil(), Node *__left = _nil(), Node *__right = _nil() ) 
            : Node(__x, __parent, __left, __right), _M_balance(equal_height)
            {
            }

        balance_factor  _M_balance;

        void set_balance ( balance_factor __b )
            {
            _M_balance = __b;
            }

        balance_factor get_balance () const
            {
            return _M_balance;
            }

        };  // struct avl_node

    typedef Node *                          node_itr;
    typedef typename Node::balance_factor   balance_factor;
    typedef _Tp                             data_type;
    typedef Node                            balance_base;
    typedef _Comp                           comp;
    typedef typename _Alloc::template rebind<avl_node>::other   allocator;

    static void _new_node(node_itr &__node, allocator &__alloc)
        {
        __node = __alloc.allocate(1);
        __alloc.construct(static_cast<avl_node*>(__node), avl_node());
        }

    static void _new_node(node_itr &__node, const data_type &__new_data, allocator &__alloc)
        {
        __node = __alloc.allocate(1);
        __alloc.construct(static_cast<avl_node*>(__node), avl_node(__new_data));
        }

    static void _delete_node(node_itr &__node, allocator &__alloc)
        {
        __alloc.destroy(static_cast<avl_node*>(__node));
        __alloc.deallocate(static_cast<avl_node*>(__node));
        __node = _nil();
        }

    static data_type & _data (node_itr __node)
        {
        return static_cast<avl_node*>(__node)->_M_data;
        }

    static void _set_balance (node_itr __node, balance_factor __b)
        {
        static_cast<avl_node*>(__node)->set_balance(__b);
        }

    static balance_factor _get_balance (node_itr __node)
        {
        return static_cast<avl_node*>(__node)->get_balance();
        }

    static void _image_copy(node_itr __dst, node_itr __src)
        {
        avl_node* __avldst = static_cast<avl_node*>(__dst);
        avl_node* __avlsrc = static_cast<avl_node*>(__src);
        __avldst->_M_balance = __avlsrc->_M_balance;
        }

    static void _image_swap(node_itr __a, node_itr __b)
        {
        avl_node* __avla = static_cast<avl_node*>(__a);
        avl_node* __avlb = static_cast<avl_node*>(__b);
        swap(__avla->_M_balance, __avlb->_M_balance);
        }

    static comp _comp;

    };  // bstree_adjust_traits

template <class _Tp, class _Comp, class _Alloc>
    _Comp avl_adjust_traits<_Tp, _Comp, _Alloc>::_comp;


// class binary_tree

template < class _Tp, class _Adjust_Traits = bstree_adjust_traits<_Tp>, 
    class _Adjuster = bstree_adjuster<_Adjust_Traits> >
class binary_tree : public _Adjuster::template rebind<_Adjust_Traits>::other
    {
    typedef binary_tree                     _Myt;

protected:
    typedef tree_err::err_no                ErrCode;

    typedef typename _Adjust_Traits::node   node;
    typedef typename _Adjust_Traits::node_itr   node_itr;

public:
    typedef _Tp                             data_type;
    typedef typename _Adjust_Traits::allocator _Alloc;
    typedef typename _Adjuster::template rebind<_Adjust_Traits>::other  adjuster;

    struct node_info
        {
        enum type { root, left, right };

        type    _M_type;
        int     _M_depth;
        node_itr    _M_dump;

        node_info ( type __type, int __depth, node_itr __dump )
            : _M_type(__type), _M_depth(__depth), _M_dump(__dump)
            {
            }
        };

protected:
    template <class visitor>
        void inorder_recursive ( node_itr __sub_root, visitor &__visit, node_info &__info )
        /*  Pre:    [:__sub_root:] is either nil or points to a subtree of the binary tree.
            Post:   The subtree has been traversed in inorder sequence.
            Uses:   The function [:inorder_recursive:] recursively. */
        {
        if ( !_Adjust_Traits::_isnil(__sub_root )) {
            int __depth = __info._M_depth + 1;
            inorder_recursive(__sub_root->_M_left, __visit, 
                node_info(node_info::left, __depth, __sub_root->_M_left)
                );
            __visit(__sub_root->_M_data, __info);
            inorder_recursive(__sub_root->_M_right, __visit, 
                node_info(node_info::right, __depth, __sub_root->_M_right)
                );
            }
        }

    template <class visitor>
        void preorder_recursive ( node_itr __sub_root, visitor &__visit, node_info &__info )
        /*  Pre:  [:__sub_root:] is either nil or points to a subtree of the binary tree.
            Post: The subtree has been traversed in preorder sequence.
            Uses: The function [:preorder_recursive:] recursively. */
        {
        if ( !_Adjust_Traits::_isnil(__sub_root )) {
            __visit(__sub_root->_M_data, __info);
            int __depth = __info._M_depth + 1;
            preorder_recursive(__sub_root->_M_left, __visit, 
                node_info(node_info::left, __depth, __sub_root->_M_left)
                );
            preorder_recursive(__sub_root->_M_right, __visit, 
                node_info(node_info::right, __depth, __sub_root->_M_right)
                );
            }
        }

    template <class visitor>
        void postorder_recursive ( node_itr __sub_root, visitor &__visit, node_info &__info )
        /*  Pre:  [:__sub_root:] is either nil or points to a subtree of the binary tree.
            Post: The subtree has been traversed in postorder sequence.
            Uses: The function [:postorder_recursive:] recursively. */
        {
        if ( !_Adjust_Traits::_isnil(__sub_root )) {
            int __depth = __info._M_depth + 1;
            postorder_recursive(__sub_root->_M_left, __visit, 
                node_info(node_info::left, __depth, __sub_root->_M_left)
                );
            postorder_recursive(__sub_root->_M_right, __visit, 
                node_info(node_info::right, __depth, __sub_root->_M_right)
                );
            __visit(__sub_root->_M_data, __info);
            }
        }

    void clear_subtree ( node_itr __sub_root, _Alloc &__alloc = _Alloc() )
        /*  Pre:  [:__sub_root:] is either nil or points to a subtree of the binary tree.
            Post: The subtree has been cleared by a postorder destruction mechanism.
            Uses: The function [:clear_subtree:] recursively. */
        {
        if ( !_Adjust_Traits::_isnil(__sub_root )) {
            clear_subtree(__sub_root->_M_left);
            clear_subtree(__sub_root->_M_right);
            _Adjust_Traits::_delete_node(__sub_root, __alloc);
            }           
        }

    void copy_tree( node_itr &__node, const node_itr __copy_root, _Alloc &__alloc = _Alloc() )
        /*  Pre:  [:__node:] is handle of the node who will be the root of the copy.
            Post: the copy is inserted at [:*__node:] by a preorder construction mechanism.
            Uses: The function [:copy_tree:] recursively. */
        {
        if (_isnil(__copy_root))
            {
            __node = _nil();
            return;
            }
        _Adjust_Traits::_new_node(__node, __alloc);
        *__node = *__copy_root;
        copy_tree(__node->_M_left, __copy_root->_M_left);
        copy_tree(__node->_M_right, __copy_root->_M_right);
        }

public:

#define AVLTREE_BSTREE_ITERATOR_CLASS_NAME(order)   order##_iterator

#define AVLTREE_BSTREE_ITERATOR_CLASS_DEF(order)                                \
        class AVLTREE_BSTREE_ITERATOR_CLASS_NAME(order)                         \
        {                                                                       \
            typedef AVLTREE_BSTREE_ITERATOR_CLASS_NAME(order)   _Myt;           \
        public:                                                                 \
            AVLTREE_BSTREE_ITERATOR_CLASS_NAME(order)( adjuster *__adjuster )    \
                :_M_pNode(_Adjust_Traits::_nil()) {}                          \
            AVLTREE_BSTREE_ITERATOR_CLASS_NAME(order)( node_itr __node, adjuster *__adjuster )    \
                :_M_pNode(__node) {}                                            \
            _Myt & operator++ ()                                                \
                {                                                               \
                _M_adjuster->##order##_inc(_M_pNode);                           \
                return *this;                                                   \
                }                                                               \
            _Myt operator++ (int)                                               \
                {                                                               \
                _Myt temp = *this;                                              \
                ++*this;                                                        \
                return temp;                                                    \
                }                                                               \
            _Myt & operator-- ()                                                \
                {                                                               \
                _M_adjuster->##order##_dec(_M_pNode);                           \
                return *this;                                                   \
                }                                                               \
            _Myt operator-- (int)                                               \
                {                                                               \
                _Myt temp = *this;                                              \
                --*this;                                                        \
                return temp;                                                    \
                }                                                               \
            data_type operator* () { return _M_pNode->_M_data;  }               \
            bool isnil() { return (_Adjust_Traits::_isnil(_M_pNode)); }         \
        private:                                                                \
            friend class binary_tree;                                           \
            node_itr    _M_pNode;                                               \
            adjuster    *_M_adjuster;                                           \
        };

    AVLTREE_BSTREE_ITERATOR_CLASS_DEF(preorder)
    AVLTREE_BSTREE_ITERATOR_CLASS_DEF(inorder)
    AVLTREE_BSTREE_ITERATOR_CLASS_DEF(postorder)


    binary_tree () : _M_root(_Adjust_Traits::_nil())
        /* Post: An empty binary tree has been created  */
        {
        }

    binary_tree (const _Myt &__copy)
        {
        copy_tree(_M_root, __copy._M_root);
        }

    ~binary_tree ()
        {
        clear();
        }

    preorder_iterator   preorder_begin ()
        {
        preorder_iterator   result(_M_root, this);
        return result;
        }

    inorder_iterator    inorder_begin ()
        {
        inorder_iterator    result(this);
        result._M_pNode = this->adjuster::leftmost(_M_root);
        return result;
        }

    postorder_iterator  postorder_begin ()
        {
        postorder_iterator  result(this);
        result._M_pNode = this->adjuster::leftdeepmost(_M_root);
        return result;
        }

    bool is_empty () const
        /* Post: A result of true is returned if the binary tree is empty. Otherwise, false is returned. */
        {
        return ( _Adjust_Traits::_isnil(_M_root) );
        }

    template <class visitor>
        void inorder (visitor __visitor)
        /*  Post: The tree has been traversed in inorder sequence.
            Uses: The function [:inorder_recursive:]. */
        {
        inorder_recursive(_M_root, __visitor, node_info(node_info::root, 0, _M_root));
        }

    template <class visitor>
        void preorder (visitor __visitor)
        /*  Post: The tree has been traversed in preorder sequence.
            Uses: The function [:preorder_recursive:] */
        {
        preorder_recursive(_M_root, __visitor, node_info(node_info::root, 0, _M_root));
        }

    template <class visitor>
        void postorder (visitor __visitor)
        /*  Post: The tree has been traversed in postorder sequence.
            Uses: The function [:postorder_recursive:]. */
        {
        postorder_recursive(_M_root, __visitor, node_info(node_info::root, 0, _M_root));
        }

    void clear ()
        /*  Post: The subtree has been cleared with memory deallocated.
            Uses: The function [:clear_subtree&&. */
        {
        clear_subtree(_M_root);
        }

    template <class _Comp>
        ErrCode insert (const _Tp &__new_data, _Comp __comp, _Alloc &__alloc = _Alloc())
        /*  Post: [:__new_data:] is inserted at the right place to maintain the increasing order defined by [:__comp:] 
         */
        {
        node_itr __parent = _Adjust_Traits::_nil();
        node_itr *__p = &_M_root;
        int __cmp;
        while ( !_Adjust_Traits::_isnil(*__p) )
            {
            __parent = *__p;
            __cmp = _comp(__new_data, (*__p)->_M_data);
            if ( __cmp < 0 )
                __p = &((*__p)->_M_left);
            else if ( __cmp > 0 )
                __p = &((*__p)->_M_right);
            else
                return tree_err::duplicate;
            }
        _Adjust_Traits::_new_node(*__p, __new_data, __alloc);
        (*__p)->_M_parent = __parent;

        return tree_err::success;
        }

    binary_tree & operator= (const binary_tree &__copy)
        {
        clear();
        copy_tree (&_M_root, __copy._M_root);
        }

protected:
    node_itr    _M_root;

    };  /* class binary_tree */

template < class _Tree >
    struct tree_viewer 
    {
    typedef typename _Tree::data_type   DataType;
    typedef typename _Tree::node_info   NodeInfo;

    _Tree                               *_M_tree;
    int                                 _M_field_width;

    enum view_mode { view_sort, view_struct };

    view_mode                   _M_view_mode;

    tree_viewer ( _Tree *__tree, int __field_width = 5, view_mode __vm = view_sort) 
        : _M_tree(__tree), _M_view_mode(__vm), _M_field_width(__field_width)
        {

        }

    struct visit_sort
        {
        std::ostream    *_M_out;
        int             _M_field_width;

        visit_sort( std::ostream &__stream_out, int __field_width ) 
            : _M_out(&__stream_out), _M_field_width(__field_width)
            {
            }

        void operator() ( const DataType &__x, const NodeInfo & )
            {
            _M_out->width(_M_field_width);
            *_M_out << __x;
            }
        };

    struct visit_struct
        {
        std::ostream    *_M_out;
        int             _M_field_width;
        _Tree           *_M_tree;

        std::queue<int> _M_stick_queue;

        visit_struct( std::ostream &__stream_out, int __field_width, _Tree *__tree ) 
            : _M_out(&__stream_out), _M_field_width(__field_width), _M_tree(__tree)
            {
            }

        void operator() ( const DataType &__x, const NodeInfo &__info )
            {
            std::queue<int> __temp_queue;
            if ( __info._M_type == NodeInfo::root ) {
                _M_out->width(3); *_M_out << " - ";
                *_M_out << '[';
                _M_out->width(_M_field_width); *_M_out << __x;
                *_M_out << ']';
                }
            else if ( __info._M_type == NodeInfo::left ) {
                _M_out->width(3); 
                if (!_M_tree->_isnil(__info._M_dump->_M_parent) && !_M_tree->_isnil(__info._M_dump->_M_parent->_M_right))
                    *_M_out << " + ";
                else
                    *_M_out << " - ";
                *_M_out << '[';
                _M_out->width(_M_field_width); *_M_out << __x;
                *_M_out << ']';
                _M_stick_queue.push(__info._M_depth);
                }
            else {
                *_M_out << std::endl;
                
                int __stick;

                _M_out->width(_M_field_width + 5); 
                *_M_out << "";

                for ( int i = 1; i < __info._M_depth; i++ ) {
                    if ( i == 1 )
                        {
                        if ( !_M_stick_queue.empty() )
                            {
                            __stick = _M_stick_queue.front();
                            _M_stick_queue.pop();
                            __temp_queue.push(__stick);
                            }
                        else
                            __stick = -1;
                        }

                    if ( i == __stick ) {
                        *_M_out << " | ";
                        _M_out->width(_M_field_width + 2);
                        *_M_out << "";
                        __temp_queue.push(__stick);
                        if ( !_M_stick_queue.empty() ) {
                            __stick = _M_stick_queue.front();
                            _M_stick_queue.pop();
                            }
                        else
                            __stick = -1;
                        }
                    else {
                        _M_out->width(_M_field_width + 5); 
                        *_M_out << "";
                        }
                    }

                _M_stick_queue = __temp_queue;

                *_M_out << " \\ ";
                *_M_out << '[';
                _M_out->width(_M_field_width); *_M_out << __x;
                *_M_out << ']';
                }
            }
        };

    void operator() ( std::ostream &__stream_out )
        {
        switch ( _M_view_mode ) {
            case view_sort:
                {
                visit_sort __v(__stream_out, _M_field_width);
                _M_tree->inorder(__v);
                break;
                }
            case view_struct:
                {
                visit_struct __v(__stream_out, _M_field_width, _M_tree);
                _M_tree->preorder(__v);
                break;
                }
            }
        }
    };  // struct tree_viewer

template <class _Tree>
    std::ostream & operator<< (std::ostream &__stream_out, tree_viewer<_Tree> &__tview)
    {
    __tview(__stream_out);
    return __stream_out;
    }

template <class _Tp, class _LessThan, bool _UseRecursive = false, 
    class _Adjust_Traits = avl_adjust_traits<_Tp, _LessThan>, 
    class _Adjuster = avl_adjuster<_Adjust_Traits> >
class avl_tree : public binary_tree< _Tp, _Adjust_Traits, _Adjuster >
    {
    typedef binary_tree<_Tp, _Adjust_Traits>    _Mybase;

protected:
    typedef typename _Adjust_Traits::node           Node;   // binary_tree::node
    typedef typename _Adjust_Traits::avl_node       avl_node;

public:
    typedef _Tp                                     data_type;
    typedef typename _Adjust_Traits::allocator      _Alloc;


    ErrCode insert ( Node *__newp )
    {
        if (_Adjust_Traits::_isnil(_M_root))
        {
            _M_root = __newp;
        }
        else
        {
            Node *__p = _M_root;
            int __result = _Adjuster::search(__p, __newp->_M_data);
            if (__result == 1)
            {
                __p->_M_right = __newp;
                __p->_M_right->_M_parent = __p;
                __p = __p->_M_right;
            }
            else if (__result == -1)
            {
                __p->_M_left = __newp;
                __p->_M_left->_M_parent = __p;
                __p = __p->_M_left;
            }
            else
                return tree_err::duplicate;

            _Adjuster::insert_adjust(__p);

            for ( ; !_Adjust_Traits::_isnil(_M_root->_M_parent); _M_root = _M_root->_M_parent ) ; 
        }
        return tree_err::success;
    }

    ErrCode insert ( const _Tp &__new_data, _Alloc &__alloc = _Alloc())
        {
        if (_UseRecursive)
            {
            bool __taller;  // dummy

            if ( _Adjuster::insert_recursive<true>(_M_root, __new_data, __taller, __alloc) )
                return tree_err::success;
            else
                return tree_err::duplicate;
            }
        else
            {
            Node *__newp;
            _Adjust_Traits::_new_node(__newp, __new_data, __alloc);
            return insert(__newp);
            }
        }

    ErrCode remove_bypointer( Node *__p )
        {
        Node *__sad_parent;

        bool __is_left;
        if ( !_Adjust_Traits::_isnil(__p->_M_left) )
            {
            /* remove */
            Node *__start = __p;
            _Adjuster::replace_left_bypointer(__start, __sad_parent, __is_left);
            if (__p == _M_root) _M_root = __start;
            }
        else if ( !_Adjust_Traits::_isnil(__p->_M_right) )
            {
            /* remove */
            Node *__start = __p;
            _Adjuster::replace_right_bypointer(__start, __sad_parent, __is_left);
            if (__p == _M_root) _M_root = __start;
            }
        else
            {
            __sad_parent = __p->_M_parent;
            if ( _Adjust_Traits::_isnil(__sad_parent) ) 
                {
                _M_root = _Adjust_Traits::_nil();
                return tree_err::success;
                }
            else
                {
                if ( __is_left = (__p == __sad_parent->_M_left) ) __sad_parent->_M_left = _Adjust_Traits::_nil();
                else __sad_parent->_M_right = _Adjust_Traits::_nil();
                }
            }

        _Adjuster::remove_adjust(__sad_parent, __is_left);

        for ( ; !_Adjust_Traits::_isnil(_M_root->_M_parent); _M_root = _M_root->_M_parent ) ; 

        return tree_err::success;
        }

    ErrCode remove_byvalue( Node *&__p )
        {
        Node *__sad_parent;

        bool __is_left;
        if ( !_Adjust_Traits::_isnil(__p->_M_left) )
            {
            /* remove */
            _Adjuster::replace_left_byvalue(__p, __sad_parent, __is_left);
            }
        else if ( !_Adjust_Traits::_isnil(__p->_M_right) )
            {
            /* remove */
            _Adjuster::replace_right_byvalue(__p, __sad_parent, __is_left);
            }
        else
            {
            __sad_parent = __p->_M_parent;

            if ( _Adjust_Traits::_isnil(__sad_parent) ) 
                {
                _M_root = _Adjust_Traits::_nil();
                return tree_err::success;
                }
            else
                {
                if ( __is_left = (__p == __sad_parent->_M_left) ) __sad_parent->_M_left = _Adjust_Traits::_nil();
                else __sad_parent->_M_right = _Adjust_Traits::_nil();
                }
            }

        _Adjuster::remove_adjust(__sad_parent, __is_left);

        for ( ; !_Adjust_Traits::_isnil(_M_root->_M_parent); _M_root = _M_root->_M_parent ) ; 

        return tree_err::success;
    }

    template <bool _ByPointer>
    ErrCode remove ( const _Tp &__old_data, _Alloc &__alloc = _Alloc() )
        {
        if (_UseRecursive)
            {
            bool __shorter; // dummy
            // replace
            if (_Adjuster::remove_recursive<true, _ByPointer>(_M_root, __old_data, __shorter, __alloc))
                return tree_err::success;
            else
                return tree_err::not_present;
            }
        else
            {
            Node *__p = _M_root;
            int __result = _Adjuster::search(__p, __old_data);

            if (__result != 0)
                return tree_err::not_present;

            ErrCode err;
            if (_ByPointer)
                err = remove_bypointer(__p);
            else
                err = remove_byvalue(__p);
            _Adjust_Traits::_delete_node(__p, __alloc);
            return err;
            }
        }

    };  // class avl_tree


_QTL_END

#endif  // ifndef _QTL_avltree_H
