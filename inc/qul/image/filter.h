/**
 * === Image Utility ===
 *	Unit Name:	filter
 *	Author:		quanben
 *	Purpose:	Base class of filters
 * == History ==
 *  2006-01-18:	Created
 *
 */

#ifndef _filter_H
#define _filter_H

#include "../all/predef.h"
#include "../all/types.h"

_QUL_BEGIN

template <class _SampleType = UInt8>
	struct TypicalFilterIO
	{
	typedef _SampleType	SampleType;
	
	SampleType	*pStream;
	UInt32		size;

	TypicalFilterIO(SampleType *pStrm = NULL, UInt32 sizeStrm = 0) 
		: pStream(pStrm), size(sizeStrm)
		{
		}

	};

template <class _Input = TypicalFilterIO<UInt8> &, class _Output = _Input, class _ResType = UInt32>
	class BaseStrmFilter
	{
public:
	typedef _Input		Input;
	typedef _Output		Output;
	typedef _ResType	ResType;

	virtual ResType	Transform (Input input, Output output) = 0;
	virtual ResType ITransform (Input input, Output output) { return _ResType(); }

	};	// class BaseTransformer

_QUL_END

#endif	// ifndef _filter_H
