/**
 * === QUL Common Files ===
 *  Unit Name:  macros
 *  Author:     Lincoln Yu
 *  Purpose:    Common macros
 * == History ==        
 *	2006-02-15: Created.
 * == Comments ==
 *
 */

#ifndef _macros_H
#define _macros_H

#define CONCAT_DIRECT(a,b)  a##b
#define CONCAT(a,b)         CONCAT_DIRECT(a,b)
#define TOSTR_DIRECT(tag)   #tag
#define TOSTR(tag)          TOSTR_DIRECT(tag)

#define MAX(a,b)			(((a)>(b))?(a):(b))
#define MIN(a,b)			(((a)<(b))?(a):(b))

#endif	// ifndef _macros_H