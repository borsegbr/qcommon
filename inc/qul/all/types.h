/**
 * === QUL Common Files ===
 *  Unit Name:  types
 *  Author:     Lincoln Yu
 *  Purpose:    Type definition
 * == History ==        
 *  2005-11-20: Created.
 * == Comments ==
 *
 */

#ifndef _QUL_types_H
#define _QUL_types_H

#include "predef.h"

_QUL_BEGIN


typedef unsigned char	UInt8;
typedef unsigned short	UInt16;
typedef unsigned long	UInt32;

typedef signed char		SInt8;
typedef signed short	SInt16;
typedef signed long		SInt32;


#ifndef NULL
	#define NULL		0
#endif

_QUL_END

#endif	// ifndef _QUL_types_H

