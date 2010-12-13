/**
 * === Numeric Library ===
 *  Unit Name:  factors
 *  Author:     Lincoln Yu
 *  Purpose:    Algorithms dealing with factor resolving
 * == History ==        
 *  2005-10-31: Created.
 *  2005-11-02: Debugged on Mac OS X (Tiger).
 *  2005-11-05: [:vector_pushbacker:] added.
 * == Comments ==
 *
 */

#ifndef _QTL_factors_H
#define _QTL_factors_H   

#include <iostream>
#include <vector>

#include "../all/predef.h"

_QTL_BEGIN

template <typename _Tp>
	struct vector_pushbacker
	{
    typedef std::vector<_Tp>    _VecTp;
    
	void operator() (_VecTp &v, _Tp x) const
		{
        v.push_back(x);
		}
	};

template <typename _SrcTp, typename _FctTp, typename _FctCtnr, typename _PushBack>
    void resolve_factors (const _SrcTp &__src, const _SrcTp &__limit, _FctCtnr &__vf, _PushBack __push_back)
	/*
		*   __limit is bigger than sqrt(__src)
		*/
	{
    _FctTp __last_factor = 2;
    _SrcTp __q = __src;
    for (; __q > 1; )
		{
        _FctTp __factor = __last_factor;
        bool __found = false;
        for ( ; __factor < __limit && !(__found = (__q % __factor == 0)); ++__factor );

        if (!__found) __factor = __q;
        __push_back(__vf, __factor);

        __q /= __factor;
        __last_factor = __factor;
		}
	}

_QTL_END

#endif  // ifndef _QTL_factors_H
