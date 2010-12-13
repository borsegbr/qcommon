/**
 * === Signal Processing Library ===
 *  Unit Name:  fir
 *  Author:     Lincoln Yu
 *  Purpose:    FIR filter emulation
 * == History ==        
 *  2006-12-30: Created.
 * == Comments ==
 *
 */

#if !defined(_fir_H)
#define _fir_H	"20061229"

#include "../all/predef.h"


#include <vector>


_QTL_BEGIN


template <class _Coeff = int>
class fir_filter
	{
private:
	typedef	std::vector<_Coeff>					_CoeffVec;
	typedef typename _CoeffVec::iterator		_CoeffVecItr;
	typedef typename _CoeffVec::const_iterator	_CoeffVecCItr;
	

public:

	fir_filter(int _size = 1) 
		: _M_coeffs(_size, 0)
		{
		if (_size <= 0)
			{
			_M_coeffs.resize(1);
			}
		}

	~fir_filter()
		{
		}

	void resize (int size)
		{
		_M_coeffs.resize(size);
		}

	int get_length ()
		{
		return _M_coeffs.size();
		}

	_Coeff & operator[] (int _index)
		{
		if (_index < 0)
			{
			_index = 0;
			}
		if (_index >= (int)_M_coeffs.size())
			{
			// enlarge
			int _new_size = (_index+1) * 4 / 3;
			_M_coeffs.resize(_new_size);
			}
		return _M_coeffs[_index];
		}

	const _Coeff & operator[] (int _index) const
		{
		if (_index < 0 || _index >= _M_coeffs.size())
			{
			_index = 0;
			}
		return _M_coeffs[_index];
		}

	template <class _SampleItr>
			void response(const _SampleItr &_input_begin, _Coeff &_output) const
		{
		_CoeffVecCItr	_itrc = _M_coeffs.begin();
		_SampleItr		_itrsmp = _input_begin;
		
		_output = 0;
		
		for (; _itrc != _M_coeffs.end(); ++_itrc, ++_itrsmp)
			{
			_output += (*_itrsmp) * (*_itrc);
			}
		}
	
	template <class _SampleItr>
			void response(const _SampleItr &_input_begin, _Coeff &_output, int _len) const
			/*
			* Pre: assure [:_len:] is less than length of coefficient array
			*/
		{
		_CoeffVecCItr	_itrc = _M_coeffs.begin();
		_SampleItr		_itrsmp = _input_begin;

		_output = 0;

		for (int i = 0; i < _len; ++_itrc, ++_itrsmp)
			{
			_output += (*_itrsmp) * (*_itrc);
			}
		}

	template <class _SampleItr>
			void response(const _SampleItr &_input_begin, const _SampleItr &_input_end,
			_Coeff &_output) const
		{
		_CoeffVecCItr	_itrc = _M_coeffs.begin();
		_SampleItr		_itrsmp = _input_begin;

		_output = 0;

		for (; _itrsrc != _input_end; ++_itrc, ++_itrsmp)
			{
			_output += (*_itrsmp) * (*_itrc);
			}
		}

	template <class _SampleItr>
			void response(const _SampleItr &_input_begin, _Coeff &_output, int _subsampling, 
			int _len) const
			/*
			 * Pre: assure [:_len:] is less than length of coefficient array
			 */
		{
		_CoeffVecCItr	_itrc = _M_coeffs.begin();
		_SampleItr		_itrsmp = _input_begin;

		_output = 0;
		for (int i = 0; i < _len; ++i, ++_itrc, _itrsmp+=_subsampling)
			{
			_output += (*_itrsmp) * (*_itrc);
			}
		}


private:
	_CoeffVec	_M_coeffs;

	};

_QTL_END


#endif	// if !defined(_fir_H)
