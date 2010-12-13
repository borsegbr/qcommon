/*
 * === Stream Utility ===
 *	Unit Name:	base64 
 *	Author:		quanben
 *	Purpose:	Base64 converter
 * == History ==
 *  2007-11-09: Created, with encoder and corresponding decoder implemented.
 *  2008-04-18: Modified, iterator-style generic methods, string type supported.
 *  2008-04-24: *dst++ = *dst++ = .. not my intent...
 *
 */


#ifndef _base64_H
#define _base64_H

#include "../all/predef.h"

#include <string>


_QUL_BEGIN

class Base64Encoder
{
public:
	typedef	std::string	String;

public:
    int GetEncodedLen (int len) const
    {
        return ((len + 2) / 3) * 4;
    }

	template <class OutCharItr, class InCharItr>
	void Encode (OutCharItr dst, InCharItr src, InCharItr srcEnd) const
	{
		unsigned char src0, src1, src2;
        static char     *encMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		for ( ; src != srcEnd; )
		{
			src0 = *src++;
			if (src == srcEnd)
			{
				*dst++ = encMap[src0 >> 2];
				*dst++ = encMap[(src0 << 4) & 0x30];
				*dst++ = '=';
				*dst++ = '=';
				return;
			}
			src1 = *src++;
			if (src == srcEnd)
			{
				*dst++ = encMap[src0 >> 2];
				*dst++ = encMap[((src0 << 4) & 0x30) | (src1 >> 4)];
				*dst++ = encMap[(src1 << 2) & 0x3c];
				*dst++ = '=';
				return;
			}

			src2 = *src++;
			*dst++ = encMap[src0 >> 2];
			*dst++ = encMap[((src0 << 4) & 0x30) | (src1 >> 4)];
            *dst++ = encMap[((src1 << 2) & 0x3c) | (src2 >> 6)];
			*dst++ = encMap[src2 & 0x3f];
		}
	}

	void Encode (char *dst, const unsigned char *src, int len) const
	{
		Encode<char*,const unsigned char*>(dst, src, src+len);
	}

	void EncodeStr (String &out, const String &in) const
	{
		int lenIn = in.size();
		int lenOut = GetEncodedLen(lenIn);
		out.resize(lenOut);
		Encode(out.begin(), in.begin(), in.end());
	}
};

class Base64Decoder
{
public:
	typedef	std::string	String;

public:
    int GetBufferLen (int len) const
    {
        if (len % 4 != 0)
        {
            return -1;
        }
        return len / 4 * 3;
    }

	template <class OutCharItr, class InCharItr>
	void Decode (OutCharItr &dst, InCharItr src, InCharItr srcEnd) const
	{
		unsigned long temp;
		char srcl1 = *--srcEnd, srcl2, srcl3, srcl4;
		if (srcl1 == '=')
		{
			srcl2 = *--srcEnd;
			srcl3 = *--srcEnd;
			srcl4 = *--srcEnd;
		}
		else
		{
			++srcEnd;
		}

        for ( ; src != srcEnd; )
        {
            temp = InvMap(*src++);
            temp <<= 6;
            temp |= InvMap(*src++);
            temp <<= 6;
            temp |= InvMap(*src++);
            temp <<= 6;
            temp |= InvMap(*src++);

			*dst++ = (temp >> 16) & 0xff;
			*dst++ = (temp >> 8) & 0xff;
			*dst++ = temp & 0xff;
        }

		if (srcl1 == '=')
		{
			if (srcl2 == '=')
			{
                temp = InvMap(srcl4);
                temp <<= 6;
                temp |= InvMap(srcl3);
                temp >>= 4;
                *dst++ = temp;
			}
			else
			{
                temp = InvMap(srcl4);
                temp <<= 6;
                temp |= InvMap(srcl3);
                temp <<= 6;
                temp |= InvMap(srcl2);

				*dst++ = temp >> 10;
				*dst++ = (temp >> 2) & 0xff;
			}
		}
	}

    int Decode (unsigned char *dst, const char *src, int len) const
    {
		unsigned char *pdst = dst;
		if (len % 4 != 0)
		{
			len -= len % 4;
		}
		Decode<unsigned char *, const char *>(pdst, src, src+len);
		return (pdst - dst);
    }

	void DecodeStr (String &out, const String &in) const
	{
		int lenIn = in.size();
		int lenOutX = GetBufferLen(lenIn);
		out.resize(lenOutX);

		String::iterator itrOut = out.begin();
		Decode(itrOut, in.begin(), in.end());
		out.resize(itrOut - out.begin());
	}

protected:
    unsigned char InvMap (char b64) const
    {
        if (b64 >= 'A' && b64 <= 'Z')
        {
            return b64 - 'A';
        }
        else if (b64 >= 'a' && b64 <= 'z')
        {
            return b64 - 'a' + 26;
        }
        else if (b64 >= '0' && b64 <= '9')
        {
            return b64 - '0' + 52;
        }
        else if (b64 == '+')
        {
            return 62;
        }
        else    /* b64 == '/' */
        {
            return 63;
        }
    }
};

_QUL_END

#endif  /* ifndef _base64_H */

