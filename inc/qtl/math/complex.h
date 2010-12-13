/**
 * === Math Library ===
 *  Unit Name:  complex
 *  Author:     Lincoln Yu
 *  Purpose:    An enhanced complex container derived from STL
 * == History ==        
 *  2005-11-12: Created.
  * == Comments ==
 *
 */

#ifndef _QTL_complex_H
#define _QTL_complex_H

#include <complex>
#include <iostream>

#include "../all/predef.h"
#include "../all/stlver.h"


_QTL_BEGIN

using namespace std;

// class math_complex

template <typename _Ty>
	class math_complex : public complex<_Ty>
	{
	typedef math_complex<_Ty>	_Myt;
	typedef complex<_Ty>		_Super;

public:
	math_complex () : _Super()
		{
		}

	math_complex (const _Ty &__r, const _Ty &__i) 
		: _Super(__r, __i)
		{
		}

	math_complex (const _Ty &__right)
		: _Super(__right, 0)
		{
		}

	math_complex (const _Myt &__right)
		: _Super((_Ty)__right._Super::real(), (_Ty)__right._Super::imag())
		{	// construct from other math_complex type
		}


	_Myt conjugate ()
		{
		return _Myt(_Super::real(), -_Super::imag());
		}

	_Myt &turn_conjugate ()
		{
		#if defined(_STLVER_PJP)
			_Super::imag(-_Super::imag());
		#else
			_Super::imag() = -_Super::imag();
		#endif
		return (*this);
		}

	_Myt &turn_negative ()
		{
		#if defined(_STLVER_PJP)
			_Super::real(-_Super::real());
			_Super::imag(-_Super::imag());
		#else
			_Super::real() = -_Super::real();
			_Super::imag() = -_Super::imag();
		#endif
		return (*this);
		}
	
	_Ty modulus_square ()
		{
		return _Super::real() * _Super::real() + _Super::imag() * _Super::imag();
		}

	operator _Ty ()
		{
		return _Super::real();
		}

	_Myt operator- ()
		{
		_Myt __temp(-_Super::real(), -_Super::imag());
		return __temp;
		}

	_Myt& operator= (const _Ty &__right)
		{	// assign real
		#if defined(_STLVER_PJP)
			_Super::real(__right);
			_Super::imag(0);
		#else
			_Super::real() = __right;
			_Super::imag() = 0;
		#endif
		return (*this);
		}

	_Myt& operator= (const _Myt &__right)
		{	// assign other complex type
		#if defined(_STLVER_PJP)
			_Super::real(__right._Super::real());
			_Super::imag(__right._Super::imag());
		#else
			_Super::real() = __right._Super::real();
			_Super::imag() = __right._Super::imag();
		#endif
		return (*this);
		}
	
// operators

	_Myt& operator+= (const _Ty &__right)
		{	// add real
		#if defined(_STLVER_PJP)
			_Super::real(_Super::real() + __right);
		#else
			_Super::real() = _Super::real() + __right;
		#endif
		return (*this);
		}

	_Myt operator+ (const _Ty &__right)
		{	// add real
		_Myt __temp(*this);
		__temp += __right;
		return __temp;
		}

	_Myt& operator-= (const _Ty &__right)
		{	// subtract real
		#if defined(_STLVER_PJP)
			_Super::real(_Super::real() - __right);
		#else
			_Super::real() = _Super::real() - __right;
		#endif
		return (*this);
		}

	_Myt operator- (const _Ty &__right)
		{	// add real
		_Myt __temp(*this);
		__temp -= __right;
		return __temp;
		}

	_Myt& operator*= (const _Ty &__right)
		{	// multiply by real
		#if defined(_STLVER_PJP)
			_Super::real(_Super::real() * __right);
			_Super::imag(_Super::imag() * __right);
		#else
			_Super::real() = _Super::real() * __right;
			_Super::imag() = _Super::imag() * __right;
		#endif
		return (*this);
		}

	_Myt operator* (const _Ty &__right)
		{	// add real
		_Myt __temp(*this);
		__temp *= __right;
		return __temp;
		}

	_Myt& operator/= (const _Ty &__right)
		{	// divide by real
		#if defined(_STLVER_PJP)
			_Super::real(_Super::real() / __right);
			_Super::imag(_Super::imag() / __right);
		#else
			_Super::real() = _Super::real() / __right;
			_Super::imag() = _Super::imag() / __right;
		#endif
		return (*this);
		}

	_Myt operator/ (const _Ty &__right)
		{	// add real
		_Myt __temp(*this);
		__temp /= __right;
		return __temp;
		}


	_Myt& operator+= (const _Myt &__right)
		{	// add other complex
		#if defined(_STLVER_PJP)
			this->_Add(__right);
		#else
			_Super::operator+=(__right);
		#endif
		return (*this);
		}

	_Myt operator+ (const _Myt &__right)
		{	// add real
		_Myt __temp(*this);
		__temp += __right;
		return __temp;
		}

	_Myt& operator-= (const _Myt &__right)
		{	// subtract other complex
		#if defined(_STLVER_PJP)
			this->_Sub(__right);
		#else
			_Super::operator-=(__right);
		#endif
		return (*this);
		}

	_Myt operator- (const _Myt &__right)
		{	// add real
		_Myt __temp(*this);
		__temp -= __right;
		return __temp;
		}

	_Myt& operator*= (const _Myt &__right)
		{	// multiply by other complex
		#if defined(_STLVER_PJP)
			this->_Mul(__right);
		#else
			_Super::operator*=(__right);
		#endif
		return (*this);
		}

	_Myt operator* (const _Myt &__right)
		{	// add real
		_Myt __temp(*this);
		__temp *= __right;
		return __temp;
		}


	_Myt& operator/= (const _Myt &__right)
		{	// divide by other complex
		#if defined(_STLVER_PJP)
			this->_Div(__right);
		#else
			_Super::operator/=(__right);
		#endif
		return (*this);
		}

	_Myt operator/ (const _Myt &__right)
		{	// add real
		_Myt __temp(*this);
		__temp /= __right;
		return __temp;
		}

	};	// class complex


template <typename _Ty>
	inline math_complex<_Ty> operator+ (const _Ty &__left, const math_complex<_Ty> &__right)
	{
	math_complex<_Ty> __temp(__right);
	__temp += __left;
	return __temp;
	}

template <typename _Ty>
	inline math_complex<_Ty> operator- (const _Ty &__left, const math_complex<_Ty> &__right)
	{
	math_complex<_Ty> __temp(__left);
	__temp -= __right;
	return __temp;
	}

template <typename _Ty>
	inline math_complex<_Ty> operator* (const _Ty &__left, const math_complex<_Ty> &__right)
		{
	math_complex<_Ty> __temp(__right);
	__temp *= __left;
	return __temp;
	}

template <typename _Ty>
	inline math_complex<_Ty> operator/ (const _Ty &__left, const math_complex<_Ty> &__right)
	{
	math_complex<_Ty> __temp(__left);
	__temp /= __right;
	return __temp;
	}

template <typename _Ty>
	inline ostream & operator<< (ostream &__out_strm, const math_complex<_Ty> &__val)
	{
	__out_strm << '['<< __val.real() << ',' << __val.imag() << ']';
	return __out_strm;
	}

_QTL_END


#endif	// ifndef _QTL_complex_H
