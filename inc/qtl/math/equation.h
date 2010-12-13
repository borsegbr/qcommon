/**
 * === Math Library ===
 *  Unit Name:  equation
 *  Author:     Lincoln Yu
 *  Purpose:    Algorithms dealing with common equations
 * == History ==        
 *  2005-11-03: Created.
 * == Comments ==
 *
 */

#ifndef _QTL_equation_H
#define _QTL_equation_H

#include "../all/predef.h"

_QTL_BEGIN

template <typename _CoefTp, typename _RootTp, typename _Sqrt> 
        inline int solve_quadratic (const _CoefTp &__a, const _CoefTp &__b, const _CoefTp &__c, 
        _RootTp &__r1, _RootTp &__r2, _Sqrt __sqrt)
        /*
         *  Solve equation in form of __a * x^2 + __b * x + __c = 0
         */
    {
    _CoefTp __tmp1 = __b;   __tmp1 *= __b;
    _CoefTp __tmp2 = __a;   __tmp2 *= __c;  __tmp2 *= 4;
    __tmp1 -= __tmp2;
    _RootTp __delta = __tmp1;
    if(__delta < 0) return 0;       // delta is less than 0
    __delta = __sqrt(__delta);
    __tmp1 = -__b;
    __tmp2 = __a;   __tmp2 *= 2;
    __r2 = __r1 = -__b; 
    __r1 -= __delta; __r1 /= __tmp2;
    __r2 += __delta; __r2 /= __tmp2;
    return (__delta == 0)? 1 : 2;
    }

_QTL_END

#endif  // ifndef _QTL_equation_H
