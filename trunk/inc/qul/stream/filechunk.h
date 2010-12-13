/*
 * === Stream Utility ===
 *	Unit Name:	filechunk 
 *	Author:		quanben
 *	Purpose:	Base class of file chunk manipulator
 * == History ==
 *  2006-01-29:	Created.
 *	2007-01-03:	virtual function [:bool FileChunk::Validate():] added.
 *
 */

#ifndef _filechunk_H
#define _filechunk_H

#include "../all/types.h"


_QUL_BEGIN

class FileChunk
	{
public:
	virtual UInt32 Parse (UInt8 *pBuf) { return 0; }					// Parse buffer with size unspecified
	virtual UInt32 Parse (UInt8 *pBuf, UInt32 size) { return 0; }		// Parse buffer with size specified by [:size:]

	virtual UInt32 Make (UInt8 *pBuf) { return 0; }						// Create chunk on buffer with size unspecified
	virtual UInt32 Make (UInt8 *pBuf) const { return 0; }				
	virtual UInt32 Make (UInt8 *pBuf, UInt32 size) { return 0; }		// Create chunk on buffer with size specified by [:size:]
	virtual UInt32 Make (UInt8 *pBuf, UInt32 size) const { return 0; }

	virtual bool   Validate () { return true; }							// Validate the data

	};	// class FileChunk


_QUL_END

#endif	// ifndef _filechunk_H
