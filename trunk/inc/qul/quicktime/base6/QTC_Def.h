/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_Def
 *  Author:     Lincoln Yu
 *  Purpose:    Local QuickTime component definition
 * == History ==        
 *	2006-01-25:	Created.
 *
 */

#ifndef _QTC_Def_H
#define _QTC_Def_H

#include <qul/all/predef.h>
#include <qul/all/prefix.h>


#if defined(_TARGET_OS_WIN32)
	#include <QuickTimeComponents.h>
	#include <FixMath.h>
	#include <mediahandlers.h>
	#include <ImageCodec.h>
#elif defined(_TARGET_OS_MACH)
	#include <QuickTime/QuickTime.h>
#endif

typedef ComponentResult	QTC_Result;

#define _QTC_BEGIN	namespace qtc {
#define _QTC_END	}

#ifdef __cplusplus

_QTC_BEGIN

typedef ComponentResult	Result;

_QTC_END

#endif

#endif	// ifndef _QTC_Def_H

