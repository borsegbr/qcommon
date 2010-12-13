/**
 * === Containers Library ===
 *  Unit Name:  tree
 *  Author:     Lincoln Yu
 *  Purpose:    An TRIAL implementation of several trees as exercise, not a good container in STL concept.
 *	Reference:	Data Structures and Program Design in C++, by Robert L. Kruse
 *				and Alexander J. Ryba
 * == History ==        
 *  2005-11-19: Created, with binary tree, search tree and AVL tree added.
 * == Comments ==
 *
 */

#ifndef _QTL_avltree_H
#define _QTL_avltree_H

#include <cassert>
#include <iostream>
#include <queue>


#include "../all/predef.h"	

_QTL_BEGIN

struct tree_err
	{
	enum err_no 
		{
		success			= 0,
		duplicate		= -1,
		not_present		= -2,
		};
	};

// class binary_tree

template <class _Tp>
	class binary_tree
	{

	typedef binary_tree		_Myt;

protected:
	typedef tree_err::err_no	ErrCode;

public:
	struct node
		{
	// types
		enum balance_factor { left_higher, equal_height, right_higher };

	// data members
		_Tp		_M_data;
		node	*_M_left;
		node	*_M_right;

	// constructors
		node () : _M_left(NULL), _M_right(NULL)
			{
			}

		node ( const _Tp &__x, node *__left = NULL, node *__right = NULL ) 
			: _M_data(__x), _M_left(__left), _M_right(__right)
			{
			}

	// virtual methods
		virtual void set_balance ( balance_factor __b )
			{
			}

		virtual balance_factor get_balance () const
			{
			return equal_height;
			}
	};

	struct node_info
		{
		enum type { root, left, right };

		type	_M_type;
		int		_M_depth;
		node	*_M_dump;

		node_info ( type __type, int __depth, node *__dump )
			: _M_type(__type), _M_depth(__depth), _M_dump(__dump)
			{
			}
		};

protected:
	template <class visitor>
		void inorder_recursive ( node *__sub_root, visitor &__visit, node_info &__info )
		/*	Pre:	[:__sub_root:] is either NULL or points to a subtree of the binary tree.
			Post:	The subtree has been traversed in inorder sequence.
			Uses:	The function [:inorder_recursive:] recursively. */
		{
		if ( __sub_root != NULL ) {
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
		void preorder_recursive ( node *__sub_root, visitor &__visit, node_info &__info )
		/*	Pre:  [:__sub_root:] is either NULL or points to a subtree of the binary tree.
			Post: The subtree has been traversed in preorder sequence.
			Uses: The function [:preorder_recursive:] recursively. */
		{
		if ( __sub_root != NULL ) {
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
		void postorder_recursive ( node *__sub_root, visitor &__visit, node_info &__info )
		/*	Pre:  [:__sub_root:] is either NULL or points to a subtree of the binary tree.
			Post: The subtree has been traversed in postorder sequence.
			Uses: The function [:postorder_recursive:] recursively. */
		{
		if ( __sub_root != NULL ) {
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

	void clear_subtree ( node *__sub_root )
		/*	Pre:  [:__sub_root:] is either NULL or points to a subtree of the binary tree.
			Post: The subtree has been cleared by a postorder destruction mechanism.
			Uses: The function [:clear_subtree:] recursively. */
		{
		if ( __sub_root != NULL ) {
			clear_subtree(__sub_root->_M_left);
			clear_subtree(__sub_root->_M_right);
			delete __sub_root;
			}			
		}

	void insert_tree( node * &__node, node *__copy_root )
		/*	Pre:  [:__node:] is handle of the node who will be the root of the copy.
			Post: the copy is inserted at [:*__node:] by a preorder construction mechanism.
			Uses: The function [:insert_tree:] recursively. */
		{
		__node = new node(*__copy_root);
		insert_tree(&__node->_M_left, __copy_root->_M_left);
		insert_tree(&__node->_M_right, __copy_root->_M_right);
		}


public:

// constructors
	binary_tree () : _M_root(NULL), _M_height(0), _M_size(0)
		/* Post: An empty binary tree has been created	*/
		{
		}

	binary_tree (const _Myt &__copy)
		{
		insert_tree (&_M_root, __copy._M_root);
		}

	~binary_tree ()
		{
		clear();
		}

// methods
	bool is_empty () const
		/* Post: A result of true is returned if the binary tree is empty. Otherwise, false is returned. */
		{
		return ( _M_root == NULL );
		}

	int height () const
		{
		return _M_height;
		}

	int size () const
		{
		return _M_size;
		}

	template <class visitor>
		void inorder (visitor __visitor)
		/*	Post: The tree has been traversed in inorder sequence.
			Uses: The function [:inorder_recursive:]. */
		{
		inorder_recursive(_M_root, __visitor, node_info(node_info::root, 0, _M_root));
		}

	template <class visitor>
		void preorder (visitor __visitor)
		/*	Post: The tree has been traversed in preorder sequence.
			Uses: The function [:preorder_recursive:] */
		{
		preorder_recursive(_M_root, __visitor, node_info(node_info::root, 0, _M_root));
		}

	template <class visitor>
		void postorder (visitor __visitor)
		/*	Post: The tree has been traversed in postorder sequence.
			Uses: The function [:postorder_recursive:]. */
		{
		postorder_recursive(_M_root, __visitor, node_info(node_info::root, 0, _M_root));
		}

	void clear ()
		/*	Post: The subtree has been cleared with memory deallocated.
			Uses: The function [:clear_subtree:]. */
		{
		clear_subtree(_M_root);
		_M_height = 0;
		}

	template <class _less_than>
		ErrCode insert (const _Tp &__new_data, _less_than __lt)
		/*	Post: [:__new_data:] is inserted at the right place to maintain the increasing order defined by [:__lt:]
			Hint: not virtual */
		{
		node * &__p = _M_root;
		while (__p != NULL)
			{
			if ( __lt(__new_data, (__p)->_M_data) )
				__p = (__p)->_M_left;
			else if ( __lt((*__p)->_M_data), __new_data )
				__p = (__p)->_M_right;
			else
				return tree_err::duplicate;
			}
		__p = new node(__new_data);

		return tree_err::success;
		}

	template <class _less_than>
		ErrCode remove (const _Tp &__old_data, _less_than __lt)
		/*	Post: [:__old_data:] is removed. The searching is leaded by [:__lt:]
			Hint: not virtual */
		{
		node * &__p = _M_root;
		while (__p != NULL)
			{
			if ( __lt(__old_data, __p->_M_data) )
				__p = __p->_M_left;
			else
				__p = __p->_M_right;
			}
		delete __p;
		__p = NULL;

		return tree_err::success;
		}

// operator overridings
	binary_tree & operator= (const binary_tree &__copy)
		{
		clear();
		insert_tree (&_M_root, __copy._M_root);
		}

// virtual methods



protected:
	node	*_M_root;
	int	_M_height;
	int _M_size;
	
	};

template <class _Tp>
	struct tree_viewer 
	{
	typedef binary_tree<_Tp>			Tree;
	typedef typename Tree::node_info	NodeInfo;

	Tree								*_M_tree;
	int									_M_field_width;

	enum view_mode { view_sort, view_struct };

	view_mode					_M_view_mode;

	tree_viewer ( Tree *__tree, int __field_width = 5, view_mode __vm = view_sort) 
		: _M_tree(__tree), _M_view_mode(__vm), _M_field_width(__field_width)
		{
		}

	struct visit_sort
		{
		std::ostream	*_M_out;
		int				_M_field_width;

		visit_sort( std::ostream &__out, int __field_width ) 
			: _M_out(&__out), _M_field_width(__field_width)
			{
			}

		void operator() ( const _Tp &__x, const NodeInfo & )
			{
			_M_out->width(_M_field_width);
			*_M_out << __x;
			}
		};

	struct visit_struct
		{
		std::ostream	*_M_out;
		int				_M_field_width;

		std::queue<int>	_M_stick_queue;

		visit_struct( std::ostream &__out, int __field_width ) 
			: _M_out(&__out), _M_field_width(__field_width)
			{
			}

		void operator() ( const _Tp &__x, const NodeInfo &__info )
			{
			if ( __info._M_type == NodeInfo::root ) {
				_M_out->width(3); *_M_out << " - ";
				*_M_out << '[';
				_M_out->width(_M_field_width); *_M_out << __x;
				*_M_out << ']';
				}
			else if ( __info._M_type == NodeInfo::left ) {
				_M_out->width(3); *_M_out << " T ";
				*_M_out << '[';
				_M_out->width(_M_field_width); *_M_out << __x;
				*_M_out << ']';
				_M_stick_queue.push(__info._M_depth);
				}
			else {
				*_M_out << std::endl;
				
				int __stick;
				if ( !_M_stick_queue.empty() ) {
					__stick = _M_stick_queue.back();
					_M_stick_queue.pop();
					}
				else
					__stick = -1;

				for ( int i = 0; i < __info._M_depth; i++ ) {
					if ( i == __stick ) {
						*_M_out << " | ";
						_M_out->width(_M_field_width + 2);
						*_M_out << "";
						if ( !_M_stick_queue.empty() ) {
							__stick = _M_stick_queue.back();
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

				*_M_out << " \\ ";
				*_M_out << '[';
				_M_out->width(_M_field_width); *_M_out << __x;					
				*_M_out << ']';
				}
			}
		};

	void operator() ( std::ostream &__out )
		{
		switch ( _M_view_mode ) {
			case view_sort:
				{
				visit_sort __v(__out, _M_field_width);
				_M_tree->inorder(__v);
				break;
				}
			case view_struct:
				{
				visit_struct __v(__out, _M_field_width);
				_M_tree->preorder(__v);
				break;
				}
			}
		}
	
	};

template <class _Tp>
std::ostream & operator<< (std::ostream &__out, tree_viewer<_Tp> &__tview)
	{
	__tview(__out);
	return __out;
	}

// class search_tree

template <class _Tp, class _less_than>
class search_tree : public binary_tree<_Tp>
	{
	typedef binary_tree<_Tp>	_Mybase;

protected:
	typedef _Mybase::node		Node;
		
	Node * search_for_node ( Node *__sub_root, const _Tp *__target) const
		{
		while ( __sub_root != NULL && __sub_root->_M_data != __target )
			if ( _M_lt(__target, __sub_root->_M_data) )
				__sub_root = __sub_root->_M_left;
			else
				__sub_root = __sub_root->_M_right;
		return __sub_root;
		}
public:
	virtual ErrCode insert ( const _Tp &__new_data )
		{
		return tree_err::success;
		}

	virtual ErrCode remove ( const _Tp &__old_data )
		{
		return tree_err::success;
		}

	ErrCode tree_search ( _Tp &__target ) const
		{
		ErrCode __result = TreeErr::Success;
	
		Node *__found = search_for_node(_M_root, target);

		if ( __found == NULL )
			__result = TestErr::not_present;
		else
			__target = __found->_M_data;

		return (__result);
		}

protected:
	_less_than	_M_lt;
	};


template <class _Tp, class _less_than>
class avl_tree : public search_tree<_Tp, _less_than>
{
	typedef search_tree<_Tp, _less_than>	_Mybase;

protected:
	typedef _Mybase::Node					Node;	// binary_tree::node

	struct avl_node : Node
		{
	// constructors
		avl_node () : Node()
			{
			}

		avl_node ( const _Tp &__x, node *__left = NULL, node *__right = NULL ) 
			: Node(__x, __left, __right), _M_balance(equal_height)
			{
			}

	// data memebers
		balance_factor	_M_balance;

	// virtual methods
		virtual void set_balance ( balance_factor __b )
			{
			_M_balance = __b;
			}

		virtual balance_factor get_balance () const
			{
			return _M_balance;
			}

		};	// class avl_node

	void rotate_left ( Node * &__sub_root )
		/* Pre:  [:__sub_root:] points to a subtree of the AVL tree. This subtree has a nonempty right subtree.
			Post: [:__sub_root:] is reset to point to its former right child, and the former [:sub_root:] node is
					the left child of the new [:__sub_root:] node. */
		{
		assert ( __sub_root != NULL && __sub_root->_M_right != NULL );

		Node *__right_tree = __sub_root->_M_right;
		__sub_root->_M_right = __right_tree->_M_left;
		__right_tree->_M_left = __sub_root;
		__sub_root = __right_tree;
		}

	void rotate_right ( Node * &__sub_root )
		/* Pre:  [:__sub_root:] points to a subtree of the AVL tree. This subtree has a nonempty left subtree.
			Post: [:__sub_root:] is reset to point to its former left child, and the former [:sub_root:] node is
					the right child of the new [:__sub_root:] node.
			Hint: The case [:Node::equal_height:] on [:__left_tree:] only occurs when using removal. */
		{
		assert ( __sub_root != NULL && __sub_root->_M_left != NULL );

		Node *__left_tree = __sub_root->_M_left;
		__sub_root->_M_left = __left_tree->_M_right;
		__left_tree->_M_right = __sub_root;
		__sub_root = __left_tree;
		}

	void left_balance ( Node * &__sub_root )
		/* Pre:  [:__sub_root:] points to a subtree of the AVL tree that is doubly unbalanced on the left.
			Post: [:__sub_root:] is reset to point to its former right child, and the former [:sub_root:] node is
				the left child of the new [:__sub_root:] node.
			Hint: The case [:Node::equal_height:] on [:__left_tree:] only occurs when using removal */
		{
		Node * &__left_tree = __sub_root->_M_left;
		Node * __sub_tree;
		switch ( __left_tree->get_balance() )
			{
			case Node::left_higher: case Node::equal_height:	// single rotation right
				__sub_root->set_balance(Node::equal_height);
				__left_tree->set_balance(Node::equal_height);
				rotate_right(__sub_root);
				break;
			case Node::right_higher:		// double rotation right
				__sub_tree = __left_tree->_M_right;
				switch ( __sub_tree->get_balance() )
				{
				case Node::equal_height:
					__sub_root->set_balance(Node::equal_height);
					__left_tree->set_balance(Node::equal_height);
					break;
				case Node::right_higher:
					__sub_root->set_balance(Node::equal_height);
					__left_tree->set_balance(Node::left_higher);
					break;
				case Node::left_higher:
					__sub_root->set_balance(Node::right_higher);
					__left_tree->set_balance(Node::equal_height);
					break;
			}

			__sub_tree->set_balance(Node::equal_height);
			rotate_left(__left_tree);
			rotate_right(__sub_root);
			break;
			}
		}

	void right_balance ( Node * &__sub_root )
		/* Pre:  [:__sub_root:] points to a subtree of the AVL tree that is doubly unbalanced on the right.
			Post: [:__sub_root:] is reset to point to its former left child, and the former [:sub_root:] node is
				the right child of the new [:__sub_root:] node. */
		{
		Node * &__right_tree = __sub_root->_M_right;
		switch ( __right_tree->get_balance() )
			{
			case Node::right_higher: case Node::equal_height:	// single rotation left
				__sub_root->set_balance(Node::equal_height);
				__right_tree->set_balance(Node::equal_height);
				rotate_left(__sub_root);
				break;
			case Node::left_higher:		// double rotation left
				Node *__sub_tree = __right_tree->_M_left;
				switch ( __sub_tree->get_balance() )
				{
				case Node::equal_height:
					__sub_root->set_balance(Node::equal_height);
					__right_tree->set_balance(Node::equal_height);
					break;
				case Node::left_higher:
					__sub_root->set_balance(Node::equal_height);
					__right_tree->set_balance(Node::right_higher);
					break;
				case Node::right_higher:
					__sub_root->set_balance(Node::left_higher);
					__right_tree->set_balance(Node::equal_height);
					break;
			}

			__sub_tree->set_balance(Node::equal_height);
			rotate_right(__right_tree);
			rotate_left(__sub_root);
			break;
			}
		}

	ErrCode avl_insert ( Node * &__sub_root, const _Tp &__new_data, bool &__taller )
		{
		ErrCode __result = tree_err::success;

		if ( __sub_root == NULL ) {
			__sub_root = new avl_node(__new_data);
			__taller = true;
			}
		else if ( __new_data == __sub_root->_M_data ) {
			__result = tree_err::duplicate;
			__taller = false;
			}
		else if ( _M_lt(__new_data, __sub_root->_M_data) ) {	// Insert in left subtree.
			__result = avl_insert(__sub_root->_M_left, __new_data, __taller);
			if ( __taller == true ) {
				switch ( __sub_root->get_balance() ) {
					case Node::left_higher:
						left_balance(__sub_root);
						__taller = false;	// Rebalancing always shortens the tree.
						break;
					case Node::equal_height:
						__sub_root->set_balance(Node::left_higher);
						break;
					case Node::right_higher:
						__sub_root->set_balance(Node::equal_height);
						__taller = false;
						break;
					}
				}
			}
		else {	// Insert in right subtree.
			__result = avl_insert(__sub_root->_M_right, __new_data, __taller);
			if ( __taller == true )
				switch ( __sub_root->get_balance() ) {
					case Node::left_higher:
						__sub_root->set_balance(Node::equal_height);
						__taller = false;
						break;
					case Node::equal_height:
						__sub_root->set_balance(Node::right_higher);
						break;
					case Node::right_higher:
						right_balance(__sub_root);
						__taller = false;
						break;
				}
			}
		return __result;
		}

	Node * avl_replace_left ( Node *__sub_root, _Tp &__val )
		{
		Node *__p = __sub_root->_M_left;
		for ( ; __p->_M_right != NULL; __p = __p->_M_right)
			{
			}
			
		if ( __p->_M_left == NULL)
			{
			__val = __p->_M_data;
			return (NULL);
			}
		else
			{
			__val = __p->_M_left->_M_data;
			return (__p);
			}
		}

	Node * avl_replace_right ( Node *__sub_root, _Tp &__val )
		{
		Node *__p = __sub_root->_M_right;
		for ( ; __p->_M_left != NULL; __p = __p->_M_left)
			{
			}
			
		if ( __p->_M_right == NULL)
			{
			__val = __p->_M_data;
			return (NULL);
			}
		else
			{
			__val = __p->_M_right->_M_data;
			return (__p);
			}
		}

	ErrCode avl_remove ( Node * &__sub_root, const _Tp &__old_data, bool &__shorter )
		{
		ErrCode __result = tree_err::success;

		assert ( __sub_root != NULL );

		_Tp __val;

		if ( __old_data == __sub_root->_M_data ) {	// remove
			if ( __sub_root->_M_left != NULL )
				{
					Node *__rsv = avl_replace_left(__sub_root, __val);
					__result = avl_remove(__sub_root->_M_left, __val, __shorter);
					if (__rsv != NULL)
						{
							__sub_root->_M_data = __rsv->_M_data;
							__rsv->_M_data = __val;
						}
					else {
							__sub_root->_M_data = __val;
						}
					// adjust left
					goto __adjust_left;
				}
			else if ( __sub_root->_M_right != NULL )
				{
					Node *__rsv = avl_replace_right(__sub_root, __val);
					__result = avl_remove(__sub_root->_M_right, __val, __shorter);
					if (__rsv != NULL)
						{
							__sub_root->_M_data = __rsv->_M_data;
							__rsv->_M_data = __val;
						}
					else {
							__sub_root->_M_data = __val;
						}
					// adjust right
					goto __adjust_right;
				}
			else {
				delete __sub_root;
				__sub_root = NULL;
				__shorter = true;
				return __result;
				}
			}
		else if ( _M_lt(__old_data, __sub_root->_M_data) ) {	// Removal occurs in left subtree.
			__result = avl_remove(__sub_root->_M_left, __old_data, __shorter);
__adjust_left:
			if ( __shorter == true ) {
				switch ( __sub_root->get_balance() ) {
					case Node::left_higher:
						__sub_root->set_balance(Node::equal_height);
						break;
					case Node::equal_height:
						__sub_root->set_balance(Node::right_higher);
						__shorter = false;
						break;
					case Node::right_higher:
						__shorter = (__sub_root->_M_right->get_balance() != Node::equal_height);
						right_balance(__sub_root);	// rebalancing
						break;
					}
				}
			}
		else {	// Removal occurs in right subtree.
			__result = avl_remove(__sub_root->_M_right, __old_data, __shorter);
__adjust_right:
			if ( __shorter == true )
				switch ( __sub_root->get_balance() ) {
					case Node::left_higher:
						__shorter = (__sub_root->_M_right->get_balance() != Node::equal_height);
						left_balance(__sub_root);	// rebalancing
						break;
					case Node::equal_height:
						__sub_root->set_balance(Node::left_higher);
						__shorter = false;
						break;
					case Node::right_higher:
						__sub_root->set_balance(Node::equal_height);
						break;
				}
			}
		return __result;
		}

public:
	virtual ErrCode insert ( const _Tp &__new_data )
		{
		bool __taller;	// dummy
		return avl_insert(_M_root, __new_data, __taller);
		}

	virtual ErrCode remove ( const _Tp &__old_data )
		{
		bool __shorter;	// dummy
		// replace
		return avl_remove(_M_root, __old_data, __shorter);
		}

	};	// class avl_tree

_QTL_END

#endif	// ifndef _QTL_avltree_H
