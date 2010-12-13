#include <cstdio>

/**
 * === Standard Algorithm Library ===
 *  Unit Name:  sortings
 *  Author:     Lincoln Yu
 *  Purpose:    Sorting algorithms
 * == History ==        
 *  2005-11-23: Created, with several implementation of quick sort.
 *	2008-03-03:	Heap sort added, with test code.
 *  2008-03-04:	Heap sort reimplemented.
 * == Comments ==
 *  Heap sorting: view details at the function declaration
 *
 *  book abbreviations: 
 *    CPL - C Programming Language, by Biran W. Kernighan and Dennis M. Ritchie
 *    Data Structure - Described in C, by Yan Wei-min & Wu Wei-min
 */

#ifndef _QTL_sortings_H
#define _QTL_sortings_H

#include "../all/predef.h"

_QTL_BEGIN

#ifndef _W64
#   define _W64
#endif

template <typename _Iter, typename _Int = _W64 int>
	    struct get_mid
	    /* for contiguous lists */
	{
	bool operator() (const _Iter &__first, const _Iter &__last, _Iter &__pivot)
		{
        _Int __mid = __last - __first;	// cannot be unsigned, because [:__last:] may exceed [:__first:]
		if (--__mid <= 0)
			return (false);
		__mid /= 2;
		__pivot = __first + __mid;
		return (true);
		}
	};

template <typename _Iter, typename _Less, typename _Swap, typename _GetMid>
	    void qsort (const _Iter &__first, const _Iter &__last, _Less __less, _Swap __swap, _GetMid __get_mid)
        /*	Post:	[[:__first:], [:__last:]) is sorted.
         *	Ref:	[CPL], Section 5.7
         *  Hint:	The pivot keeps the less value itself and on its left, actual PIVOT VALUE for decision is stored at [:__first:].
         */
	{
	_Iter __pivot;
	if (!__get_mid(__first, __last, __pivot)) return;
	__swap(__first, __pivot);
	__pivot = __first;
	_Iter __it = __first;
	// [:__it < __last:] is replaced by [:__it != __last:]
	for ( ++__it; __it != __last; ++__it)
		if ( __less(*__it, *__first) )
			__swap(++__pivot, __it);
	__swap(__first, __pivot);
	qsort(__first, __pivot, __less, __swap, __get_mid);
	qsort(++__pivot, __last, __less, __swap, __get_mid);
	}

template <typename _Tp, typename _Iter, typename _Less, typename _Swap, typename _GetMid>
        void qsort2 (const _Iter &__first, const _Iter &__last, _Less __less, _Swap __swap, _GetMid __get_mid)
        /*	Post:	[[:__first:], [:__last:]) is sorted.
         *	Ref:	by Robbie Mosaic
         *	Hint:	
         */
	{
	_Iter __tmp;
	__get_mid(__first, __last, __tmp);
	_Iter __left = __first;
	_Iter __right = __last; --__right;
	_Iter __i = __left;
	_Iter __j = __right;

	_Tp __mid_val = *__tmp;

	do
		{
		while (__less(*__i, __mid_val) ) //&& __i < __right)
			++__i;
		while (__less(__mid_val, *__j) ) // && __j > __left)
			--__j;

		// [:__i:] holds value no less than pivot
		// [:__j:] holds value no more than pivot
		// so if [:__i == __j:], the value they both holds is just pivot

		
		if (__i != ++(__tmp = __j)) {
			__swap(__i, __j);
			if (__i == __j)
				{
				++__i, --__j;
				break;
				}
			++__i, --__j;
			}
		else
			{
			break;
			}

		} while (true);

	if (__left != __j && __left != ++(__tmp = __j))
		qsort2<_Tp, _Iter, _Less, _Swap, _GetMid>(__left, __tmp, __less, __swap, __get_mid);

	if (__right != __i && ++(__tmp = __right) != __i)
		qsort2<_Tp, _Iter, _Less, _Swap, _GetMid>(__i, __last, __less, __swap, __get_mid);
	}

template <typename _Iter, typename _Comp, typename _Swap, typename _GetMid>
        void qsort_dede (const _Iter &__first, const _Iter &__last, _Comp __comp, _Swap __swap, _GetMid __get_mid)
        /*	Post:	[[:__first:], [:__last:]) is sorted.
         *	Ref:	by Robbie Mosaic
         *	Hint:	LESS block and MORE block with a PIVOT shuttle between both sides, and the GAP turns narrower to zero.
         */
	{

	_Iter __pivot;
	if ( !__get_mid(__first, __last, __pivot) )
		return;
	_Iter __left = __first;
	_Iter __right = __last; --__right;
	__swap(__left, __pivot);

	while ( true )
		{
		int __comp_res;
		// [:__it < __last:] is replaced by [:__it != __last:]
		for ( ; __left != __right && (__comp_res = __comp(*__left, *__right)) < 0; --__right );

		if ( __left == __right )
			break;
		if ( __comp_res == 0 ) {
			++__left;
			}
		else {
			__swap(__left, __right);
			}

		// the pivot is now at [:__right:]
		// [:__left < __right:] is replaced by [:__left != __right:]
		for ( ; __left != __right && (__comp_res = __comp(*__left, *__right)) < 0; ++__left );

		if ( __left == __right )
			break;
		if ( __comp_res == 0 ) {
			--__right;
			}
		else {
			__swap(__left, __right);
			}

		}

	qsort_dede(__first, __left, __comp, __swap, __get_mid);
	qsort_dede(++__left, __last, __comp, __swap, __get_mid);
	
	}

/* 
 * first element is in[1].
 */
template <class _Swap, class _Elem = int, 
          class _VecOneBased = _Elem *, class _Index = int>
struct heap_sort_traits
{
    typedef _VecOneBased    VecOneBased;
    typedef _Swap   Swap;
    typedef _Elem   Elem;
    typedef _Index  Index;
};

template <class _Traits>
inline void heap_adjust (typename _Traits::VecOneBased &vec, 
                         const typename _Traits::Index &begin, 
                         const typename _Traits::Index &end)
{   /* Establishing big top heap */
    typedef typename _Traits::Index    Index;
    typedef typename _Traits::Elem     Elem;

    Elem top = vec[begin];
    Index i = begin;
    Index j = i * 2;
    for ( ; j <= end; i = j, j *= 2)
    {
        if (j < end && vec[j] < vec[j+1])
        {
            ++j;
        }
        if (top < vec[j]) { vec[i] = vec[j]; }
        else { break; }
    }
    vec[i] = top;
}

template <class _Traits>
void heap_sort (typename _Traits::VecOneBased &vec,
                const typename _Traits::Index &count, 
                typename _Traits::Swap __swap)
{
    typedef typename _Traits::Index    Index;
    typedef typename _Traits::Elem     Elem;

    for (Index i = count/2; i > 0; --i)
    {
        heap_adjust<_Traits>(vec, i, count);
    }

    for (Index i = count; i > 2; --i)
    {
        __swap(vec[1], vec[i]);
        heap_adjust<_Traits>(vec, 1, i - 1);
    }
    __swap(vec[1], vec[2]);
}

_QTL_END

#endif	/* ifndef _QTL_sortings_H */


#if defined(_test_heap_sort) && _test_heap_sort

#include <cstdio>
#include <cstdlib>

void init (int *p, int size)
{
    for (int i = 0; i < size; i++)
    {
        p[i] = rand() % 100;

    }
}

void view (int *p, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%2d ", p[i]);
        if (i % 16 == 15)
        {
            printf("\n");
        }
    }
}

void int_swap (int &a, int &b) { int tmp = a; a = b; b = tmp; }

int main ()
{
#define VEC_SIZE	32
	using namespace qtl;

    static int vec[VEC_SIZE];
    int n = VEC_SIZE;
    int *pvec = vec - 1;
    init(vec, n);
    view(vec, n);
    printf("heap sorting...");
    heap_sort< heap_sort_traits< void(*)(int&,int&) > >
        (pvec, n, int_swap);
    printf("ok\n");
    view(vec, n);
}


#endif	/* if defined(_test_heap_sort) && _test_heap_sort  */

