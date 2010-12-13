/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_Util
 *  Author:     Lincoln Yu
 *  Purpose:	QuickTime component utilities
 * == History ==        
 *	2006-02-12:	Created.
 *
 */

#ifndef _QTC_Util_H
#define _QTC_Util_H

#define CHECK_ERR(func)		{ if((err=(func))!=noErr) {goto bail;} }
#define CHECK_DO(func)		{ if((func)!=noErr) {goto bail;} }
#define RAISE(e)			{ err=(e); goto bail; }
#define FAILED(func)		((func)!=noErr)
#define SUCCEEDED(func)		((func)==noErr)


#endif	// ifndef _QTC_Util_H
