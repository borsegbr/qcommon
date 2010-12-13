/**
 * === Bit Library ===
 *  Unit Name:  bac
 *  Author:     Lincoln Yu
 *  Purpose:    Binary arithmetic coding algorithms
 *
 * == History ==        
 *  2006-09-14: Created. [:limited_arithmetic_codec:] class finished.
 *  2006-09-14: [:nonmal_arithmetic_codec:] class (codec with no multiplication) finished.
 *  2006-09-17: [:condswitch_arithmetic_codec:] class (codec with conditional switch) finished.
 *  2006-09-19: Traits reformed.
 *  2007-01-11: Coding scheme used by H.26X (e.g. H.264 CABAC) added.
 * == Comments ==
 *
 */

#if !defined (bac_H)
#define bac_H   200609190043

#include "../all/predef.h"

#include <cassert>


_QTL_BEGIN


/* Arithmetic coding evoluted from a very rough one to a scheme recommended by JPEG2000 */

struct limited_arithmetic_codec_default_N_P
    {
    enum
        {
        _K_N = 16,
        };
    enum
        {
        _K_P = 14,
        };
    };

template <class _NP = limited_arithmetic_codec_default_N_P>
        struct limited_arithmetic_codec_default_traits
    {
    typedef unsigned char       _BitType;       // each bit of output bitstream, [EMBODY]
    typedef unsigned char       _ValType;       // used as bit, [EMBODY]
    typedef unsigned long       _ProbType;      // P bits at most, [EMBODY]
    typedef unsigned long       _ProbMap[2];    // [REF/EMBODY]
    typedef _ValType            _ValOrder[2];   // map from index to val, [REF/EMBODY]
    typedef unsigned long       _AregType;      // N bits at most, [EMBODY]
    typedef unsigned long       _CregType;      // N+P bits at most, [EMBODY]

    typedef void _BitEmitProc(const _BitType &);
    typedef void _BitEatProc(_BitType &);

    void (*bit_emit)(const _BitType &);
    void (*bit_eat)(_BitType &);

    int                         _N;
    int                         _P;
    const static int            _num_vals = 2;
    _ValOrder                   _val_order;

    limited_arithmetic_codec_default_traits ()
        {
        _val_order[0] = 0;
        _val_order[1] = 1;
        _N = _NP::_K_N;
        _P = _NP::_K_P;
        }

    template <class _RegType>
            bool _test_reg_bit (const _RegType &_reg, int _pos)
        {
        return ((_reg & (1<<_pos)) != 0);
        }

    template <class _RegType>
            _RegType & _set_reg_bit (_RegType &_reg, int _pos)
        {
        _reg |= (1<<_pos);
        return _reg;
        }

    template <class _RegType>
            _RegType & _flip_reg_bit (_RegType &_reg, int _pos)
        {
        _reg ^= (1<<_pos);
        return _reg;
        }

    template <class _RegType>
            _RegType & _shl_reg (_RegType &_reg, int _shift)
        {
        _reg <<= _shift;
        return _reg;
        }

    template <class _RegType>
            _RegType & _shr_reg (_RegType &_reg, int _shift)
        {
        _reg >>= _shift;
        return _reg;
        }

    };  // class limited_arithmetic_codec_traits

template <class _Traits = limited_arithmetic_codec_default_traits>
        class limited_arithmetic_codec : public _Traits
    {
    typedef _Traits                         _Mybase;

protected:
    typedef typename _Traits::_BitType      _BitType;
    typedef typename _Traits::_ValType      _ValType;
    typedef typename _Traits::_ProbType     _ProbType;
    typedef typename _Traits::_ProbMap      _ProbMap;
    typedef typename _Traits::_ValOrder     _ValOrder;
    typedef typename _Traits::_AregType     _AregType;
    typedef typename _Traits::_CregType     _CregType;

    typedef typename _Traits::_BitEmitProc  _BitEmitProc;
    typedef typename _Traits::_BitEatProc   _BitEatProc;

public:
    typedef _Traits                         traits;

public:
    limited_arithmetic_codec () : _Mybase()
        {
        }

    virtual ~limited_arithmetic_codec ()
        {
        }

    void set_bit_eat_proc (_BitEatProc *_proc)
        {
        bit_eat = _proc;
        }

    void set_bit_emit_proc (_BitEmitProc *_proc)
        {
        bit_emit = _proc;
        }

    virtual void initialize ()
        {
        _M_C = 0;

        _M_Creg_size = _N+_P;

        _M_A = 0;
        _set_reg_bit<_AregType>(_M_A, _N);

        _M_r = -1;
        _M_b = 0;

        _M_dec_counter = _M_Creg_size;
        }

    virtual void finalize_encode ()
        {
        if (_M_r >= 0)
            {
            bit_emit(0);
            for ( ; _M_r > 0; --_M_r)
                {
                bit_emit(1);
                }
            }
        for (int i = 0; i < _M_Creg_size; ++i)
            {
            _shl_reg<_CregType>(_M_C, 1);
            bit_emit (_test_reg_bit<_CregType>(_M_C, _M_Creg_size)? 1 : 0);
            }
        }
    
    void encode (const _ValType &_val, _ProbMap _probmap, _ProbMap _fmap)
        {
        _CregType _T = _M_A * _probmap[_val];
        _M_C += _M_A * _fmap[_val];
        if (_test_reg_bit<_CregType>(_M_C, _M_Creg_size))
            {
            propagate_carry();
            }
        while (!_test_reg_bit<_CregType>(_T, _M_Creg_size-1))
            {
            _shl_reg<_CregType>(_T, 1);
            renormalize_once();
            }
        _M_A = _shr_reg<_CregType>(_T, _P);
        }

    void decode (_ValType &_res_val, _ProbMap _probmap, _ProbMap _fmap)
        {
        for (; _M_dec_counter > 0; --_M_dec_counter)
            {
            retrieve_bit();
            }

        _CregType _T_A, _T, _T_last;
        _ValType _last_val = _val_order[0];
        _T_last = _M_A * _fmap[_last_val];
        for (int i = 1; i < _num_vals; ++i)
            {
            _ValType _val = _val_order[i];
            _T = _M_A * _fmap[_val];
            if (_M_C < _T)
                {
                _M_C -= _T_last;
                _T_A = _M_A * _probmap[_last_val];
                _res_val = _last_val;
                break;
                }
            _last_val = _val;
            _T_last = _T;
            }
        if (i == _num_vals)
            {
            _M_C -= _T_last;
            _T_A = _M_A * _probmap[_last_val];
            _res_val = _last_val;
            }
        while (!_test_reg_bit<_CregType>(_T_A, _M_Creg_size-1))
            {
            ++_M_b;
            _shl_reg<_CregType>(_T_A, 1);
            retrieve_bit();
            }
        
        
        _M_A = _shr_reg<_CregType>(_T_A, _P);
        
        }

protected:
    void propagate_carry ()
        {
        _flip_reg_bit<_CregType>(_M_C, _M_Creg_size);

        bit_emit(1);
        if (_M_r > 0)
            {
            --_M_r;
            for ( ; _M_r > 0; --_M_r)
                {
                bit_emit(0);
                }
            }
        else
            {
            _M_r = -1;
            }
        }

    void renormalize_once ()
        {
        ++_M_b;
        _shl_reg<_CregType>(_M_C, 1);

        if (_test_reg_bit<_CregType>(_M_C, _M_Creg_size))
            {
            _flip_reg_bit<_CregType>(_M_C, _M_Creg_size);
            if (_M_r < 0)
                {
                bit_emit(1);
                }
            else
                ++_M_r;
            }
        else 
            {
            if (_M_r >= 0)
                {
                bit_emit(0);
                for ( ; _M_r > 0; --_M_r)
                    {
                    bit_emit(1);
                    }
                }
            _M_r = 0;
            }
            
        }

    void retrieve_bit ()
        {
        _BitType _bit;
        bit_eat(_bit);

        _shl_reg<_CregType>(_M_C, 1);
        if (_test_reg_bit<_CregType>(_M_C, _M_Creg_size))
            {
            _flip_reg_bit<_CregType>(_M_C, _M_Creg_size);
            }
        if (_bit)
            _set_reg_bit<_CregType>(_M_C, 0);
        }

    int         _M_r;
    int         _M_b;

    _AregType   _M_A;
    _CregType   _M_C;
    int         _M_Creg_size;


    int         _M_dec_counter;
    };  // class binary_coder


struct nonmul_arithmetic_codec_default_traits
    {
    typedef float                                       _AlphaType;

    typedef limited_arithmetic_codec_default_traits<>   _BaseTraits;

    nonmul_arithmetic_codec_default_traits ()
        {
        _alpha = 0.708f;
        }

    template <class _RegType>
        _RegType & _neg_reg (_RegType & _reg, int len)
        {
        _reg = (1<<len) - _reg;
        return _reg;
        }

protected:
    _AlphaType      _alpha;

    };

template <class _Traits = nonmul_arithmetic_codec_default_traits>
        class nonmul_arithmetic_codec : public _Traits, 
        public limited_arithmetic_codec<typename _Traits::_BaseTraits>
    {
    typedef _Traits     _Mybase;
    typedef _CregType   _MyRegType;

public:
    nonmul_arithmetic_codec () : _Mybase()
        {
        _M_alpha_bin = 0;
        _set_reg_bit(_M_alpha_bin, _N);
        _M_alpha_bin = (_MyRegType)(_Traits::_alpha * _M_alpha_bin);

        }
    
    void refresh (const _MyRegType &_p0 /* [:_N:]-bit value */)
        {
        _MyRegType _p = _p0;
        _MyRegType _p0bar;
        if (_test_reg_bit(_p, _N-1)) 
            {
            _neg_reg<_MyRegType>(_p, _N);
            _M_pbar = (_MyRegType)(_Traits::_alpha * _p);
            _p0bar = _M_alpha_bin - _M_pbar;
            }
        else
            {
            _M_pbar = (_MyRegType)(_Traits::_alpha * _p);
            _p0bar = _M_pbar;
            }
        
        _M_s = (_test_reg_bit(_p0bar, _N-1)) ? 1 : 0;
        }

    virtual void initialize ()
        {
        _M_C = 0;
        _M_Creg_size = _N;

        _M_A = 0;
        _set_reg_bit<_AregType>(_M_A, _N);

        _M_r = -1;
        _M_b = 0;

        _M_dec_counter = _M_Creg_size;
        }

    void encode (const _ValType &_val)
        {
        if (_val == _M_s)
            {
            _M_A -= _M_pbar;
            _M_C += _M_pbar;
            }
        else
            _M_A = _M_pbar;

        if (_test_reg_bit<_MyRegType>(_M_C, _M_Creg_size))
            {
            propagate_carry();
            }
        while (!_test_reg_bit<_MyRegType>(_M_A, _N-1))
            {
            _shl_reg<_MyRegType>(_M_A, 1);
            renormalize_once();
            }
        }

    void decode (_ValType &_res_val)
        {
        for (; _M_dec_counter > 0; --_M_dec_counter)
            {
            retrieve_bit();
            }

        if (_M_C < _M_pbar)
            {
            _res_val = 1 - _M_s;
            _M_A = _M_pbar;
            }
        else
            {
            _res_val = _M_s;
            _M_A -= _M_pbar;
            _M_C -= _M_pbar;
            }

        while (!_test_reg_bit<_MyRegType>(_M_A, _N-1))
            {
            _shl_reg<_MyRegType>(_M_A, 1);
            ++_M_b;
            retrieve_bit();
            }
        }

protected:
    _ValType        _M_s;       // must be bit

    _MyRegType      _M_pbar;
    _MyRegType      _M_alpha_bin;
    };  // class nonmul_arithmetic_coder


template <class _Traits = nonmul_arithmetic_codec_default_traits>
        class condswitch_arithmetic_codec 
        : public nonmul_arithmetic_codec<_Traits>
    {
    typedef _CregType   _MyRegType;

public:
    void encode (const _ValType &_val)
        {
        _ValType    _s;
        _s = _M_s;
        _M_A -= _M_pbar;
        if (_M_A < _M_pbar)
            {
            _s = 1 - _s;
            }
        if (_val == _s) // coding as MPS
            {
            _M_C += _M_pbar;
            }
        else            // coding as LPS
            {
            _M_A = _M_pbar;
            }

        if (_test_reg_bit<_MyRegType>(_M_C, _M_Creg_size))
            {
            propagate_carry();
            }
        while (!_test_reg_bit<_MyRegType>(_M_A, _N-1))
            {
            _shl_reg<_MyRegType>(_M_A, 1);
            renormalize_once();
            }
        }

    void decode (_ValType &_res_val)
        {
        for (; _M_dec_counter > 0; --_M_dec_counter)
            {
            retrieve_bit();
            }

        _ValType    _s;
        _s = _M_s;
        _M_A -= _M_pbar;

        if (_M_A < _M_pbar)
            {
            _s = 1 - _s;
            }
        if (_M_C < _M_pbar)
            {
            _res_val = 1 - _s;
            _M_A = _M_pbar;
            }
        else
            {
            _res_val = _s;
            _M_C -= _M_pbar;
            }

        while (!_test_reg_bit<_MyRegType>(_M_A, _N-1))
            {
            _shl_reg<_MyRegType>(_M_A, 1);
            ++_M_b;
            retrieve_bit();
            }
        }   

    };  // class condswitch_arithmetic_codec


/* binary arithmetic coding used in H.26X */


struct normal_ac_traits
    {
    typedef int Range;

    static Range _K_quarter;
    static Range _K_half;
    static Range _K_full;

#if !defined(PUT_BIT)
#define PUT_BIT(b)  put_bit(b)
    void (*put_bit)(int b);
#endif

#if !defined(GET_BIT)
#define GET_BIT(b)  get_bit(b)
    void (*get_bit)(int b);
#endif
    };

normal_ac_traits::Range normal_ac_traits::_K_quarter = 0x100;
normal_ac_traits::Range normal_ac_traits::_K_half = 0x200;
normal_ac_traits::Range normal_ac_traits::_K_full = 0x400;

template <class _Normal_AC_Traits = normal_ac_traits>
        class normal_ac : public _Normal_AC_Traits
    {
protected:
    typedef typename _Normal_AC_Traits::Range   _Range;

    _Range  _M_range;
    _Range  _M_range_decode;
    _Range  _M_low;

    union
        {
        int     _M_outstanding_count;
        int     _M_scale;
        };

    void put_ac_bits(int b)
        {
        PUT_BIT(b);
        for (;_M_outstanding_count;_M_outstanding_count--)
            {
            PUT_BIT(1-b);
            }
        }

    void encoder_renormalize ()
        {
        while(_M_range < _Normal_AC_Traits::_K_quarter)
            {
            if (_M_low < _Normal_AC_Traits::_K_quarter)
                {
                put_ac_bits(0);
                }
            else if (_M_low < _Normal_AC_Traits::_K_half)
                {
                _M_outstanding_count++;
                _M_low -= _Normal_AC_Traits::_K_quarter;
                }
            else
                {
                put_ac_bits(1);
                _M_low -= _Normal_AC_Traits::_K_half;
                }

            _M_range += _M_range;
            _M_low += _M_low;
            }
        }

public:
    normal_ac ()
        {
        Initialize();
        }

    virtual ~normal_ac ()
        {
        }

    void Initialize ()
        {
        _M_outstanding_count = 0;
        _M_low = 0;
        _M_range = _Normal_AC_Traits::_K_full;
        _M_range_decode = _Normal_AC_Traits::_K_full;
        }

    const _Range & GetRange () const
        {
        return _M_range;
        }

    void encode_mps (_Range _range_lps)
        {
        _M_range -= _range_lps;
        encoder_renormalize();
        }

    void encode_lps (_Range _range_lps)
        {
        _M_low += _M_range - _range_lps;
        _M_range = _range_lps;
        encoder_renormalize();
        }

    void encode_flush ()
        {
        while(_M_low > 0 || _M_low + _M_range < _Normal_AC_Traits::_K_full)
            {
            if (_M_low + _M_low + _M_range > _Normal_AC_Traits::_K_full)
                {   // go up, output 1
                put_ac_bits(1);
                _M_low -= _Normal_AC_Traits::_K_half;
                _M_low += _M_low;
                _M_range += _M_range;
                }
            else
                {   // go down, output 0
                put_ac_bits(0);
                _M_low += _M_low;
                _M_range += _M_range;
                }
            }
        }
    
    int decode (_Range _range_lps)
        {
        int bit;

        _Range _range_mps = _M_range - _range_lps;
        _Range low  = _M_low;
        _Range high = _M_low + _M_range_decode;
        _Range mid = high - (_range_lps<<_M_scale);
        
        int res;

        while(1)
            {
            if (mid <= 0)
                {
                _M_low = mid;
                _M_range_decode = high - mid;
                _M_range = _range_lps;
                res = 0;
                break;
                }
            else if (mid >= _Normal_AC_Traits::_K_full)
                {
                _M_low = low;
                _M_range_decode = mid - low;
                _M_range = _range_mps;
                res = 1;
                break;
                }
            GET_BIT(bit);
            if (bit)
                {
                low -= _Normal_AC_Traits::_K_half;
                mid -= _Normal_AC_Traits::_K_half;
                high -= _Normal_AC_Traits::_K_half;
                low += low;
                mid += mid;
                high += high;
#if 1
                _M_scale++;
#endif
                }
            else
                {
                low += low;
                mid += mid;
                high += high;
#if 1
                _M_scale++;
#endif
                }
            }

        while(_M_range < _Normal_AC_Traits::_K_quarter)
            {
            _M_range += _M_range;
#if 1
            _M_scale--;
#endif
            }

#if 1
        assert(_M_range_decode == _M_range<<_M_scale);
#else
        _Range range = _M_range_decode;
        _M_scale = 0;
        while(range != _M_range)
            {
            range >>= 1;
            _M_scale++;
            }
#endif
        return res;
        }
    };


_QTL_END


#endif  // if !defined (bac_H)