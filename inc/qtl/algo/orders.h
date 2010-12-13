/**
 * === Standard Algorithm Library ===
 *  Unit Name:  orders
 *  Author:     Lincoln Yu
 *  Purpose:    Algorithms dealing with order generation
 * == History ==        
 *  2005-11-01: Created
 *  2005-11-02: Debugged on Mac OS X (Tiger)
 *  2005-11-09: factor-based increment and decrement: [:get_inc_next:], [:get_dec_next:] added
 *  2008-04-07: stack-based order commutor, with test script
 *  2008-04-08: commutor improved, can deal with array with identical elements. The same capability 
 *              in [:get_ascend_next:], etc. is verified.
 * == Comments ==
 *  View details in specific functions
 */

#ifndef _QTL_orders_H
#define _QTL_orders_H


#include "../all/predef.h"

#include <vector>
#include <stack>


_QTL_BEGIN

using namespace std;

// get_ascend_next

template <typename _Iterator, typename _Tp> 
    void get_ascend_next (const _Iterator &__begin, const _Iterator &__end, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a objective version
    *
    *  <1> swap [:*(--__watch):] for the last unit bigger than it
    *  <2> units after [:__watch:] are in descendent order
    *
    *  Don't call the function when [:__watch:] == [:__begin:]
    */
    {
    _Iterator __rbegin = __end; --__rbegin;
    _Iterator __it = __rbegin, __it_tmp = __watch; 

    /*  <1>  */
    _Tp __tmp = *--__it_tmp;
    
    for ( ; __it != __watch && !(__tmp < *__it); --__it );  // <2>
    
    *__it_tmp = *__it;
    *__it = __tmp;
    /*  </1>  */
    
    for ( __it = __rbegin, __it_tmp = __watch; __it != __watch && __it != __it_tmp; 
        __it_tmp = __watch, ++__watch, --__it ) 
        {
        __tmp = *__watch;
        *__watch = *__it;
        *__it = __tmp;
        }

    for ( __it = __rbegin, __it_tmp = __it, --__it_tmp; 
        __it != __begin && !(*__it_tmp < *__it); 
        __it = __it_tmp, --__it_tmp );
    
    __watch = __it;
    }

template <typename _Iterator, typename _Swapper, typename _LT> 
    void get_ascend_next (const _Iterator &__begin, const _Iterator &__end, 
    _Swapper __swap, _LT __lt, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a functorized version
    *
    *  <1> swap [:*(--__watch):] for the last unit bigger than it
    *  <2> units after [:__watch:] are in descendent order
    *
    *  Don't call the function when [:__watch:] == [:__begin:]
    */
    {
    _Iterator __rbegin = __end; --__rbegin;
    _Iterator __it = __rbegin, __it_tmp = __watch;

    /*  <1>  */
    --__it_tmp;
    
    for ( ; __it != __watch && !__lt(*__it_tmp, *__it) ; --__it );  // <2>

    __swap(__it, __it_tmp);
    /*  </1>  */
    
    for ( __it = __rbegin, __it_tmp = __watch; __it != __watch && __it != __it_tmp; 
        __it_tmp = __watch, ++__watch, --__it ) 
        {
        __swap(__it, __watch);
        }

    for ( __it = __rbegin, __it_tmp = __it, --__it_tmp; 
        __it != __begin && !__lt(*__it_tmp, *__it); 
        __it = __it_tmp, --__it_tmp );
    
    __watch = __it;
    }


// get_descend_next

template <typename _Iterator, typename _Tp> 
    void get_descend_next (const _Iterator &__begin, const _Iterator &__end, _Iterator &__watch)
    /* Comments:
    *
    *  This is a objective version.
    *
    *  <1> swap [:*(--__watch):] for the last unit bigger than it
    *  <2> units after [:__watch:] are in descendent order
    *
    *  Don't call the function when [:__watch:] == [:__begin:]
    */
    {
    _Iterator __rbegin = __end; --__rbegin;
    _Iterator __it = __watch, __it_tmp = __watch;

    /*  <1>  */
    _Tp __tmp = *--__it_tmp;
    
    for ( ++__it;  __it != __end && !(__tmp < *__it); ++__it ); // <2>
    
    *__it_tmp = *--__it;
    *__it = __tmp;
    /*  </1>  */
    
    for ( __it = __rbegin, __it_tmp = __watch; __it != __watch && __it != __it_tmp; 
        __it_tmp = __watch, ++__watch, --__it ) 
        {
        __tmp = *__watch;
        *__watch = *__it;
        *__it = __tmp;
        }

    for ( __it = __rbegin, __it_tmp = __it, --__it_tmp; 
        __it != __begin && !(*__it < *__it_tmp) 
        ; __it = __it_tmp, --__it_tmp );
    
    __watch = __it;
    }

template <typename _Iterator, typename _Swapper, typename _LT> 
    void get_descend_next (const _Iterator &__begin, const _Iterator &__end, 
    _Swapper __swap, _LT __lt, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a functorized version
    *
    *  <1> swap [:*(--__watch):] for the last unit bigger than it
    *  <2> units after [:__watch:] are in descendent order
    *
    *  Don't call the function when [:__watch:] == [:__begin:]
    */
    {
    _Iterator __rbegin = __end; --__rbegin;
    _Iterator __it = __watch, __it_tmp = __watch;

    /*  <1>  */
    --__it_tmp;
    
    for ( ++__it;  __it != __end && !__lt(*__it_tmp, *__it); ++__it );  // <2>
    
    __swap(--__it, __it_tmp);
    /*  </1>  */
    
    for ( __it = __rbegin, __it_tmp = __watch; __it != __watch && __it != __it_tmp; 
        __it_tmp = __watch, ++__watch, --__it ) 
        {
        __swap(__it, __watch);
        }

    for ( __it = __rbegin, __it_tmp = __it, --__it_tmp; 
        __it != __begin && !__lt(*__it, *__it_tmp)
        ; __it = __it_tmp, --__it_tmp );
    
    __watch = __it;
    }


// get_ascend_watch

template <typename _Iterator>
    void get_ascend_watch (const _Iterator &__begin, const _Iterator &__end, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a objective version.
    *
    */
    {
    __watch = __end;

    _Iterator __it_bef = --__watch;
    
    for ( --__it_bef; __watch != __begin && !(*__it_bef < *__watch); __watch = __it_bef, --__it_bef);
    }

template <typename _Iterator, typename _LT>
    void get_ascend_watch (const _Iterator &__begin, const _Iterator &__end, _LT __lt, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a functorized version.
    *
    */
    {
    __watch = __end;

    _Iterator __it_bef = --__watch;
    
    for ( --__it_bef; __watch != __begin && !__lt(*__it_bef, *__watch); __watch = __it_bef, --__it_bef);
    }


// get_descend_watch

template <typename _Iterator>
    void get_descend_watch (const _Iterator &__begin, const _Iterator &__end, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a objective version.
    *
    */      
    {
    __watch = __end;

    _Iterator __it_bef = --__watch;
    
    for ( --__it_bef; __watch != __begin && !(*__watch < *__it_bef); __watch = __it_bef, --__it_bef);
    }

template <typename _Iterator, typename _LT>
    void get_descend_watch (const _Iterator &__begin, const _Iterator &__end, _LT __lt, _Iterator &__watch)
    /* == Comments ==
    *
    *  This is a functorized version.
    *
    */
    {
    __watch = __end;

    _Iterator __it_bef = --__watch;
    
    for ( --__it_bef; __watch != __begin && !__lt(*__watch, *__it_bef); __watch = __it_bef, --__it_bef);
    }


// get_inc_next

template <typename _SrcIter, typename _FctIter>
    void get_inc_next(const _SrcIter &__src_begin, const _SrcIter &__src_end, 
    const _FctIter &__fct_begin, const _FctIter &__fct_end)
    /* == Comments ==
    *
    *  This is a objective version.
    *
    */
    {
    _SrcIter __it_src;
    _FctIter __it_fct;

    for ( __it_src = __src_begin, __it_fct = __fct_begin; 
        !(*__it_src + 1 < *__it_fct);   /* one more */
        *__it_src = 0, ++__it_src, ++__it_fct )
    {
    }
    ++*__it_src;
    }

template <typename _SrcIter, typename _FctIter, typename _LT, typename _Zero, typename _Inc>
    void get_inc_next (const _SrcIter &__src_begin, const _SrcIter &__src_end, 
    const _FctIter &__fct_begin, const _FctIter &__fct_end, 
    _LT __lt, _Zero __zero, _Inc __inc)
    /* == Comments ==
    *
    *  This is a functorized version.
    *
    *  Pay attention that the comparison is between [:*__it_src + 1:] and [:__it_fct:], 
    *  view objective counterpart
    *
    */
    {
    _SrcIter __it_src;
    _FctIter __it_fct;

    for ( __it_src = __src_begin, __it_fct = __fct_begin; 
        !__lt(*__it_src, *__it_fct); 
        __zero(*__it_src), ++__it_src, ++__it_fct )
        {
        }
    __inc(*__it_src);
    }


// get_dec_next

template <typename _SrcIter, typename _FctIter>
    void get_dec_next (const _SrcIter &__src_begin, const _SrcIter &__src_end, 
    const _FctIter &__fct_begin, const _FctIter &__fct_end)
    /* == Comments ==
    *
    *  This is a objective version.
    *
    */
    {
    _SrcIter __it_src;
    _FctIter __it_fct;

    for ( __it_src = __src_begin, __it_fct = __fct_begin; 
        !(*__it_src > 0); 
        *__it_src = *__it_fct - 1 /* one less */,  ++__it_src, ++__it_fct )
        {
        }
    --*__it_src;
    }

template <typename _SrcIter, typename _FctIter, typename _NZ, typename _Map, typename _Dec>
    void get_dec_next (const _SrcIter &__src_begin, const _SrcIter &__src_end, 
    const _FctIter &__fct_begin, const _FctIter &__fct_end, 
    const _NZ &__nz, const _Map &__map, const _Dec &__dec)
    /* == Comments ==
    *
    *  This is a functorized version.
    *
    *  Pay attention that the map is from [:*__it_fct - 1:] to [:*__it_src:],
    *  view objective counterpart
    *
    */
    {
    _SrcIter __it_src;
    _FctIter __it_fct;

    for ( __it_src = __src_begin, __it_fct = __fct_begin; 
        !__nz(*__it_src); 
        __map(*__it_src, *__it_fct), ++__it_src, ++__it_fct )
        {
        }
    __dec(*__it_src);
    }

// class assignment_order

class assignment_order
    {
    struct rec
        {
        unsigned int step;
        int trial;
        };

    public:
        void init (int n, const vector<int> &avail)
            {
            _M_order.resize(n);
            _M_unused = avail;
            _M_r.step = 0;
            _M_r.trial = 0;
            }

        bool get_next ()
            {
            bool found;
            while (1)
                {
                found = false;
                for (unsigned int i = _M_r.trial; i < _M_unused.size(); i++)
                    if (_M_unused[i] > 0)
                        {
                        found = true;
                        _M_unused[i]--;
                        _M_r.trial = i + 1;
                        _M_stack.push(_M_r);
                        _M_order[_M_r.step] = i; 
                        _M_r.trial = 0;
                        _M_r.step++;
                        break;
                        }
                if(!found)
                    {
                    if (_M_stack.empty())
                        {
                        break;
                        }
                    _M_r = _M_stack.top();
                    _M_stack.pop();
                    _M_unused[_M_r.trial-1]++;
                    }
                else if (_M_r.step >= _M_order.size())
                    {
                    _M_r = _M_stack.top();
                    _M_stack.pop();
                    _M_unused[_M_r.trial-1]++;
                    break;
                    }
                }
            return found;
            }

    protected:
        stack<rec>  _M_stack;
        vector<int> _M_order;
        vector<int> _M_unused;
        rec         _M_r;
    };  // class assignment_order


template <class _Itr>
struct OrderCommutorTraits
{
	typedef std::vector<_Itr>			Vector;
	typedef typename Vector::iterator	VecItr;

};

template <class _Itr, class _T, class _Traits = OrderCommutorTraits<_Itr> >
class OrderCommutor
{
public:
	typedef _Itr	Itr;

	typedef typename _Traits::Vector		ItrRef;
	typedef typename ItrRef::iterator		ItrRefItr;
	
	enum Type
	{
		Little = -1,
		Symmetric = 0,
		Big = 1
	};


	void Init (const Itr &begin, const Itr &end)
	{
		m_Begin = begin;
		m_Last = end;
		--m_Last;

		
		m_ItrRef.clear();
		for (Itr itr = begin; itr != m_Last; ++itr)
		{
			m_ItrRef.push_back(m_Last);
		}

		m_Last2 = m_Last;
		--m_Last2;

		m_IriLast = m_ItrRef.end();
		--m_IriLast;

		m_Cur = m_Last2;
		m_IriCur = m_IriLast;
		
		m_Done = false;
	}

	bool Done ()
	{
		return m_Done;
	}

	Type GetType ()
	{
		Itr left = m_Begin;
		Itr right = m_Last;
		for ( ; ; )
		{
			if (*left > *right)
			{
				return Big;
			}
			else if (*left < *right)
			{
				return Little;
			}
			++left;
			if (left == right)
			{
				break;
			}
			--right;
			if (left == right)
			{
				break;
			}
		}
		return Symmetric;
	}

	void Inc ()
	{
	__begin:
		while (*m_IriCur == m_Cur)
		{
			if (m_Cur == m_Begin)
			{
				m_Done = true;
				return;
			}
			--m_Cur;
			--m_IriCur;
		}
		Itr i = (*m_IriCur)--;
		while (*m_Cur == *i)
		{
			if (*m_IriCur == m_Cur)
			{
				goto __begin;
			}
			i = (*m_IriCur)--;
		}

		Swap(*m_Cur, *i);

		Itr left = m_Cur;
		Itr right = m_Last;
		
		for ( ++left; left != right; )
		{
			Swap(*left, *right);

			++left;
			if (left == right)
			{
				break;
			}
			--right;
		}

		ItrRefItr iri = m_IriCur;
		for ( ++iri; iri != m_ItrRef.end(); ++iri )
		{
			*iri = m_Last;
		}

		m_Cur = m_Last2;
		m_IriCur = m_IriLast;

	}

protected:
	void Swap (_T &a, _T &b) { _T t = a; a = b; b = t; }

protected:
	Itr		m_Cur, m_Begin, m_Last, m_Last2;
	ItrRefItr	m_IriCur, m_IriLast;
	ItrRef	m_ItrRef;
	bool	m_Done;

};

_QTL_END

#if defined (QTL_DEBUG_ORDERS) && QTL_DEBUG_ORDERS

int test_array[] = {0, 0, 1, 2, 2};
#define len_array	(sizeof(test_array)/sizeof(int))

int TestMain (void)
{
	using namespace qtl;
	typedef OrderCommutor<int *, int>	Commutor;
	Commutor	commutor;
	commutor.Init(test_array, test_array + len_array);
	while (!commutor.Done())
	{
		for (int j = 0; j < len_array; j++)
		{
			printf("%d ", test_array[j]);
		}
		printf("\n");
		commutor.Inc();
		while (!commutor.Done() && commutor.GetType() == Commutor::Big)
		{
			commutor.Inc();
		}
	}
}

int TestMain2 (void)
{
	using namespace qtl;
	int *watch;
	get_ascend_watch<int*>(test_array, test_array + len_array, watch);

	while (1)
	{
		for (int j = 0; j < len_array; j++)
		{
			printf("%d ", test_array[j]);
		}
		printf("\n");
		if (test_array == watch)
		{
			break;
		}
		get_ascend_next<int*, int>(test_array, test_array + len_array, watch);
	}

}

#endif	/* if defined (QTL_DEBUG_ORDERS) && QTL_DEBUG_ORDERS */

#endif  // ifndef _QTL_orders_H
