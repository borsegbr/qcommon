/*
* === Image Utility ===
*	Unit Name:	canvas 
*	Author:		quanben
*	Purpose:	Pixel canvas.
* == History ==
*  2005-12-12:	Created.
*
*/

#ifndef _QUL_canvas_H
#define _QUL_canvas_H

#include "../all/predef.h"
#include "../all/types.h"

_QUL_BEGIN

enum PixelFormat
	{
	PF_BGRA,
	PF_RGBA,
	PF_ARGB,
	PF_ABGR,
	PF_RGB,
	PF_BGR
	};

class PixelRef
	{
	public:

	PixelRef(PixelFormat pixelFormat) : m_pPixel(NULL)
		{
		SetPixelFormat(pixelFormat);
		}

	void SetPixelFormat(PixelFormat pixelFormat)
		{
		switch(pixelFormat)
			{
			case PF_BGRA:
				m_IdxBlue	= 0;
				m_IdxGreen	= 1;
				m_IdxRed	= 2;
				m_IdxAlpha	= 3;
				break;
			case PF_RGBA:
				m_IdxRed	= 0;
				m_IdxGreen	= 1;
				m_IdxBlue	= 2;
				m_IdxAlpha	= 3;
				break;
			case PF_ARGB:
				m_IdxAlpha	= 0;
				m_IdxRed	= 1;
				m_IdxGreen	= 2;
				m_IdxBlue	= 3;
				break;
			case PF_ABGR:
				m_IdxAlpha	= 0;
				m_IdxBlue	= 1;
				m_IdxGreen	= 2;
				m_IdxRed	= 3;
				break;
			case PF_RGB:
				m_IdxAlpha	= 0;
				m_IdxRed	= 0;
				m_IdxGreen	= 1;
				m_IdxBlue	= 2;
				break;
			case PF_BGR:
				m_IdxAlpha	= 0;
				m_IdxBlue	= 0;
				m_IdxGreen	= 1;
				m_IdxRed	= 2;
				break;
			}
		}

	void Bind(UInt8 *p)
		{
		m_pPixel = p;
		}

	UInt8 GetRed () const
		{
		return m_pPixel[m_IdxRed];
		}

	UInt8 GetGreen () const
		{
		return m_pPixel[m_IdxGreen];
		}

	UInt8 GetBlue () const
		{
		return m_pPixel[m_IdxBlue];
		}

	UInt8 GetAlpha () const
		{
		return m_pPixel[m_IdxAlpha];
		}

	void SetRed (UInt8 red)
		{
		m_pPixel[m_IdxRed] = red;
		}

	void SetGreen (UInt8 green)
		{
		m_pPixel[m_IdxGreen] = green;
		}

	void SetBlue (UInt8 blue)
		{
		m_pPixel[m_IdxBlue] = blue;
		}

	void SetAlpha (UInt8 alpha)
		{
		m_pPixel[m_IdxAlpha] = alpha;
		}

	private:
		UInt8	m_IdxRed;
		UInt8	m_IdxGreen;
		UInt8	m_IdxBlue;
		UInt8	m_IdxAlpha;

		UInt8	*m_pPixel;

	};	// struct PixelRef

template <class _PixelRef = PixelRef>	
	class RowRef
	{
	public:
		RowRef(UInt16 bytesPerPixel, PixelFormat pixelFormat) 
			: m_BytesPerPixel(bytesPerPixel), m_PixelRef(pixelFormat), m_pRow(NULL)
			{
			}

		void Bind(UInt8 *pRow)
			{
			m_pRow = pRow;
			}

		_PixelRef operator[] (UInt32 idxCol) const
			{
			_PixelRef temp = m_PixelRef;
			temp.Bind(m_pRow + idxCol * m_BytesPerPixel);
			return temp;
			}

	private:
		UInt8		*m_pRow;
		UInt16		m_BytesPerPixel;
		_PixelRef	m_PixelRef;
	};	// class RowRef

template < class _PixelRef = PixelRef, class _RowRef = RowRef<_PixelRef> >
	class CanvasBase
	{
public:
	CanvasBase (UInt32 widthInByte, UInt16 bytesPerPixel = 4, PixelFormat pixelFormat = PF_BGRA, UInt8 *pData = NULL) 
		: m_WidthInByte(widthInByte), m_pData(pData), m_RowRef(bytesPerPixel, pixelFormat)
		{
		}

	virtual ~CanvasBase ()
		{
		}

	_RowRef operator[] (UInt32 idxRow) const
		{
		_RowRef temp = m_RowRef;
		temp.Bind(m_pData + idxRow * m_WidthInByte);
		return temp;
		}

protected:
	UInt32		m_WidthInByte;
	UInt8		*m_pData;

	_RowRef		m_RowRef;
	
	};	// class CanvasBase


template < class _PixelRef = PixelRef, class _RowRef = RowRef<_PixelRef> >
	class ImageRef : public CanvasBase<_PixelRef, _RowRef>
	{
	typedef CanvasBase<_PixelRef, _RowRef>	_Mybase;

public:
	ImageRef (UInt32 widthInByte, UInt16 bytesPerPixel = 4, PixelFormat pixelFormat = PF_BGRA, UInt8 *pData = NULL) 
		: _Mybase(widthInByte, bytesPerPixel, pixelFormat, pData)
		{		
		}

	virtual void Bind(UInt8 *pData)
		{
		m_pData = pData;
		}

	};

_QUL_END

#endif	// ifndef _QUL_canvas_H
