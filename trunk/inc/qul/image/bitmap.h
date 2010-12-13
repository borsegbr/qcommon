/*
 * === Image Utility ===
 *	Unit Name:	bitmap 
 *	Author:		quanben
 *	Purpose:	To deal with BITMAP files.
 * == History ==
 *  2005-12-09:	Created
 *
 */

#ifndef _QUL_bitmap_H
#define _QUL_bitmap_H

#include <string.h>
#include "../all/endians.h"
#include "../filefmt/filechunk.h"

_QUL_BEGIN

class BitmapHeader : public FileChunk
	{
protected:

	struct BmpFileHdr : public FileChunk
		{
#pragma pack(2)	// alignment
		struct Dump{
			BE16	bfType;
			LE32	bfSize;
			LE16	bfReserved1;
			LE16	bfReserved2;
			LE32	bfOffBits;
			Dump ()
				: bfType('BM'), bfReserved1(0), 
				bfReserved2(0), bfOffBits(sizeof(Dump)+sizeof(BmpImgHdr::Dump))
				{
				}
		}	dump;
#pragma pack()
		
		BmpFileHdr() : dump()
			{
			}

		virtual UInt32 Parse (UInt8 *pBuf)
			{
			memcpy(&dump, pBuf, sizeof(Dump));
			return sizeof(Dump);
			}

		virtual UInt32 Make (UInt8 *pBuf) const
			{
			memcpy(pBuf, &dump, sizeof(Dump));
			return sizeof(Dump);
			}

		};

	struct BmpImgHdr : public FileChunk
		{
		struct Dump
			{
			LE32	biSize;
			LE32	biWidth;
			LE32	biHeight;
			LE16	biPlanes;
			LE16	biBitCount;			// total bit count of all components of the pixel
			LE32	biCompression;		
			LE32	biSizeImage;		// size of the image data
			LE32	biXPelsPerMeter;
			LE32	biYPelsPerMeter;
			LE32	biClrUsed;
			LE32	biClrImportant;
			
			Dump ()
				: biSize(sizeof(Dump)), biPlanes(1), biCompression(0),
				biXPelsPerMeter(0), biYPelsPerMeter(0), biClrUsed(0), biClrImportant(0)
				{
				}

			Dump (UInt32 width, UInt32 height, UInt16 bytesPerPixel)
				: biSize(sizeof(Dump)), biPlanes(1), biCompression(0),
				biWidth(width), biHeight(height), biBitCount(bytesPerPixel << 3),
				biSizeImage(width * height * bytesPerPixel), biClrUsed(0), 
				biXPelsPerMeter(0), biYPelsPerMeter(0), biClrImportant(0)
				{
				}
			}	dump;

		BmpImgHdr() : dump()
			{
			}

		BmpImgHdr(UInt32 width, UInt32 height, UInt16 bytesPerPixel) 
			: dump(width, height, bytesPerPixel)
			{
			}

		virtual UInt32 Parse (UInt8 *pBuf)
			{
			memcpy(&dump, pBuf, sizeof(Dump));
			return dump.biSize;
			}

		virtual UInt32 Make (UInt8 *pBuf) const
			{
			memcpy(pBuf, &dump, sizeof(Dump));
			return sizeof(Dump);
			}

		};	// struct BmpFileHdr

public:

	enum
		{
		k_BmpHdrBaseSize = sizeof(BmpFileHdr::Dump) + sizeof(BmpImgHdr::Dump),
		};

	BitmapHeader ()	// do nothing, later assignment expected
		{
		}

	BitmapHeader (UInt32 width, UInt32 height, UInt16 bytesPerPixel)
		: m_ImgHdr(width, height, bytesPerPixel)
		{
		UInt32 fileSize = m_FileHdr.dump.bfOffBits;
		fileSize += (UInt32) m_ImgHdr.dump.biSizeImage;
		m_FileHdr.dump.bfSize = fileSize;
		}

	virtual UInt32 Make (UInt8 *pBuf)
		{
		UInt8 *p = pBuf;
		UInt32 sizeTotal = 0;
		UInt32 size = 0;
		
		size = m_FileHdr.Make(p);
		sizeTotal += size;
		p += size;
		
		size = m_ImgHdr.Make(p);
		sizeTotal += size;
		
		return sizeTotal;
		}

	virtual UInt32 Parse (UInt8 *pBuf)
		{
		UInt8 *p = pBuf;
		UInt32 sizeTotal = 0;
		UInt32 size = 0;
		
		size = m_FileHdr.Parse(p);
		sizeTotal += size;
		p += size;

		size = m_ImgHdr.Parse(p);
		sizeTotal += size;
		
		return sizeTotal;
		}

	UInt32 GetWidth () const
		{
		return (UInt32)m_ImgHdr.dump.biWidth;
		}

	UInt32 GetHeight () const
		{
		return (UInt32)m_ImgHdr.dump.biHeight;
		}

	UInt16 GetBitDepth () const
		{
		return (UInt16)m_ImgHdr.dump.biBitCount;
		}

	UInt32 GetDataOffset () const
		{
		return (UInt32)m_FileHdr.dump.bfOffBits;
		}

	UInt32 GetImageSize () const
		{
		return (UInt32)m_ImgHdr.dump.biSizeImage;
		}

	/*
	 * == Comments ==
	 *  The bitmap file must has a valid File-header section.
	 */
	UInt32 GetRowBytes () const
		{
		UInt32 sizeImage = (UInt32)m_ImgHdr.dump.biSizeImage;
		if ( sizeImage == 0 ) 
			{
			sizeImage = (UInt32)m_FileHdr.dump.bfSize - (UInt32)m_FileHdr.dump.bfOffBits;
			}
		return sizeImage / (UInt32)m_ImgHdr.dump.biHeight;
		}

protected:
	BmpFileHdr	m_FileHdr;
	BmpImgHdr	m_ImgHdr;

	};	// class Bitmap

_QUL_END

#endif	// ifndef _BITMAP_H
