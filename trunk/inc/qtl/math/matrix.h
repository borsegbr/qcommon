/**
 * === Math Library ===
 *  Unit Name:  matrix
 *  Author:     Lincoln Yu
 *  Purpose:    A simple implementation of matrix
 *
 * == History ==        
 *  2005-11-12: Created.
 *  2007-01-21: Minor changes. Added several entries for multiplication of matrix, 
 *              i.e. [:product:], [:rmult:], etc.
 *  2007-01-22: Using of namespace [:std:] removed.
 *  2007-06-04: Value setting scheme of [:matrix:] improved. Traits of square matrix used.
 *              Determinant calculation added.
 *  2007-08-15: Add [:explicit:] to some constructors of matrices with one parameter
 *              of pointer type, inverse of squared matrix reorginzed
 *  2008-10-24: move matrix algorithm such as inv, det out to matxalg.h
 * == Comments ==
 *
 */

#ifndef _QTL_matrix_H
#define _QTL_matrix_H

#include <utility>
#include <iostream>
#include <cmath>

#include "../all/predef.h"
#include "matxalg.h"


_QTL_BEGIN

template <class _ValTp, int _RowCnt, int _ColCnt = _RowCnt> 
    class matrix
    {
private:
    typedef matrix<_ValTp, _RowCnt, _ColCnt>    _Myt;

public:
    typedef _ValTp          _ValArr[_ColCnt];
    typedef const _ValTp    _ValCArr[_ColCnt];

    typedef _ValTp          (*_DataPtr)[_ColCnt];
    typedef const _ValTp    (*_DataCPtr)[_ColCnt];

    typedef _ValTp          *_LinearPtr;
    typedef const _ValTp    *_LinearCPtr;

    enum { _K_row_cnt = _RowCnt };
    enum { _K_col_cnt = _ColCnt };

public:
    matrix ()
        {
        }

    matrix (const _ValTp &__e)
        {
        set_values(__e);
        }

    explicit matrix (_LinearCPtr __data)
        {
        set_values(__data);
        }

    explicit matrix (_DataCPtr __data)
        {
        set_values(__data);
        }

    matrix (const _Myt &__copy)
        {
        set_values((_DataCPtr)__copy._M_data);
        }

    _Myt & operator= (const _Myt &__copy)
        {
        set_values((_DataCPtr)__copy._M_data);
        return *this;
        }

    _Myt & operator= (_LinearCPtr __data)
        {
        set_values(__data);
        return *this;
        }

    _Myt & operator= (_DataCPtr __data)
        {
        set_values(__data);
        return *this;
        }

    _ValTp * get_buffer() const
        {
        return (_ValTp *)_M_data;
        }

    void set_value (int __idx, const _ValTp &__e)
        {
        _LinearPtr p = (_LinearPtr)_M_data;
        p[__idx] = __e;
        }

    const _ValTp & get_value (int __idx) const
        {
        _LinearCPtr p = (_LinearCPtr)_M_data;
        return p[__idx];
        }

    void fill (const _ValTp &__e, int __row_begin, int __col_begin, int __row_end, int __col_end)
        {
        for (int __row = __row_begin; __row < __row_end; ++__row)
            for (int __col = __col_begin; __col < __col_end; ++__col)
                _M_data[__row][__col] = __e;        
        }

    void fill (const _ValTp &__e = _ValTp(0))
        {
#if 1
        for (_ValTp *__p = &_M_data[0][0]; __p != &_M_data[_RowCnt][_ColCnt]; ++__p)
            {
            *__p = __e;
            }
#else
        for (int __row = 0; __row < _RowCnt; ++__row)
            for (int __col = 0; __col < _ColCnt; ++__col)
                _M_data[__row][__col] = __e;
#endif
        }

    void set_row_values (_LinearCPtr __data, int __row, int __col_begin = 0, int __col_end = _ColCnt)
        {
#if 1
        memcpy(&_M_data[__row][__col_begin], __data, (__col_end - __col_begin) * sizeof(_ValTp));
#else
        for (int __col = __col_begin; __col < __col_end; ++__col)
            _M_data[__row][__col] = *__data++;
#endif
        }

    void set_col_values (_LinearCPtr __data, int __col, int __row_begin = 0, int __row_end = _RowCnt)
        {
        for (int __row = __row_begin; __row < __row_end; ++__row)
            _M_data[__row][__col] = *__data++;
        }

    void set_values (_LinearCPtr __data, int __row_begin, int __col_begin, int __row_end, int __col_end)
        {
        for (int __row = __row_begin; __row < __row_end; ++__row)
            for (int __col = __col_begin; __col < __col_end; ++__col)
                _M_data[__row][__col] = *__data++;
        }

    void set_values (_LinearCPtr __data)
        {
        memcpy(_M_data, __data, sizeof(_M_data));
        }

    void set_values (_DataCPtr __data, int __row_begin, int __col_begin, int __row_end, int __col_end)
        {
        set_values((_LinearCPtr)__data, __row_begin, __col_begin, __row_end, __col_end);
        }

    void set_values (_DataCPtr __data)
        {
#if 1
        memcpy(_M_data, __data, sizeof(_M_data));
#else
        for (int __row = 0; __row < _RowCnt; ++__row)
            for (int __col = 0; __col < _ColCnt; ++__col)
                _M_data[__row][__col] = __data[__row][__col];
#endif
        }

    template <class _Matrix>
            void set_values (const _Matrix &__other, int __row_begin, int __col_begin, int __row_end, int __col_end)
        {
        set_values(__other.get_buffer(), __row_begin, __col_begin, __row_end, __col_end);
        }

    template <class _Matrix>
            void set_values (const _Matrix &__other)
        {
        set_values((_LinearCPtr)__other.get_buffer(), 0, 0, _Matrix::_K_row_cnt, _Matrix::_K_col_cnt);
        }

    void swap_row (int __r1, int __r2)
        {
        _ValTp __temp;
        for (int __col = 0; __col < _ColCnt; __col++)
            {
            __temp = _M_data[__r1][__col];
            _M_data[__r1][__col] = _M_data[__r2][__col];
            _M_data[__r2][__col] = __temp;
            }
        }

    void swap_col (int __c1, int __c2)
        {
        _ValTp __temp;
        for (int __row = 0; __row < _RowCnt; __row++)
            {
            __temp = _M_data[__row][__c1];
            _M_data[__row][__c1] = _M_data[__row][__c2];
            _M_data[__row][__c2] = __temp;
            }
        }

    _ValCArr & operator[] (int __index) const
        {
        return _M_data[__index];
        }

    _ValArr & operator[] (int __index)
        {
        return _M_data[__index];
        }

protected:
    _ValTp      _M_data[_RowCnt][_ColCnt];

    };  /* class matrix */

template <class _ValTp>
    struct num_abs
    {
    _ValTp operator() (const _ValTp __val)
        {
        return fabs(__val);
        }
    };

template <class _ValTp>
    struct square_matrix_traits
    {
    typedef num_abs<_ValTp>     _Abs;
    };

template <class _ValTp, int _Order, class _Traits = square_matrix_traits<_ValTp> >
    class square_matrix : public matrix<_ValTp, _Order, _Order>
    {
protected:
    typedef typename _Traits::_Abs   _Abs;

private:
    typedef matrix<_ValTp, _Order, _Order>          _Mybase;
    typedef square_matrix<_ValTp, _Order, _Traits>  _Myt;

public:
    square_matrix () : _Mybase()
        {
        unitize();
        }

    square_matrix (const _Myt &__copy) : _Mybase(__copy)
        {
        }

    explicit square_matrix (typename _Mybase::_LinearCPtr __data) 
        : _Mybase(__data)
        {
        }

    explicit square_matrix (typename _Mybase::_DataCPtr __data) 
        : _Mybase(__data)
        {
        }

    void unitize ()
        {
        for (int i = 0; i < _Order; i++)
            {
            _Mybase::_M_data[i][i] = _ValTp(1);
            for (int j = 0; j < i; j++)
                _Mybase::_M_data[i][j] = _Mybase::_M_data[j][i] = _ValTp(0);
            }
        }

    void product (const _Myt &__left, const _Myt &__right)
        {
        for (int __row = 0; __row < _Order; __row++)
            for (int __col = 0; __col < _Order; __col++)
                {
                _ValTp __sum = 0;
                for (int i = 0; i < _Order; i++)
                    __sum += __left._Mybase::_M_data[__row][i] * __right._Mybase::_M_data[i][__col];
                _Mybase::_M_data[__row][__col] = __sum;
                }
        }

    void lmult (const _Myt &__right, _Myt &__result) const
        {
        __result.product(*this, __right);
        }

    void rmult (const _Myt &__left, _Myt &__result) const
        {
        __result.product(__left, *this);
        }

    _Myt operator* (const _Myt &__right) const
        {
        _Myt __result;
        lmult(__right, __result);
        return __result;
        }

    _Myt & operator*= (const _Myt &__right)
        {
        _Myt __temp;
        lmult(__right, __temp);
        *this = __temp;
        return (*this);
        }

    void transpose ()
        // Note:    no conjugation
        {
        for (int __row = 0; __row < _Order - 1; __row++)
            for (int __col = __row + 1; __col < _Order; __col++)
                std::swap<_ValTp>(_Mybase::_M_data[__row][__col], _Mybase::_M_data[__col][__row]);
        }

    void det (_ValTp &__det)
        {
        int     __row_order[_Order];
        _ValTp  __buf[_Order][_Order];
        memcpy(__buf, _Mybase::_M_data, _Order * _Order * sizeof(_ValTp));
        __det = (_ValTp)1;

        square_matrix_det<_ValTp>   matxdet;
        matxdet(__det, (_ValTp*)__buf, __row_order, _Order);
        }

    bool inverse (_ValTp &__det)
        /* Jordan */
        {
        int ip[_Order];
        square_matrix_inv<_ValTp, int, int *, _ValTp *, _Abs>   minv;
        return minv((_ValTp*)_Mybase::_M_data, __det, ip, _Order, _M_abs);
        }

    bool inverse ()
        /* Jordan */
        {
        int ip[_Order];
        square_matrix_inv<_ValTp, int, int *, _ValTp *, _Abs>   minv;
        return minv((_ValTp*)_Mybase::_M_data, ip, _Order, _M_abs);
        }

protected:
    _Abs    _M_abs;

    };  // class square_matrix


template <class _ValTp, int _RowCnt, int _ColCnt>
        std::ostream & operator<< (std::ostream &__out, matrix<_ValTp, _RowCnt, _ColCnt> __matrix)
    {
    __out << "[";
    int __row, __col;
 for (__row = 0; __row < _RowCnt; __row++)
        {
  for (__col = 0; __col < _ColCnt - 1; __col++)
            __out << __matrix[__row][__col] << ", ";

        if (__row < _RowCnt - 1)
            __out << __matrix[__row][__col] << "; ";
        else 
            __out << __matrix[__row][__col];
        }
    __out << "]";
    return __out;
    }

_QTL_END


#endif  // ifndef _QTL_matrix_H 
