/**
 * === QUL Common Files ===
 *  Unit Name:  endians
 *  Author:     Lincoln Yu
 *  Purpose:    Endian convesioin
 * == History ==        
 *  2005-11-20: Created.
 *  2006-03-06: Make it suitable for MAC.
 *  2007-01-03: Add macro IS_BIG_ENDIAN.
 * == Comments ==
 *
 */

#ifndef _QUL_endians_H
#define _QUL_endians_H

#include "prefix.h"
#include "types.h"
#include "predef.h"

#define EndianU16_Swap(u16)		((u16>>8)|(u16<<8))
#define EndianU32_Swap(u32)		((u32>>24)|(u32>>8)&0xff00|(u32<<8)&0xff0000|(u32<<24))



#ifdef _TARGET_CPU_POWERPC

	#define IS_BIG_ENDIAN			1

	/* Use the framework */
	#include <QuickTime/QuickTimeComponents.k.h>


#else

	#define IS_BIG_ENDIAN			0

	#if defined(_TARGET_CPU_IA32) && !defined(__ENDIAN__)

		#define EndianU16_BtoN(u16)		EndianU16_Swap(u16)
		#define EndianU16_LtoN(u16)		(u16)
		#define EndianU16_NtoB(u16)		EndianU16_Swap(u16)
		#define EndianU16_NtoL(u16)		(u16)


		#define EndianU32_BtoN(u32)		EndianU32_Swap(u32)
		#define EndianU32_LtoN(u32)		(u32)
		#define EndianU32_NtoB(u32)		EndianU32_Swap(u32)
		#define EndianU32_NtoL(u32)		(u32)

	#else

	#endif	// if defined(_TARGET_CPU_IA32) && !defined(__ENDIAN__)

#endif	// ifdef _TARGET_OS_MACH


#ifdef __cplusplus

_QUL_BEGIN

struct LE16
	{
	UInt16	dump;
	LE16() : dump(0) {}
	LE16(UInt16 val)
		: dump(EndianU16_NtoL(val))
		{
		}
	LE16 & operator= (UInt16 val)
		{
		dump = EndianU16_NtoL(val);
		return *this;
		}
	operator UInt16() const
		{
		return EndianU16_LtoN(dump);
		}
	};	// struct LE16

struct BE16
	{
	UInt16	dump;
	BE16() : dump(0) {}
	BE16(UInt16 val)
		: dump(EndianU16_NtoB(val))
		{
		}
	BE16 & operator= (UInt16 val)
		{
		dump = EndianU16_NtoB(val);
		return *this;
		}
	operator UInt16() const
		{
		return EndianU16_BtoN(dump);
		}
	};	// struct BE16

struct LE32
	{
	UInt32	dump;
	LE32() : dump(0) {}
	LE32(UInt32 val)
		:dump(EndianU32_NtoL(val))
		{
		}
	LE32 & operator= (UInt32 val)
		{
		dump = EndianU32_NtoL(val);
		return *this;
		}
	operator UInt32() const
		{
		return EndianU32_LtoN(dump);
		}
	};	// struct LE32

struct BE32
	{
	UInt32	dump;
	BE32() : dump(0) {}
	BE32(UInt32 val)
		:dump(EndianU32_NtoB(val))
		{
		}
	BE32 & operator= (UInt32 val)
		{
		dump = EndianU32_NtoB(val);
		return *this;
		}
	operator UInt32() const
		{
		return EndianU32_BtoN(dump);
		}
	};

_QUL_END

#endif	// ifdef __cplusplus

#endif	// ifndef _QUL_endians_H
