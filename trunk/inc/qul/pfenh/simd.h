/**
* === QUL ===
*  Unit Name:  simd
*  Author:     Lincoln Yu
*  Purpose:    SIMD utilities
* == History ==        
*  2006-01-19: Created.
* == Comments ==
*
*/

#ifndef _simd_H
#define _simd_H

#include "../all/prefix.h"

#if defined(_TARGET_CPU_IA32) && defined(_TARGET_OS_WIN32)

#include "../all/predef.h"

#include <xmmintrin.h>


#ifdef __cplusplus
_QUL_BEGIN
#endif

typedef union _SSE128
	{
	float	vals[4];
	__m128	m;
	} SSE128;

#ifdef __cplusplus
_QUL_END
#endif

#endif	// if defined(_TARGET_CPU_IA32) && defined(_TARGET_OS_WIN32)

#endif	// ifndef _simd_H