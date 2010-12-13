/*
* === Image Utility ===
*	Unit Name:	colrspc
*	Author:		quanben
*	Purpose:	Color space utilities
* == History ==
*  2006-01-18:	Created
*
*/

#ifndef _colrspc_H
#define _colrspc_H

#include <cmath>

#include "../all/predef.h"
#include "../all/types.h"

#ifdef _USE_SSE
#include <utils/pfenh/simd.h>
#endif

_QUL_BEGIN

template <class _InType, class _OutType>
	struct Colr3Converter
	{
	typedef _InType		InType;
	typedef _OutType	OutType;

	virtual void Convert (InType i0, InType i1, InType i2, OutType o0, OutType o1, OutType o2) = 0;

	};	// class ColrConverterFloat

template <class _Float = float>
	class YCC8toRGB8Converter_Float : public Colr3Converter<_Float &, _Float &>
	{
private:
	typedef Colr3Converter<_Float &, _Float &>	_Mybase;
	
public:
	typedef typename _Mybase::InType	InType;
	typedef typename _Mybase::OutType	OutType;
	
	virtual void Convert (InType i0, InType i1, InType i2, OutType o0, OutType o1, OutType o2)
		{
		#ifdef _USE_SSE
			float v0[4], v1[4], v2[4];
			float r[4];

			__m128	mi = _mm_set_ps(0, i2, i1, i0);
			__m128	md = _mm_set_ps(0, d[2], d[1], d[0]);
					mi = _mm_sub_ps(mi, md);

			*((__m128*)v0) = _mm_set_ps(0, m[0][2], m[0][1], m[0][0]);
			*((__m128*)v0) = _mm_mul_ps(*((__m128*)v0), mi);
			*((__m128*)v1) = _mm_set_ps(0, m[1][2], m[1][1], m[1][0]);
			*((__m128*)v1) = _mm_mul_ps(*((__m128*)v1), mi);
			*((__m128*)v2) = _mm_set_ps(0, m[2][2], m[2][1], m[2][0]);
			*((__m128*)v2) = _mm_mul_ps(*((__m128*)v2), mi);

			*((__m128*)r) = _mm_set_ps(0,v2[0],v1[0],v0[0]);
			__m128	mp = _mm_set_ps(0,v2[1],v1[1],v1[1]);
			*((__m128*)r) = _mm_add_ps(*((__m128*)r), mp);
					mp = _mm_set_ps(0, v2[2],v1[2],v0[2]);
			*((__m128*)r) = _mm_add_ps(*((__m128*)r), mp);
			o0 = r[0], o1 = r[1], o2 = r[2];

		#else
			i0 -= d[0], i1 -= d[1], i2 -= d[2];
			
			o0 = m[0][0] * i0 + m[0][1] * i1 + m[0][2] * i2;
			o1 = m[1][0] * i0 + m[1][1] * i1 + m[1][2] * i2;
			o2 = m[2][0] * i0 + m[2][1] * i1 + m[2][2] * i2;
		#endif		
		

		}


	YCC8toRGB8Converter_Float (int s = 0)
		{
		Set(s);
		}

	void Set (int s)
		{
		int t = 0;
		for (int i = 0; i < 3; i++)
			{
			for (int j = 0; j < 3; j++)
				{
				m[i][j] = Settings[s][t++];
				}
			}
		for (int i = 0; i < 3; i++)
			{
			d[i] = Settings[s][t++];
			}
		}

	_Float m[3][3];
	_Float d[3];

	static _Float Settings[3][12];

	};	// class YCC8toRGB8Converter_Float

template <class _Float>
	_Float YCC8toRGB8Converter_Float<_Float>::Settings[3][12] = 
	{
		// Uncompressed 8-bit 4:2:2
		{
		1.1644, -0.0027, 1.6001,
		1.1644, -0.3898, -0.8080,
		1.1644, 2.0183, 0.0090,
		16, 128, 128
		},

		// recommended by Microsoft
		// ref: http://www.microsoft.com/china/MSDN/library/enterprisedevelopment/softwaredev/VideoRende8BitYUV.mspx
		{
		1.164383, 0, 1.596027, 
		1.164383, -0.391762, -0.812968, 
		1.164383, 2.017232, 0, 
		16, 128, 128
		},

		// ref: http://www.fourcc.org/fccyvrgb.php
		{
		1.164, 0, 1.596,
		1.164, -0.391, -0.813, 
		1.164, 2.018, 0, 
		16, 128, 128
		},
	};

template <class _Float = float>
	class RGB8toYCC8Converter_Float : public Colr3Converter<_Float &, _Float &>
	{
private:
	typedef Colr3Converter<_Float &, _Float &>	_Mybase;
	
public:
	typedef typename _Mybase::InType	InType;
	typedef typename _Mybase::OutType	OutType;
	
	virtual void Convert (InType i0, InType i1, InType i2, OutType o0, OutType o1, OutType o2)
		{
		o0 = m[0][0] * i0 + m[0][1] * i1 + m[0][2] * i2 + d[0];
		o1 = m[1][0] * i0 + m[1][1] * i1 + m[1][2] * i2 + d[1];
		o2 = m[2][0] * i0 + m[2][1] * i1 + m[2][2] * i2 + d[2];
		}

	RGB8toYCC8Converter_Float (int s = 0)
		{
		Set(s);
		}

	void Set (int s)
		{
		int t = 0;
		for (int i = 0; i < 3; i++)
			{
			for (int j = 0; j < 3; j++)
				{
				m[i][j] = Settings[s][t++];
				}
			}
		for (int i = 0; i < 3; i++)
			{
			d[i] = Settings[s][t++];
			}
		}

	_Float m[3][3];
	_Float d[3];

	static _Float Settings[3][12];

	};	// class RGB8toYCC8Converter_Float

template <class _Float>
	_Float RGB8toYCC8Converter_Float<_Float>::Settings[3][12] = 
	{
		// Uncompressed 8-bit 4:2:2
		{
			0.2549, 0.5059, 0.0980,
			-0.1490, -0.2902, 0.4392,
			0.4392, -0.3686, -0.0706,
			16, 128, 128
		},

		// recommended by Microsoft
		// ref: http://www.microsoft.com/china/MSDN/library/enterprisedevelopment/softwaredev/VideoRende8BitYUV.mspx
		{
		0.256788, 0.504129, 0.097906, 
		-0.148223, -0.290993, 0.439216, 
		0.439216, -0.367788, -0.071427, 
		16, 128, 128
		},

		// ref: http://www.fourcc.org/fccyvrgb.php
		{
		0.257, 0.504, 0.098,
		-0.148, -0.291, 0.439, 
		0.439, -0.368, -0.071, 
		16, 128, 128
		},
	};

template <class _Float = float>
	class GammaCorrector_ZeroBased
	{
public:
	GammaCorrector_ZeroBased(_Float gamma, _Float span) 
		: m_Gamma(gamma), m_Degamma(1/gamma), m_Span(span)
		{
		}

	void Correct(_Float &val) const
		{
		val /= m_Span;
		val = pow(val, m_Gamma);
		val *= m_Span;
		}

	void Decorrect(_Float &val) const
		{
		val /= m_Span;
		val = pow(val, m_Degamma);
		val *= m_Span;
		}
private:
	_Float	m_Span;
	_Float	m_Gamma;
	_Float	m_Degamma;
	};	// class GammaCorrector


template <class _Float = float>
	class GammaCorrector
	{
public:
	GammaCorrector(_Float gamma, _Float min, _Float span) 
		: m_Gamma(gamma), m_Degamma(1/gamma), m_Min(min), m_Span(span)
		{
		}

	void Correct(_Float &val) const
		{
		val -= m_Min;
		val /= m_Span;
		val = pow(val, m_Gamma);
		val *= m_Span;
		val += m_Min;
		}

	void Decorrect(_Float &val) const
		{
		val -= m_Min;
		val /= m_Span;
		val = pow(val, m_Degamma);
		val *= m_Span;
		val += m_Min;
		}
private:
	_Float	m_Span;
	_Float	m_Min;
	_Float	m_Gamma;
	_Float	m_Degamma;
	};	// class GammaCorrector



_QUL_END

#endif	// ifndef _colrspc_H
