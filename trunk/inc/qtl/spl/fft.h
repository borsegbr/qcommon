/**
 * === Signal Processing Library ===
 *  Unit Name:  fft
 *  Author:     Lincoln Yu
 *  Purpose:    classical spectrum analysis algorithm, fast fourier transform (FFT).
 * == History ==        
 *  2005-10-31: Created.
 *  2005-11-05: Debugged.
 *	2005-12-08: Modified and debugged with unit autoptr changed.
 *  2006-05-01: Some expediency to flirt with the Super Idiot (MinGW) GNU C++ Compiler. 
 *  2007-12-09: Compliance with Visual Studio 2005, range checking on vector iterator of
 *              which would fail at certain places of code with iterator hopping.
 * == Comments ==
 *
 */

#ifndef _QTL_fft_H
#define _QTL_fft_H

#define _SECURE_SCL                 0
#define _HAS_ITERATOR_DEBUGGING     0

#include <utility>
#include <vector>
#include <cmath>

#include "../all/predef.h"
#include "../ctnr/autoptr.h"
#include "../num/factors.h"
#include "../math/complex.h"


_QTL_BEGIN

using namespace std;

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

#ifndef M_DBLPI
	#define M_DBLPI     6.28318530717958647693
#endif

#ifndef M_PI_2
	#define M_PI_2		1.57079632679489661923
#endif

// struct fft_default_traits

struct fft_default_traits
	{

// type definition
	typedef unsigned long						index_type;
	typedef unsigned long						factor_type; 
	typedef unsigned short						step_type;

	typedef double								sample_type;
	typedef math_complex<sample_type>			work_type;

	enum direction								{ forward, inverse };


// for FFT
	typedef										sample_type (*_MathFunc)(sample_type);

	static const sample_type					_S_DblPi;
	static const _MathFunc						_S_cos;
	static const _MathFunc						_S_sin;
	static const _MathFunc						_S_sqrt;

// not necessary to FFT
	static const sample_type					_S_Pi;
	static const sample_type					_S_HlfPi;

	};	// struct fft_default_traits

const fft_default_traits::sample_type	fft_default_traits::_S_DblPi = M_DBLPI;
const fft_default_traits::_MathFunc		fft_default_traits::_S_cos = cos;
const fft_default_traits::_MathFunc		fft_default_traits::_S_sin = sin;
const fft_default_traits::_MathFunc		fft_default_traits::_S_sqrt = sqrt;
const fft_default_traits::sample_type	fft_default_traits::_S_Pi = M_PI;
const fft_default_traits::sample_type	fft_default_traits::_S_HlfPi = M_PI_2;



// struct discrete_fourier

template <class _Traits = fft_default_traits>
	struct fft_kernel
	{

	typedef typename _Traits::index_type		index_type;
	typedef typename _Traits::factor_type		factor_type;
	typedef typename _Traits::step_type			step_type;
	typedef typename _Traits::work_type			work_type;
	typedef typename _Traits::sample_type		sample_type;


	typedef fft_default_traits::direction		direction;

	// __step_begin is reversive when using destination continuity,
	// Normalization is also done in this function.
	template <class _ItrSrc, class _ItrDst, class _FctItr
		#if defined(_MSC_VER)
			, bool _SrcCont
		#endif	
		>
		void factorder (
		const _ItrSrc &__src_begin, const _ItrDst &__dst_begin, 
		const index_type &__len, const _FctItr &__step_begin, const step_type &__num_steps, 
		const sample_type &__coef
		#if !defined(_MSC_VER)
			, bool _SrcCont
		#endif	
		) const
		{
		typedef vector<factor_type>				FctVec;
		typedef typename FctVec::iterator		FctVecItr;
		
		_FctItr		__it_fct;
		FctVec		__cur_code(__num_steps, 0);
		FctVecItr	__it_cc;
		_ItrSrc		__it_src = __src_begin;
		_ItrDst		__it_dst = __dst_begin;

		*__it_dst = *__it_src, *__it_dst *= __coef;		// they should be compatible


		index_type __idx;

		if ( _SrcCont )
			++__it_dst;
		else
			++__it_src;

		for (__idx = 1; __idx < __len; ++__idx)
			{
			index_type __weight = __len;
			index_type __aim = 0;
			step_type __idx_steps;
			++__cur_code[0];

			for ( __idx_steps = 0, __it_cc = __cur_code.begin(), __it_fct = __step_begin; 
				__idx_steps < __num_steps; ++__idx_steps, ++__it_cc, ++__it_fct )
				{
				__weight /= *__it_fct;
				if (*__it_cc >= *__it_fct)
					{
					FctVecItr __it_tmp = __it_cc;
					*__it_cc = 0;
					++(*++__it_tmp);
					}
				__aim += *__it_cc * __weight;
				}
			if ( _SrcCont ) {
				*__it_dst = *(__it_src + __aim), *__it_dst *= __coef;	
				++__it_dst;
				}
			else {
				_ItrDst __it_dst_tmp = __it_dst + __aim;
				*__it_dst_tmp = *__it_src, *__it_dst_tmp *= __coef;
				++__it_src;
				}
			}
		}		
	
	template <class _ItrSrc, class _ItrDst, direction _Direction>
		void step(const _ItrSrc &__src_begin, const _ItrSrc &__src_end,
		const _ItrDst &__dst_begin, const _ItrDst &__dst_end, const index_type &__blk_scale, 
		const index_type &__last_blk_scale, const factor_type &__current_factor) const
		{
		index_type i = 0;
		for ( _ItrDst __it_dst = __dst_begin; __it_dst < __dst_end; ++__it_dst, ++i ) 
			{
			*__it_dst = work_type();	// zero
			index_type j = (i / __blk_scale) * __blk_scale;
			index_type __level_in_blk = i % __blk_scale;
			index_type __level_in_last_blk = i % __last_blk_scale;
			j += __level_in_last_blk;
			sample_type __fixed = _Traits::_S_DblPi / __blk_scale * __level_in_blk;
			_ItrSrc __it_src = __src_begin; __it_src += j;
			for ( index_type __last_blk_num = 0; 
				__last_blk_num < __current_factor; 
				++__last_blk_num, __it_src += __last_blk_scale) 
				{
				if ( _Direction == fft_default_traits::forward ) {
					work_type __multiplier(_Traits::_S_cos(__fixed * __last_blk_num), 
						-_Traits::_S_sin(__fixed * __last_blk_num));
					*__it_dst += *__it_src * __multiplier;
					}
				else if (_Direction == fft_default_traits::inverse ) {
					work_type __multiplier(_Traits::_S_cos(__fixed * __last_blk_num), 
						_Traits::_S_sin(__fixed * __last_blk_num));
					*__it_dst += *__it_src * __multiplier;
					}
				else { 
					// error
					}
				}
			}
		}

	// the iterator swapped when step number is odd
	template <class _WrkItr, class _FctItr, direction _Direction>
		void transform (_WrkItr &__master_begin, _WrkItr &__master_end, 
		_WrkItr &__slave_begin, _WrkItr &__slave_end, const _FctItr &__step_begin, 
		const _FctItr &__step_end) const
		{
		index_type __last_blk_scale = 1, __blk_scale = 1;

		for ( _FctItr __it_fct = __step_begin; __it_fct < __step_end; ++__it_fct )
			{
			factor_type __current_factor = *__it_fct;
			__blk_scale *= __current_factor;

			// the transform is from slave to master, and valid data is 
			// originally stored in master
            // Some ineptitude environment like VS2005 would be incapable of 
            // passing through the code if set the template parameter for [:swap:]
			swap(__master_begin, __slave_begin);
			swap(__master_end, __slave_end);

			step<_WrkItr, _WrkItr, _Direction>(__slave_begin, __slave_end, __master_begin, __master_end,
				__blk_scale, __last_blk_scale, __current_factor);

			__last_blk_scale = __blk_scale;
			}
		}

	};	// struct fft_kernel


// struct fft_plus

template <class _Traits = fft_default_traits>
	struct fft_plus
	{
	typedef typename _Traits::index_type		index_type;
	typedef typename _Traits::factor_type		factor_type;
	typedef typename _Traits::step_type			step_type;


	typedef const factor_type &					FctCRef;

	typedef vector<factor_type>					FctVec;
	typedef typename FctVec::iterator			FctVecItr;
	typedef const FctVecItr &					FctVecItrCRef;


// base autoptr target for factor iterator for FFT
	struct factors_iterator_base_target
		{
		factors_iterator_base_target() {}


		virtual factors_iterator_base_target * copy_this() const = 0;
		virtual void move_next() = 0;
		virtual void move_prev() = 0;
		virtual FctCRef get_value() const = 0;
		virtual bool is_before (const factors_iterator_base_target *__right) const = 0;
		};

// dynamic iterator
	class factors_dyniter : public autoptr<factors_iterator_base_target>
		{
		typedef factors_iterator_base_target	_Target;
		typedef autoptr<_Target>				_Super;

	public:
		factors_dyniter & operator++ ()
			{
			_Target *__p = (_Target *)_Super::_get_pTarget();
			if (!_Super::_alone())
				this->_Super::operator=(__p = __p->copy_this());

			__p->move_next();

			return (*this);
			}
		
		factors_dyniter & operator-- ()
			{
			_Target *__p = (_Target *)_Super::_get_pTarget();
			if (!_Super::_alone())
				this->_Super::operator=(__p = __p->copy_this());

			__p->move_prev();

			return (*this);
			}

		FctCRef operator *() const
			{
			_Target *__p = (_Target *)_Super::_get_pTarget();
			return ( __p->get_value() );
			}

		bool operator < (const factors_dyniter &__right) const
			{
			_Target *__p = (_Target *)_Super::_get_pTarget();
			_Target *__p2 = (_Target *)__right._Super::_get_pTarget();
			return (__p->is_before( __p2 ) );
			}

		void assign (_Target *__t)
			{
			this->_Super::operator=(__t->copy_this());
			}

		};	// class factors_dyniter

	typedef const factors_dyniter &			FctItrCRef;

// base autoptr target for factor container for FFT
	struct factors_base_target
		{
		virtual void set_length (index_type __len) = 0;
		virtual FctItrCRef begin () const = 0;
		virtual FctItrCRef end () const = 0;
		virtual step_type num_steps () const = 0;
		};

// dynamic factor container
	typedef autoptr<factors_base_target>	factors_dynctnr;

// an implementation of autoptr target for factor container as a sample
	class factors_byvec_target : public factors_base_target 
		{
		typedef factors_base_target				_Super;

		typedef vector_pushbacker<factor_type>	factor_pb;
		typedef const factor_pb &				pb_const_ref;
		typedef factors_iterator_base_target	BaseItr;

		typedef const BaseItr *					BaseItrCPtr;

		struct iterator : public BaseItr
			{
			FctVecItr		_M_iter;
			
			iterator(const FctVecItr &__fctvec_it) : BaseItr(), _M_iter(__fctvec_it) 
				{
				}

			iterator(const iterator &__copy) : BaseItr(), _M_iter(__copy._M_iter)
				{
				}

			virtual BaseItr * copy_this() const {
				iterator *res = new iterator(*this);
				return res;
				}

			virtual void move_next() { ++_M_iter; }
			virtual void move_prev() { --_M_iter; }
			virtual FctCRef get_value () const { return (*_M_iter); }

			virtual bool is_before (BaseItrCPtr __right) const 
				{ 
				return (_M_iter < ((iterator *)__right)->_M_iter);
				}

			};	// struct iterator

		FctVec									_M_fct_vec;
		factors_dyniter							_M_fct_begin, _M_fct_end;

	public:
		virtual void set_length (index_type __len)
			{
			factor_pb pushbacker;
			_M_fct_vec.clear();
			resolve_factors <index_type, factor_type, FctVec, pb_const_ref>
				(__len, (factor_type)(sqrt((float)__len) + 1), _M_fct_vec, pushbacker);

			iterator __it_begin(_M_fct_vec.begin()), __it_end(_M_fct_vec.end());

			_M_fct_begin.assign(&__it_begin);
			_M_fct_end.assign(&__it_end);

			}

		virtual FctItrCRef begin () const 
			{ 
			return _M_fct_begin; 
			}

		virtual FctItrCRef end () const 
			{ 
			return _M_fct_end; 
			}
		
		virtual step_type num_steps () const { return ((step_type)_M_fct_vec.size()); }

		};	// class factors_byvec_target

// a simple autoptr of the above target only for constructing
	class factors_byvec : public autoptr<factors_byvec_target>
		{
		typedef autoptr<factors_byvec_target>	_Super;
	public:
		factors_byvec (index_type __len) : _Super(new factors_byvec_target())
			{
			factors_base_target *__p = (factors_base_target *)_Super::_get_pTarget();

			__p->set_length(__len);
			}
		};

	
	};	// struct fft_plus


// class fft_wizard

template <class _Traits = fft_default_traits, class _Plus = fft_plus<_Traits> >
class fft_wizard
	{
	typedef fft_wizard<_Traits, _Plus>			_Myt;

public:
	typedef typename _Traits::index_type		index_type;
	typedef typename _Traits::factor_type		factor_type;
	typedef typename _Traits::step_type			step_type;

	typedef typename _Traits::work_type			work_type;
	typedef typename _Traits::sample_type		sample_type;
	typedef	vector<work_type>					work_vec;
	typedef typename work_vec::iterator			work_iter;

	typedef typename _Plus::factors_dynctnr		factors_dynctnr;
	typedef typename _Plus::factors_dyniter		factors_dyniter;
	typedef typename _Plus::factors_byvec		factors_byvec;

	typedef fft_default_traits::direction		direction;

protected:
// type definition

	typedef typename _Plus::FctVec				FctVec;
	typedef typename _Plus::FctVecItr			FctVecItr;
	typedef typename _Plus::FctVecItrCRef		FctVecItrCRef;

	typedef fft_kernel<_Traits>					Kernel;

// member variables

	Kernel										_M_kernel;

	index_type									_M_len;		// number of samples

	step_type									_M_num_steps;
	factors_dyniter								_M_step_begin;		
	factors_dyniter								_M_step_end;
	factors_dynctnr								_M_factors;

	work_vec									_M_buf_master, _M_buf_slave;

public:

// functions

	fft_wizard () : _M_len(0), _M_num_steps(0) {}

	void set_params (const factors_dynctnr &__factors, const factors_dyniter &__step_begin, 
		const factors_dyniter &__step_end, step_type __num_steps, index_type __len)
		{
		_M_factors = __factors;

		_M_step_begin = __step_begin;
		_M_step_end = __step_end;
		_M_num_steps = __num_steps;
		_M_len = __len;

		_M_buf_master.resize(__len);
		_M_buf_slave.resize(__len);
		}

	// automatically set parameters
	void set_length (index_type __len)
		{
		factors_byvec f(__len);
        factors_dynctnr fd = f;
		set_params(fd, f->begin(), f->end(), f->num_steps(), __len);
		}

	template <class _ItrSrc>
		work_iter forward_transform (const _ItrSrc &__src_begin)
		{
		work_iter __master_begin = _M_buf_master.begin(), __master_end = _M_buf_master.end();
		work_iter __slave_begin = _M_buf_slave.begin(), __slave_end = _M_buf_slave.end();

		#if defined(_MSC_VER)
			_M_kernel.factorder<_ItrSrc, work_iter, factors_dyniter, true>
				(__src_begin, __master_begin, _M_len, _M_step_begin, _M_num_steps, 1);
		#else
			// For G++, the Super Idiot
			_M_kernel.factorder
				(__src_begin, __master_begin, _M_len, _M_step_begin, _M_num_steps, 1, true);
		#endif


		_M_kernel.transform<work_iter, factors_dyniter, fft_default_traits::forward>(
			__master_begin, __master_end, __slave_begin, __slave_end, _M_step_begin, _M_step_end
			);

		return __master_begin;
		}
	
	template <class _ItrSrc>
		work_iter inverse_transform (const _ItrSrc &__src_begin)
		{
		work_iter __master_begin = _M_buf_master.begin(), __master_end = _M_buf_master.end();
		work_iter __slave_begin = _M_buf_slave.begin(), __slave_end = _M_buf_slave.end();

		sample_type __coef = (sample_type)1 / (sample_type)_M_len;

		#if defined(_MSC_VER)
			_M_kernel.factorder<_ItrSrc, work_iter, factors_dyniter, true>
				(__src_begin, __master_begin, _M_len, _M_step_begin, _M_num_steps, __coef);
		#else
			// For G++, the Super Idiot
			_M_kernel.factorder
				(__src_begin, __master_begin, _M_len, _M_step_begin, _M_num_steps, __coef, true);
		#endif
		
		_M_kernel.transform<work_iter, factors_dyniter, fft_default_traits::inverse>(
			__master_begin, __master_end, __slave_begin, __slave_end, _M_step_begin, _M_step_end
			);
		
		return __master_begin;
		}

	};  // class fft_wizard 

#undef  _SECURE_SCL
#undef  _HAS_ITERATOR_DEBUGGING

_QTL_END

#endif  // ifdef _QTL_fft_H
