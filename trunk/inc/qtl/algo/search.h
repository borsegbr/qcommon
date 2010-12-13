#ifndef _search_H
#define _search_H

#include <cstdio>


#include "../all/predef.h"

_QTL_BEGIN

#ifndef SWCH_search_H_EQUALSKIPPER_DIMIDIATE
#   define SWCH_search_H_EQUALSKIPPER_DIMIDIATE		0
#endif

#ifndef SWCH_search_H_NUDGE_CRESENDO
#   define SWCH_search_H_NUDGE_CRESENDO             0
#endif

#ifndef TEST_search_H
#   define TEST_search_H                            0
#endif

template <class Traits>
struct StepInvalidTraits
{
	typedef typename Traits::VecRef		VecRef;
	typedef typename Traits::Idx		Idx;
	typedef typename Traits::ValidRef	CondRef;
};

/*
 * No specific condidition is required.
 *
 * search in range [ib, k) for the point closest to k that cond holds or return (ib-1)
 * k must not fullfil cond
 *
 */

template <class Traits>
void step_left_until(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ib, typename Traits::CondRef cond)
{
#if defined(SWCH_searh_H_STEPSEARCH_AGGRESSIVE) && SWCH_searh_H_STEPSEARCH_AGGRESSIVE
	cond.step_left_until(vec, k, ib);
#else
	for ( ; k >= ib && !cond(vec, k) ; --k);
#endif	/* if defined(SWCH_searh_H_STEPSEARCH_AGGRESSIVE) && SWCH_searh_H_STEPSEARCH_AGGRESSIVE */
}

/*
 * No specific condidition is required.
 * k != ie must be ensured
 *
 * search in range (k, ie) for the point closest to k that cond holds or return (ib-1)
 * k must not fullfil cond
 *
 */
template <class Traits>
void step_right_until(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::CondRef cond)
{
#if defined(SWCH_searh_H_STEPSEARCH_AGGRESSIVE) && SWCH_searh_H_STEPSEARCH_AGGRESSIVE
	cond.step_right_until(vec, k, ie);
#else
	for ( ; k < ie && !cond(vec, k) ; ++k);
#endif	/* if defined(SWCH_searh_H_STEPSEARCH_AGGRESSIVE) && SWCH_searh_H_STEPSEARCH_AGGRESSIVE */
}

/*
 * Make sure that there is no uncond ones left to the first cond one
 *
 * search in range [ib, k) for the point closest to k that cond holds or return (ib-1)
 * k must not fullfil cond
 *
 * in each iteration, cond(k)==false, cond(ib)==?, cond(ib-1)==true (or out of range)
 */
template <class Traits>
void step_left_until_dimidiate(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ib, typename Traits::CondRef cond)
{
    typedef typename Traits::Idx    Idx;
    // cond(ib) probably true
    // cond(k) == false
    for ( ; ib < k; )
    {
        Idx im = (ib + k) / 2;
        if (cond(vec, im))
        {
            ib = im + 1;
        }
        else
        {
            k = im;
        }
    }
    --k;
}

/*
 * Make sure that there is no uncond ones right to the first cond one
 *
 * search in range (k, ie) for the point closest to k that cond holds or return ie
 * k must not fullfil cond
 *
 */
template <class Traits>
void step_right_until_dimidiate(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::CondRef cond)
{
    typedef typename Traits::Idx    Idx;
    for ( ; k < ie; )
    {
        Idx im = (k + ie) / 2;
        if (cond(vec, im))
        {
            ie = im;
        }
        else
        {
            k = im + 1;
        }
    }
}

/*
 *
 * Make sure that there is no uncond ones left to the first cond one
 *
 * search in range [ib, k) for the point closest to k that cond holds or return (ib-1)
 * k must not fullfil cond
 *
 * in each iteration, cond(k)==false, cond(ib)==?, cond(ib-1)==true (or out of range)
 */
template <class Traits>
void step_left_until_crescendo(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ib, typename Traits::CondRef cond)
{
    typedef typename Traits::Idx    Idx;
    const Idx k_DStep = 4;  // 4 looks nice
    Idx d;
    Idx lastk = k;

    d = 1;
    do
    {
        lastk = k;
        k -= d;
        d *= k_DStep;
    } 
    while (k >= ib && !cond(vec, k));
    /* last k doesn't satisfy cond while k satisfies or less than ib */
    
    if (k < ib)
    {
        k = ib;
        if (lastk == k)
        {
            k = ib - 1;
            return;
        }
    }

    step_left_until_dimidiate<Traits>(vec, lastk, k, cond);
    k = lastk;
}

/*
 * Make sure that there is no uncond ones right to the first cond one
 *
 * search in range (k, ie) for the point closest to k that cond holds or return ie
 * k must not fullfil cond
 *
 * in each iteration, cond(k)==false, cond(ie-1)==?, cond(ie)==true (or out of range)
 */
template <class Traits>
void step_right_until_crescendo(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::CondRef cond)
{
    typedef typename Traits::Idx    Idx;
    const Idx k_DStep = 4;
    Idx d;
    Idx lastk;

    d = 1;
    do
    {
        lastk = k; 
        k += d; 
        d *= k_DStep;
    }
    while (k < ie && !cond(vec, k));


    if (k >= ie)
    {
        k = ie;
        if (lastk == ie - 1)
        {
            k = ie;
            return;
        }
    }
    
    step_right_until_dimidiate<Traits>(vec, lastk, k, cond);
    k = lastk;
}

/*
 * find the leftmost valid element where cond holds
 *
 * in each iteration:
 * [ib, k) is investigated
 * with vec[k] = true
 *      vec[ib] == ?
 *      vec[ib-1] == true or out of range
 */

template <class Traits>
void step_to_leftmost_cond_dimidiate(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ib, typename Traits::ValidRef valid, typename Traits::CondRef cond)
{
	typedef typename Traits::Idx	Idx;
	Idx m;

	for ( ; ib < k; )
	{
		m = (ib + k) / 2;
		if (!valid(vec, m))
		{
			Idx tm = m;
			step_right_until< StepInvalidTraits<Traits> >(vec, tm, k, valid);
			if (cond(vec, tm))
			{	// investigate left
				step_left_until< StepInvalidTraits<Traits> >(vec, m, ib, valid);
				if (m < ib)
				{	// no valid element
					k = tm;
					return;
				}
				else
				{
					if (cond(vec, m))
					{
						k = m;	// investigate range [ib, k)
					}
					else
					{
						k = tm;
						return;
					}
				}
			}
			else
			{	// investigate right
				ib = m;	// investigate range [ib, k)
			}
		}
		else
		{
			if (cond(vec, m))
			{	// investigate left
				k = m;
			}
			else
			{	// investigate right
				ib = m + 1;
			}
		}
	}
}

/*
 * find to the right the first valid one where cond doesn't hold
 * 
 * in each iteration:
 * (k, ie) is investigated
 * with vec[k] == true
 *      vec[ie-1] == ?
 *      vec[ie] == true or out of range
 */

template <class Traits>
void skip_cond_right_dimidiate(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::ValidRef valid, typename Traits::CondRef cond)
{
	typedef typename Traits::Idx	Idx;
	Idx m;

	for ( ; k < ie; )
	{
		m = (k + ie) / 2;
		if (!valid(vec, m))
		{
			Idx tm = m;
			step_left_until< StepInvalidTraits<Traits> >(vec, tm, k, valid);
			if (cond(vec, tm))
			{	// investigate right
				step_right_until< StepInvalidTraits<Traits> >(vec, m, ie, valid);
				if (m >= ie)
				{	// no valid element
					k = m;	// different from step_to_leftmost_equal
					return;
				}
				else
				{
					if (cond(vec, m))
					{
						k = m + 1;	// investigate range [k, ie)
					}
					else
					{
						k = m;	// different from step_to_leftmost_equal
						return;
					}
				}
			}
			else
			{	// investigate left
				ie = m;
			}
		}
		else
		{
			if (cond(vec, m))
			{	// investigate right
				k = m + 1;
			}
			else
			{	// investigate left
				ie = m;
			}
		}
	}
}


/*
 * find the leftmost valid element where cond holds
 *
 * in each iteration:
 * [ib, k) is investigated
 * with vec[k] = true
 *      vec[ib] == ?
 *      vec[ib-1] == true or out of range
 */
template <class Traits>
void step_to_leftmost_cond_cresendo(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ib, typename Traits::ValidRef valid, typename Traits::CondRef cond)
{
    typedef typename Traits::Idx    Idx;
    const Idx k_DStep = 4;
    Idx d;
    Idx lastk = k;

    d = 1;
    do
    {
        lastk = k;
        k -= d;
        d *= k_DStep;

        if (!valid(vec, k))
        {
            step_left_until< StepInvalidTraits<Traits> >(vec, k, ib, valid);
        }
    }
    while (k >= ib && cond(vec, k));
    if (k < ib)
    {
        k = ib;
    }
    /* lastk doesn't satisfy cond while k satisfies or less than ib */

    // dimidiate search in range [ib, lastk);
    step_to_leftmost_cond_dimidiate<Traits>(vec, lastk, k, valid, cond);
    k = lastk;
}

/*
 * find to the right the first valid one where cond doesn't hold
 * 
 * in each iteration:
 * (k, ie) is investigated
 * with vec[k] == true
 *      vec[ie-1] == ?
 *      vec[ie] == true or out of range
 */

template <class Traits>
void skip_cond_right_cresendo(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::ValidRef valid, typename Traits::CondRef cond)
{
    typedef typename Traits::Idx    Idx;
    const Idx k_DStep = 4;
    Idx d;
    Idx lastk;

    d = 1;
    do
    {
        lastk = k; 
        k += d; 
        d *= k_DStep;

        if (!valid(vec, k))
        {
            step_right_until< StepInvalidTraits<Traits> >(vec, k, ie, valid);
        }
    }
    while (k < ie && cond(vec, k));

    if (k >= ie)
    {
        k = ie;
        if (lastk == ie - 1)
        {
            k = ie;
            return;
        }
    }
    
    skip_cond_right_dimidiate<Traits>(vec, lastk, k, valid, cond);
    k = lastk;
}

template <class Traits>
struct SkipEqCondDimidTraits
{
	typedef typename Traits::VecRef		VecRef;
	typedef typename Traits::Idx		Idx;
	typedef typename Traits::ValidRef	ValidRef;

	struct Cond
	{
		Idx	pt;

		Cond(Idx _pt) : pt(_pt) { }

		bool operator()(VecRef vec, Idx idx)
		{
			return vec[idx] == vec[pt];
		}
	};

	typedef Cond &	CondRef;
};


template <class Traits>
void step_to_leftmost_equal(typename Traits::VecRef vec, 
	typename Traits::Idx &k, typename Traits::Idx ib, typename Traits::ValidRef valid)
{
	typedef SkipEqCondDimidTraits<Traits>	SkipTraits;
	typename SkipTraits::Cond cond(k);
#if SWCH_search_H_NUDGE_CRESENDO
	step_to_leftmost_cond_cresendo<SkipTraits>(vec, k, ib, valid, cond);
#else
	step_to_leftmost_cond_dimidiate<SkipTraits>(vec, k, ib, valid, cond);
#endif
}

template <class Traits>
void skip_equal_right(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::ValidRef valid)
{
	typedef SkipEqCondDimidTraits<Traits>	SkipTraits;
	typename SkipTraits::Cond cond(k);
#if SWCH_search_H_NUDGE_CRESENDO
	skip_cond_right_cresendo<SkipTraits>(vec, k, ie, valid, cond);
#else
	skip_cond_right_dimidiate<SkipTraits>(vec, k, ie, valid, cond);
#endif
}


template <class Traits>
bool search_dimidiate(typename Traits::VecRef vec, typename Traits::EntryRef t, 
    typename Traits::Idx &ib, typename Traits::Idx &ie, 
    typename Traits::CompRef comp, typename Traits::ValidRef valid);


template <class Traits>
void step_to_leftmost_equal_using_search_dimidiate(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ib, typename Traits::CompRef comp, typename Traits::ValidRef valid)
{
	/*
	 * search (recursively) vec[k] within range [ib, k), obviously the return value of ib is 'the leftmose equal'
	 */
	search_dimidiate<Traits>(vec, vec[k], ib, k, comp, valid);
	k = ib;
}

template <class Traits>
void skip_equal_right_using_search_dimidiate(typename Traits::VecRef vec, typename Traits::Idx &k, 
	typename Traits::Idx ie, typename Traits::CompRef comp, typename Traits::ValidRef valid)
{
	/*
	 * search (recursively) vec[k] within range [k+1, ie), obviously the return value of ie is the first unequal
	 */
	typename Traits::Idx tmp = k;
	++k;	// to avoid repetition of same situation in the recursion
	search_dimidiate<Traits>(vec, vec[tmp], k, ie, comp, valid);
	k = ie;
}


/*
 * vec - the container of the array
 * t - the target key
 * ib - in: lower bound of search; out: index to the first key no less than the searched key or end of array
 * ie - in: upper bound of search; out: index to the first key greater than the searched key or end of array
 * valid - testor checking the validity of the key
 *
 * no specific requirements, even array with no valid item is acceptable
 * when there is no valid element, ib and ie points to the cell next the last
 *
 * return value if found return true
 */
template <class Traits>
bool search_dimidiate(typename Traits::VecRef vec, typename Traits::EntryRef t, 
    typename Traits::Idx &ib, typename Traits::Idx &ie, typename Traits::CompRef comp, 
    typename Traits::ValidRef valid)
{
    typedef typename Traits::Idx Idx;
	Idx kl, kr;
    for (; ib < ie; )
    {
        Idx k = (ib+ie)/2;
        if (!valid(vec, k))
        {
            int rcl, rcr;
            kl = k;

            step_left_until< StepInvalidTraits<Traits> >(vec, kl, ib, valid);

            if (kl < ib) { rcl = 1; }	// stop considering left side
            else { rcl = comp(t, vec[kl]); }
            if (rcl < 0)
            {	/* t < vec[kl] */
                ie = kl;
            }
            else
            {
                kr = k;
                
				step_right_until< StepInvalidTraits<Traits> >(vec, kr, ie, valid);

                if (kr >= ie) { rcr = -1; }	// stop considering right side
                else { rcr = comp(t, vec[kr]); }
                if (rcr > 0)
                {	/* t > vec[kr] */
                    ib = kr + 1;
                }
                else
                {
                    if (rcl == 0 && rcr < 0)
                    {	
						/*  
					     *	suppose t = 3
						 *  ..3XXXXX4..
						 *    ^     ^
						 *   kl     kr
					     */
#if defined(SWCH_search_H_EQUALSKIPPER_DIMIDIATE) && SWCH_search_H_EQUALSKIPPER_DIMIDIATE
                        step_to_leftmost_equal_using_search_dimidiate<Traits>(vec, kl, ib, valid);
#else
						step_to_leftmost_equal<Traits>(vec, kl, ib, valid);
#endif
                        ib = kl;
                        ie = kr;
                        return true;
                    }
                    else if (rcl == 0 && rcr == 0)
                    {
						/*  
					     *	suppose t = 3
						 *  ..3XXXXX3..
					     */
#if defined(SWCH_search_H_EQUALSKIPPER_DIMIDIATE) && SWCH_search_H_EQUALSKIPPER_DIMIDIATE
                        step_to_leftmost_equal_using_search_dimidiate<Traits>(vec, kl, ib, valid);
                        skip_equal_right_using_search_dimidiate<Traits>(vec, kr, ie, valid);
#else
                        step_to_leftmost_equal<Traits>(vec, kl, ib, valid);
                        skip_equal_right<Traits>(vec, kr, ie, valid);
#endif
                        ib = kl;
                        ie = kr;
                        return true;
                    }
                    else if (rcl > 0 && rcr == 0)
                    {
						/*  
					     *	suppose t = 3
						 *  ..2XXXXX3..
					     */
                        ib = kr;
#if defined(SWCH_search_H_EQUALSKIPPER_DIMIDIATE) && SWCH_search_H_EQUALSKIPPER_DIMIDIATE
                        skip_equal_right_using_search_dimidiate<Traits>(vec, kr, ie, valid);
#else
						skip_equal_right<Traits>(vec, kr, ie, valid);
#endif
                        ie = kr;
                        return true;
                    }
                    else    /* rcl > 0 && rcr < 0 */
                    {
						/*  
					     *	suppose t = 3
						 *  ..2XXXXX4..
					     */
                        ib = kr;
                        ie = kr;
                        return false;
                    }
                }
            }
        }
        else
        {
            int cmptk = comp(t, vec[k]);
            if (cmptk < 0)
            {	// 
                ie = k;
            }
            else if (cmptk > 0)
            {	// 
                ib = k + 1;
            }
            else
            {	// vec[t] == vec[k]
				kl = kr = k;
#if defined(SWCH_search_H_EQUALSKIPPER_DIMIDIATE) && SWCH_search_H_EQUALSKIPPER_DIMIDIATE
				step_to_leftmost_equal_using_search_dimidiate<Traits>(vec, kl, ib, valid);
                skip_equal_right_using_search_dimidiate<Traits>(vec, kr, ie, valid);
#else
				step_to_leftmost_equal<Traits>(vec, kl, ib, valid);
                skip_equal_right<Traits>(vec, kr, ie, valid);
#endif
				ib = kl;
				ie = kr;
                return true;
            }
        }
    }
	return false;
}

/*
 * vec - the container of the array, zigzagged, a turning point is at an unknown place
 * t - the target key
 * ib - in: lower bound of search; out: index to the first key no less than the searched key or end of array
 * ie - in: upper bound of search; out: index to the first key greater than the searched key or end of array
 *                                 output value of ib may greater than ie showing the rotation
 * valid - testor checking the validity of the key
 *
 * requirements:
 *  ib, ie-1 valid and vec[ib] > vec[ie-1]
 *  t > vec[ib] or t < vec[ie-1]
 *
 * I
 *           ...
 *        ..m
 *     ...         ...
 *              ...
 *
 * II
 *
 *        ...
 *     ...         ....
 *             .m..
 *          ...
 *
 *
 */
template <class Traits>
bool searchzigzag_dimidiate(typename Traits::VecRef vec, typename Traits::EntryRef t, 
    typename Traits::Idx &ib, typename Traits::Idx &ie, typename Traits::CompRef comp, 
    typename Traits::ValidRef valid)
{
	// check the style
	typedef typename Traits::Idx  Idx;
	Idx k;
	Idx m = (ib+ie) / 2;

	// m is different from ib and ie - 1
	// ie < ib
    bool only2 = false;
	if (!valid(vec, m))
	{
		k = m;
		step_left_until< StepInvalidTraits<Traits> >(vec, k, ib, valid);
		if (k == ib)
		{
			step_right_until< StepInvalidTraits<Traits> >(vec, m, ie, valid);
			if (m == ie - 1)
			{	
                only2 = true;
			}
			else
			{	// right side can do
			}
		}
		else
		{	// left side can do
			m = k;
		}
	}
    if (ib + 2 == ie)
    {
        only2 = true;
    }
    if (only2)
    {
        // only ib and ie-1 are valid, it's now at the turning point
        if (comp(t, vec[ib]) == 0)
        {
            --ie;
            return true;
        }
        else if (comp(t, vec[ie-1]) == 0)
        {
            ib = ie - 1;
            return true;
        }
        else    /* t > vec[ib] || t < vec[ie] */
        {
            --ie;
            ib = ie;
            return false;
        }
    }

	if (comp(t, vec[ib]) == 0)
	{
		k = ib;
		skip_equal_right<Traits>(vec, k, ie, valid);
		ie = k;
		return true;
	}
	if (comp(t, vec[ie-1]) == 0)
	{
		k = ie - 1;
		step_to_leftmost_equal<Traits>(vec, k, ib, valid);
		ib = k;
		return true;
	}

	// m is different from both ib and ie-1
    if (comp(t, vec[m]) == 0)
    {
        Idx kl = m;
        Idx kr = m;
        step_to_leftmost_equal<Traits>(vec, kl, ib, valid);
        skip_equal_right<Traits>(vec, kr, ie, valid);
        ib = kl;
        ie = kr;
        return true;
    }


	if (comp(vec[ib], vec[m]) <= 0)	// must have vec[ie-1] < vec[m]
	{	// case I

		if (comp(t, vec[m]) < 0 && comp(t, vec[ib]) > 0)
		{	// left
            ie = m;
            return search_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
		}
		else
		{	// right
			ib = m;	// notice [m+1, ie) can not ensure fulfilment of the presumption of zigzag search
			return searchzigzag_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
		}
	}
	else // vec[m] <= vec[ie-1])	// must have vec[m] < vec[ib]
	{	// case II

		if (comp(t, vec[m]) > 0 && comp(t, vec[ie-1]) < 0)
		{	// right
            ib = m;
            return search_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
        }
        else
        {   // left
            ie = m + 1; // [ib, m+1) goes with the presumption
            return searchzigzag_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
        }
	}
}

/*
 * vec[ib] == vec[ie-1]
 */
template <class Traits>
bool searchcircular_split(typename Traits::VecRef vec, typename Traits::Idx ib, 
    typename Traits::Idx ie, typename Traits::Idx &m, typename Traits::CompRef comp, 
    typename Traits::ValidRef valid)
{
    typedef typename Traits::Idx    Idx;
    Idx tm;
    bool rc;
    m = (ib + ie) / 2;
    if (!valid(vec, m))
    {
        tm = m;

        step_left_until< StepInvalidTraits<Traits> >(vec, tm, ib, valid);
		if (comp(vec[tm], vec[ib]) != 0)
        {
            m = tm;
            return true;
        }
        if (tm > ib + 1)
        {
            rc = searchcircular_split<Traits>(vec, ib, tm, tm, comp, valid);
            if (rc) { m = tm; return true; }
        }

        tm = m;
        step_right_until< StepInvalidTraits<Traits> >(vec, tm, ie, valid);
        if (comp(vec[tm], vec[ib]) != 0)
        {
            m = tm;
            return true;
        }
        if (tm < ie - 1)
        {
            rc = searchcircular_split<Traits>(vec, tm, ie, tm, comp, valid);
            if (rc) { m = tm; return true; }
        }
    }
    else
    {
        if (comp(vec[m], vec[ib]) != 0) { return true; }
        if (m > ib + 1)
        {
            rc = searchcircular_split<Traits>(vec, ib, m, tm, comp, valid);
            if (rc) { m = tm; return true; }
        }
        if (m < ie - 1)
        {
            rc = searchcircular_split<Traits>(vec, m, ie, tm, comp, valid);
            if (rc) { m = tm; return true; }
        }
    }
    return false;

}

/* when there is no valid element, ib and ie points to the cell next the last */
template <class Traits>
bool searchcircular_dimidiate(typename Traits::VecRef vec, typename Traits::EntryRef t, 
    typename Traits::Idx &ib, typename Traits::Idx &ie, typename Traits::CompRef comp, 
    typename Traits::ValidRef valid)
{
    typedef typename Traits::Idx    Idx;
    Idx iie = ie-1;
    Idx oib, oie;

    if (!valid(vec, ib))
    {
        step_right_until< StepInvalidTraits<Traits> >(vec, ib, ie, valid);
		if (ib == ie) { return false; }
    }
    if (!valid(vec, iie))
    {
        step_left_until< StepInvalidTraits<Traits> >(vec, iie, ib, valid);
    }


    if (ib == iie)
    {   // only one valid element
        ie = ib;    // circular
        if (comp(t, vec[ib]) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if (comp(vec[ib], vec[iie]) < 0)
    {   // nonzigzag
        oib = ib;
        oie = ie;
        bool rc = search_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
        if (ie == oie)
        {
            ie = oib;
            if (ib == oie)
            {
                ib = ie;
            }
        }
        return rc;
    }


    // ib first valid
    // iie last valid
    // they are different in index
    Idx neL = ib;
    Idx eR = iie;
    ie = iie+1;
    if (comp(vec[ib], vec[iie]) == 0)
    {
        Idx sp;
        bool allEq = !searchcircular_split<Traits>(vec, ib, ie, sp, comp, valid);
        if (allEq)
        {
            ie = ib;
            return (comp(t, vec[ib]) == 0);
        }
        else
        {
            skip_equal_right<Traits>(vec, neL, sp, valid);
            step_to_leftmost_equal<Traits>(vec, eR, sp, valid);

            if (comp(t, vec[eR]) == 0)
            {   
                ib = eR;
                ie = neL;
                return true;
            }
            if (comp(t, vec[neL]) == 0)
            {
                ib = ie = neL;
                skip_equal_right<Traits>(vec, ie, eR, valid);
                return true;
            }
            
            if (comp(vec[neL], vec[eR]) > 0)
            {
                if (comp(t, vec[neL]) < 0 && comp(t, vec[eR]) > 0)
                {
                    ib = ie = neL;
                    return false;
                }
                ib = neL;
                ie = eR + 1;
            }
            else    // vec[neL] < vec[eR]
            {   // vec[eR] is the biggest
                if (comp(t, vec[eR]) > 0)
                {
                    ib = ie = neL;
                    return false;
                }
                ib = neL;
                ie = eR;
                return search_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
            }
        }
    }
    else    // vec[ib] > vec[iie]
    {
        if (comp(t, vec[ib]) == 0)
        {
            skip_equal_right<Traits>(vec, neL, ie, valid);
            ie = neL;
            return true;
        }
        if (comp(t, vec[iie]) == 0)
        {
            step_to_leftmost_equal<Traits>(vec, eR, ib, valid);
            ie = ib;
            ib = eR;
            return true;
        }
        if (comp(t, vec[ib]) < 0 && comp(t, vec[iie]) > 0)
        {
            ie = ib;
            return false;
        }
    }

    // ib first valid, (ie-1) last valid
    // t > vec[ib] || t < vec[iie]
    return searchzigzag_dimidiate<Traits>(vec, t, ib, ie, comp, valid);
}


template <class Traits>
typename Traits::Idx searchzigzagmin_dimidiate(typename Traits::VecRef vec, 
	typename Traits::Idx ib, typename Traits::Idx ie, typename Traits::CompRef comp, typename Traits::ValidRef valid)
{
	// check the style
	typedef typename Traits::Idx  Idx;
	Idx k;
	Idx m = (ib+ie) / 2;

	// m is different from ib and ie - 1
	// ie < ib
    bool only2 = false;
	if (!valid(vec, m))
	{
		k = m;
		step_left_until< StepInvalidTraits<Traits> >(vec, k, ib, valid);
		if (k == ib)
		{
			step_right_until< StepInvalidTraits<Traits> >(vec, m, ie, valid);
			if (m == ie - 1)
			{	
                only2 = true;
			}
			else
			{	// right side can do
			}
		}
		else
		{	// left side can do
			m = k;
		}
	}
    if (ib + 2 == ie)
    {
        only2 = true;
    }
    if (only2)
    {
        // only ib and ie-1 are valid, it's now at the turning point
		return ie - 1;
    }

	if (vec[ib] <= vec[m])	// must have vec[ie-1] < vec[m]
	{	// case I
		return searchzigzagmin_dimidiate<Traits>(vec, m, ie, comp, valid);
	}
	else // vec[m] <= vec[ie-1])	// must have vec[m] < vec[ib]
	{	// case II
		return searchzigzagmin_dimidiate<Traits>(vec, ib, m + 1, comp, valid);
	}
}

/*
 * search for the min element of the circular array
 * when there is no valid element, return the index to the cell next to the last
 */
template <class Traits>
typename Traits::Idx searchcircmin_dimidiate(typename Traits::VecRef vec, 
    typename Traits::Idx ib, typename Traits::Idx ie, 
    typename Traits::CompRef comp, typename Traits::ValidRef valid)
{
    typedef typename Traits::Idx    Idx;
    Idx iie = ie-1;
    Idx oib, oie;

    if (!valid(vec, ib))
    {
        step_right_until< StepInvalidTraits<Traits> >(vec, ib, ie, valid);
		if (ib == ie) 
		{	// no valid elements
			return ie; 
		}
    }
    if (!valid(vec, iie))
    {
        step_left_until< StepInvalidTraits<Traits> >(vec, iie, ib, valid);
    }


    if (ib == iie)
    {   // only one valid element
		return ib;
    }

    if (comp(vec[ib], vec[iie]) < 0)
    {   // nonzigzag
		return ib;
    }


    // ib first valid
    // iie last valid
    // they are different in index
    Idx neL = ib;
    Idx eR = iie;
    ie = iie+1;

    if (vec[ib] == vec[iie])
    {
        Idx sp;
        bool allEq = !searchcircular_split<Traits>(vec, ib, ie, sp, comp, valid);

        if (allEq)
        {
			return ib;
        }
        else
        {
            skip_equal_right<Traits>(vec, neL, sp, valid);
            step_to_leftmost_equal<Traits>(vec, eR, sp, valid);

            if (comp(vec[neL], vec[eR]) > 0)
            {
				ib = neL;
				ie = eR + 1;
				return searchzigzagmin_dimidiate<Traits>(vec, ib, ie, comp, valid);
            }
            else    // vec[neL] < vec[eR]
            {   // vec[eR] is the highest, vec[neL] is lowest
				return neL;
            }
        }
    }
    else    // vec[ib] > vec[iie]
    {
		return searchzigzagmin_dimidiate<Traits>(vec, ib, ie, comp, valid);
    }
}


_QTL_END


#if defined(TEST_search_H) && TEST_search_H
/* ---------------------------------------------------------------------- */
/* Test Script */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>


int GenerateTestcase(int *arr, int n, int *zigarr, bool &inc)
{
    int iTurning = 0;
    int i;
    int v = 1;
    int minv=10000, maxv=0;
    int invalid = 0;

    inc = false;
    iTurning = rand()*n/RAND_MAX;

    for (i = 0; i < n; i++)
    {
        int r = rand();

        if (invalid)
        {
            arr[i]=0;
        }
        else
        {
            arr[i] = v;
            if (v<minv)minv=v;
            if (v>maxv)maxv=v;
        }
        int j = (i + iTurning) % n;
        zigarr[j] = arr[i];

        if (r%5<2)
        {
            // change validity
            if (!invalid)
            {
                r = rand();
                if (r%3==0)
                {
                    invalid = 1-invalid;
                }
            }
            else
            {
                invalid = 1-invalid;
            }
        }
        else if (r%5==2)
        {
            v = v;
        }
        else
        {   // increase
            v+=2;
        }
    }
    inc = (minv<maxv);

	if (inc)
	{
		for (i = 0 ; i < n && arr[i]==0; ++i)
		{
		}
		iTurning += i;
		iTurning %= n;
	}
	else
	{
		for (i = 0 ; i < n && zigarr[i]==0; ++i)
		{
		}
		iTurning = i;	// including empty case in which p == n
	}
    return iTurning;
}

void ElemSelect(int *arr, int n, int *pv, int *pb, int *pe)
/*
 * pb -- the first valid element that is no less than v, or end of array
 * pe -- the first valid element that is greater than v, or end of array
 */
{
    int r;
    int v;
    int i;
	do
	{
		r = rand();
		r %= n;
		i = r;
		v = arr[i];
	} while (v==0);

    r = rand();
    if (r%3==0)
    {
        if (r%2==0 && v > 1)
        {   //less
__less:
            for ( ; i >= 0 && (arr[i]==v||arr[i]==0); i--);
            i++;
            for ( ; arr[i]==0; i++);

            *pb = *pe = i;
            *pv = v-1;  
        }
        else
        {   //greater
            for ( ; i < n && (arr[i]==v||arr[i]==0); i++);
            if (i == n)
            {
                v += 2;
                *pb = *pe = n;
                *pv = v;
            }
            else
            {
                v = arr[i];
                goto __less;
            }
        }
    }
    else
    {   // get the exact element
        int j = i;
        for ( ; i >= 0 && (arr[i]==v||arr[i]==0); i--);
        i++;
        for ( ; arr[i]==0; i++);
        *pb = i;
        for ( ; j < n && (arr[j]==v||arr[j]==0); j++);
        *pe = j;
        *pv = v;
    }
}

time_t Randomize(void)
{
    time_t seed;
    time(&seed);
	//seed = 1214590794;
	//seed = 1214592164;
	seed = 1214836304;
    srand(seed);
	printf("seed = %d\n", seed);
	return seed;
}

using namespace qtl;
struct SearchTraits
{
    typedef int     Idx;
    typedef int *   VecRef;
    typedef int     EntryRef;
    
    struct Valid
    {
        bool operator()(VecRef vec, const Idx &k)
        {
            return vec[k] != 0;
        }

		void step_left_until(VecRef vec, Idx &k, Idx ib)
		{
			for ( ; k >= ib && !vec[k] ; --k);
		}
		
		void step_right_until(VecRef vec, Idx &k, Idx ie)
		{
			for ( ; k < ie && !vec[k] ; ++k);
		}
    };

    struct Comp
    {
        int operator()(EntryRef a, EntryRef b)
        {
            return a==b?0:a<b?-1:1;
        }
    };

    typedef Valid	ValidRef;
    typedef Comp    CompRef;
};

int main(void)
{
    const int kCount = 1000000;
    const int kFluc = 16;
    const int kMinN = 16;
    int arr[kMinN + kFluc];
    int zigarr[kMinN + kFluc];
    int iT = 0;

	time_t seed = Randomize();

    for (iT = 0; iT < kCount; iT++)
    {
        int j;
        int n = rand();
        n %= kFluc;
        n += kMinN;
        bool inc = false;
        int p = GenerateTestcase(arr, n, zigarr, inc);

		int v, b, e;
		ElemSelect(arr,n,&v,&b,&e);
        int zigb = (b+p)%n;
        int zige = (e+p)%n;

        if (!inc)
        {
            for (j = 0; j < n && zigarr[j]==0; j++);
            j = j%n;
            zigb=zige = j;
        }

	
		if (0)
		{	// test turning point detection
            SearchTraits::Comp  comp;
			SearchTraits::Valid valid;
			int ib = 0, ie = n;
			int itp = searchcircmin_dimidiate<SearchTraits>(zigarr, ib, ie, comp, valid);

			if (itp != p)
			{
				printf(": Test %d failed! tp = %d, tpref = %d\n", iT, itp, p);
                printf(" %d - ", n);
                for (j = 0; j < n; j++)
                {
                    printf("%d ", zigarr[j]);
                }
                printf("\n");
				break;
			}
			else
			{
                printf("%d\n", iT);
			}

		}
		else
		{	// test zigzag/nonzigzag searching
            SearchTraits::Comp  comp;
			SearchTraits::Valid valid;
			int ib = 0, ie = n;
			bool rc = searchcircular_dimidiate<SearchTraits>(zigarr, v, ib, ie, comp, valid);
			if (ib == zigb && ie == zige)
			{
				//printf(": Test passed.\n");
                printf("%d\n", iT);
			}
			else
			{
				printf(": Test %d failed! search %d, output:[%d,%d), ref:[%d,%d)\n", iT, v, ib, ie, zigb, zige);
                printf(" %d - ", n);
                for (j = 0; j < n; j++)
                {
                    printf("%d ", zigarr[j]);
                }
                printf("\n");

				break;
			}
		}
		//if (getch()=='q') break;
    }
	printf("exit with seed %d\n", seed);
    return 0;
}

#endif  /* if defined(TEST_search_H) && TEST_search_H */

#endif  /* ifndef _search_H */
