/**
 * === Signal Processing Library ===
 *  Unit Name:  dct
 *  Author:     Lincoln Yu
 *  Purpose:    classical spectrum analysis algorithm discrete cosine transform (DCT), 
 *				a version based on FFT, proposed by Matlab, see the "References" segment.
 * == History ==        
 *  2005-10-11: Created
 *  2005-11-12: Debugged
 * == Comments ==
 *
 * == References ==
 *  Original Matlab functions, see the bottom of the file.
 *
 */

#ifndef _QTL_dct_H
#define _QTL_dct_H

#include "../all/predef.h"
#include "fft.h"

_QTL_BEGIN	
	
template <class _FFT_Traits = fft_default_traits>
	class dct_wizard 
	{
protected:
	typedef _FFT_Traits							FFT_Traits;
	typedef fft_plus<FFT_Traits>				Plus;

	typedef typename FFT_Traits::sample_type	sample_type;
	typedef typename FFT_Traits::work_type		work_type;

	typedef vector<work_type>					work_vec;
	typedef typename work_vec::iterator			work_iter;
	typedef typename work_vec::reverse_iterator	work_rev_iter;

	typedef fft_wizard<FFT_Traits>				fft_type;
	typedef typename fft_type::work_iter		fft_work_iter;

	typedef typename Plus::factors_byvec		factors_byvec;

public:
    typedef typename FFT_Traits::index_type	index_type;
	typedef typename FFT_Traits::factor_type	factor_type;
	typedef typename FFT_Traits::step_type		step_type;

	typedef typename Plus::factors_dynctnr		factors_dynctnr;
	typedef typename Plus::factors_dyniter		factors_dyniter;


	index_type									_M_len;

	factors_dynctnr								_M_factors;
	factors_dyniter								_M_begin, _M_begin_minor;
	factors_dyniter								_M_end;
	step_type									_M_num_steps;

	fft_type									_M_fft;
	

public:

	void set_length (index_type __len)
		{
		_M_len = __len * 2;

		_M_factors = factors_byvec(_M_len);

		_M_begin = _M_factors->begin();
		_M_begin_minor = _M_begin;
		++_M_begin_minor;

		_M_end = _M_factors->end();

		_M_num_steps = _M_factors->num_steps();

		}
	
	template <class _IterSmp, class _IterSpc, bool _IsReal>
		void forward_transform (const _IterSmp &__smp_begin, const _IterSpc &__spc_begin)
		{
		index_type __len_minor = _M_len / 2;

		sample_type __w_0;
		sample_type __f1 = FFT_Traits::_S_Pi / _M_len, __f2;
		

		fft_work_iter __fft_out_begin;

		work_vec __fft_src;


		if ( __len_minor % 2 == 1 || ! _IsReal ) {
			index_type __len = _M_len;
			__fft_src.resize(__len);

			_IterSmp __it_smp;
			
			work_iter __it_wrk;
			work_iter __wrk_mid = __fft_src.begin() + __len_minor;
			for ( __it_wrk = __fft_src.begin(), __it_smp = __smp_begin; __it_wrk < __wrk_mid; *__it_wrk++ = *__it_smp++ );

			work_rev_iter __it_r_wrk;
			work_rev_iter __wrk_r_mid = __fft_src.rbegin() + __len_minor;
			for ( __it_r_wrk = __fft_src.rbegin(), __it_smp = __smp_begin; __it_r_wrk < __wrk_r_mid; *__it_r_wrk++ = *__it_smp++);


			_M_fft.set_params(_M_factors, _M_begin, _M_end, _M_num_steps, __len);
			__fft_out_begin = _M_fft.forward_transform(__fft_src.begin());


			__w_0 = FFT_Traits::_S_sqrt((sample_type)__len_minor) * 2;
			__f2 = (sample_type)1 / FFT_Traits::_S_sqrt((sample_type)_M_len);

			}
		else {
			index_type __len = __len_minor;
			__fft_src.resize(__len);

			work_iter __it_wrk;
			work_iter __wrk_mid = __fft_src.begin() + __len_minor / 2;
			
			_IterSmp __it_smp;
			for ( __it_wrk = __fft_src.begin(), __it_smp = __smp_begin; __it_wrk < __wrk_mid; 
				*__it_wrk = *__it_smp, ++__it_wrk, ++++__it_smp );

			work_rev_iter __it_r_wrk;
			work_rev_iter __wrk_r_mid = __fft_src.rbegin() + __len_minor / 2;
			for ( __it_r_wrk = __fft_src.rbegin(), __it_smp = __smp_begin, ++__it_smp; __it_r_wrk < __wrk_r_mid; 
				*__it_r_wrk = *__it_smp, ++__it_r_wrk, ++++__it_smp );


			_M_fft.set_params(_M_factors, _M_begin_minor, _M_end, _M_num_steps - 1, __len);
			__fft_out_begin = _M_fft.forward_transform(__fft_src.begin());


			__w_0 = FFT_Traits::_S_sqrt((sample_type)__len_minor);
			__f2 = FFT_Traits::_S_sqrt((sample_type)2 / __len_minor);

			}


		fft_work_iter __it_fft_out = __fft_out_begin;
		_IterSpc __it_spc = __spc_begin;

		*__it_spc++ = *__it_fft_out++ / __w_0;

		for ( index_type i = 1; i < __len_minor; ++i, ++__it_spc, ++__it_fft_out )					
			*__it_spc = *__it_fft_out * work_type(FFT_Traits::_S_cos (i * __f1) * __f2, -FFT_Traits::_S_sin(i * __f1) * __f2);

		// if source is real, the receiving iterator is supposed to be real, or processed by user
		// so no real casting procedure here

		}

	template <class _IterSpc, class _IterSmp, bool _IsReal>
		void inverse_transform(const _IterSpc &__spc_begin, const _IterSmp &__smp_begin)
		{
		work_type __neg_i(0, -1);
		index_type __len_minor = _M_len / 2;
		index_type __len;

		work_vec __fft_src;

		sample_type __w;
		sample_type __f1 = FFT_Traits::_S_Pi / _M_len, __f2 = FFT_Traits::_S_sqrt((sample_type)_M_len);
		
		fft_work_iter __fft_out_begin;

		if ( __len_minor % 2 == 1 || ! _IsReal ) {
			__len = _M_len;
			work_vec __fft_src(__len);

			__w = FFT_Traits::_S_sqrt((sample_type)__len_minor) * 2;


			_IterSpc __it_spc = __spc_begin;
			
			index_type i;
			work_iter __it_wrk = __fft_src.begin();

			*__it_wrk++ = *__it_spc++ * __w;
			for ( i = 1; i < __len_minor; ++i, ++__it_wrk, ++__it_spc)
				*__it_wrk = *__it_spc * work_type(FFT_Traits::_S_cos(i * __f1) * __f2, FFT_Traits::_S_sin(i * __f1) * __f2);
			
			work_rev_iter  __it_r_wrk  = __fft_src.rbegin();
			for ( i = __len_minor - 1, __it_spc = __spc_begin, ++__it_spc; i > 0; --i, ++__it_r_wrk, ++__it_spc)
				*__it_r_wrk = *__it_spc * work_type(FFT_Traits::_S_sin(i * __f1) * __f2, -FFT_Traits::_S_cos(i * __f1) * __f2);

			*__it_r_wrk = 0;

			_M_fft.set_params(_M_factors, _M_begin, _M_end, _M_num_steps, __len);
			__fft_out_begin = _M_fft.inverse_transform(__fft_src.begin());


			fft_work_iter __it_fft_out;
			_IterSmp __it_smp;
			for (i = 0, __it_smp = __smp_begin, __it_fft_out = __fft_out_begin; i < __len_minor; ++i, *__it_smp++ = *__it_fft_out++);
			}
		else {
			__len = __len_minor;
			work_vec __fft_src(__len);

			__w = FFT_Traits::_S_sqrt((sample_type)__len_minor);
			
			
			_IterSpc __it_spc = __spc_begin;
			work_iter __it_wrk = __fft_src.begin();

			*__it_wrk = *__it_spc * __w;
			index_type i;
			for (i = 1, ++__it_wrk, ++__it_spc; i < __len_minor; ++i, ++__it_wrk, ++__it_spc)
				*__it_wrk = *__it_spc * work_type(FFT_Traits::_S_cos(i * __f1) * __f2, FFT_Traits::_S_sin(i * __f1) * __f2);


			_M_fft.set_params(_M_factors, _M_begin_minor, _M_end, _M_num_steps - 1, __len);
			__fft_out_begin = _M_fft.inverse_transform(__fft_src.begin());


			fft_work_iter __fft_out_end = __fft_out_begin + __len_minor;
			fft_work_iter __fft_out_mid = __fft_out_begin + __len_minor / 2;
			fft_work_iter __it_fft_out = __fft_out_begin;
			_IterSmp __it_smp;

			for ( __it_smp = __smp_begin, __it_fft_out = __fft_out_begin; __it_fft_out < __fft_out_mid; 
				*__it_smp++ = *__it_fft_out++, ++__it_smp );

			--__fft_out_mid;
			for ( __it_smp = __smp_begin, ++__it_smp, __it_fft_out = __fft_out_end, --__it_fft_out; 
				__it_fft_out > __fft_out_mid; 
				*__it_smp++ = *__it_fft_out--, ++__it_smp );

			}

		}

	};	// class dct_wizard


_QTL_END

#endif	// ifndef _QTL_dct_H


// Original Matlab functions: 'dct' and 'idct'
/*
	function b=dct(a,n)
	%DCT  Discrete cosine transform.
	%
	%   Y = DCT(X) returns the discrete cosine transform of X.
	%   The vector Y is the same size as X and contains the
	%   discrete cosine transform coefficients.
	%
	%   Y = DCT(X,N) pads or truncates the vector X to length N 
	%   before transforming.
	%
	%   If X is a matrix, the DCT operation is applied to each
	%   column.  This transform can be inverted using IDCT.
	%
	%   See also FFT, IFFT, IDCT.

	%   Author(s): C. Thompson, 2-12-93
	%              S. Eddins, 10-26-94, revised
	%   Copyright 1988-2002 The MathWorks, Inc.
	%   $Revision: 1.7 $  $Date: 2002/03/28 17:27:34 $

	%   References: 
	%   1) A. K. Jain, "Fundamentals of Digital Image
	%      Processing", pp. 150-153.
	%   2) Wallace, "The JPEG Still Picture Compression Standard",
	%      Communications of the ACM, April 1991.


	if nargin == 0,
		error('Not enough input arguments.');
	end

	if isempty(a)
	b = [];
	return
	end

	% If input is a vector, make it a column:
	do_trans = (size(a,1) == 1);
	if do_trans, a = a(:); end

	if nargin==1,
	n = size(a,1);
	end
	m = size(a,2);

	% Pad or truncate input if necessary
	if size(a,1)<n,
	aa = zeros(n,m);
	aa(1:size(a,1),:) = a;
	else
	aa = a(1:n,:);
	end

	% Compute weights to multiply DFT coefficients
	ww = (exp(-i*(0:n-1)*pi/(2*n))/sqrt(2*n)).';
	ww(1) = ww(1) / sqrt(2);

	if rem(n,2)==1 | ~isreal(a), % odd case
	% Form intermediate even-symmetric matrix
	y = zeros(2*n,m);
	y(1:n,:) = aa;
	y(n+1:2*n,:) = flipud(aa);
	  
	% Compute the FFT and keep the appropriate portion:
	yy = fft(y);  
	yy = yy(1:n,:);

	else % even case
	% Re-order the elements of the columns of x
	y = [ aa(1:2:n,:); aa(n:-2:2,:) ];
	yy = fft(y);  
	ww = 2*ww;  % Double the weights for even-length case  
	end

	% Multiply FFT by weights:
	b = ww(:,ones(1,m)) .* yy;

	if isreal(a), b = real(b); end
	if do_trans, b = b.'; end


	function a = idct(b,n)
	%IDCT Inverse discrete cosine transform.
	%
	%   X = IDCT(Y) inverts the DCT transform, returning the
	%   original vector if Y was obtained using Y = DCT(X).
	%
	%   X = IDCT(Y,N) pads or truncates the vector Y to length N 
	%   before transforming.
	%
	%   If Y is a matrix, the IDCT operation is applied to
	%   each column.
	%
	%   See also FFT, IFFT, DCT.

	%   Author(s): C. Thompson, 2-12-93
	%              S. Eddins, 10-26-94, revised
	%   Copyright 1988-2002 The MathWorks, Inc.
	%   $Revision: 1.7 $  $Date: 2002/03/28 17:28:22 $

	%   References: 
	%   1) A. K. Jain, "Fundamentals of Digital Image
	%      Processing", pp. 150-153.
	%   2) Wallace, "The JPEG Still Picture Compression Standard",
	%      Communications of the ACM, April 1991.

	if nargin == 0,
		error('Not enough input arguments.');
	end

	if isempty(b),
	a = [];
	return
	end

	% If input is a vector, make it a column:
	do_trans = (size(b,1) == 1);
	if do_trans, b = b(:); end
	   
	if nargin==1,
	n = size(b,1);
	end
	m = size(b,2);

	% Pad or truncate b if necessary
	if size(b,1)<n,
	bb = zeros(n,m);
	bb(1:size(b,1),:) = b;
	else
	bb = b(1:n,:);
	end

	% Compute wieghts
	ww = sqrt(2*n) * exp(j*(0:n-1)*pi/(2*n)).';

	if rem(n,2)==1 | ~isreal(b), % odd case
	% Form intermediate even-symmetric matrix.
	ww(1) = ww(1) * sqrt(2);
	W = ww(:,ones(1,m));
	yy = zeros(2*n,m);
	yy(1:n,:) = W.*bb;
	yy(n+2:2*n,:) = -j*W(2:n,:).*flipud(bb(2:n,:));
	  
	y = ifft(yy);

	% Extract inverse DCT
	a = y(1:n,:);

	else % even case
	% Compute precorrection factor
	ww(1) = ww(1)/sqrt(2);
	W = ww(:,ones(1,m));
	yy = W.*bb;

	% Compute x tilde using equation (5.93) in Jain
	y = ifft(yy);
	  
	% Re-order elements of each column according to equations (5.93) and
	% (5.94) in Jain
	a = zeros(n,m);
	a(1:2:n,:) = y(1:n/2,:);
	a(2:2:n,:) = y(n:-1:n/2+1,:);
	end

	if isreal(b), a = real(a); end
	if do_trans, a = a.'; end

*/
