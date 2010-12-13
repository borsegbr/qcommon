/**
 * === String Library ===
 *  Unit Name:  kmp
 *  Author:     Lincoln Yu
 *  Purpose:    KMP text searching algorithm.
 *  Reference:  Data Structure (in C Programming Language), by Yan Wei-min and Wu Wei-min, 
 *              published by Tsinghua University Press.
 * == History ==        
 *  2005-12-05: Created and debugged.
 *	2007-10-22: Single step parse added, with methods included:
 *				find_onestep(), is_matched(), skip_match(), get_p_ptrn(), set_p_ptrn()
 *				A typical use of repetition variable search with a reading buffer is demonstrated in 
 *				src/_samples/qtl/str/mygrep.cpp.
 *  2008-03-02: Another expression of step-by-step matching, with methods progress() and regress()
 *  2008-03-03: Change the value type of _M_ptrn_len into generic type.
 *				
 * == Comments ==
 *
 */

#ifndef _QTL_kmp_H
#define _QTL_kmp_H

#include "../all/predef.h"

#include <map>

_QTL_BEGIN


template < class _CharCstItr, class _PtnCharCstItr = _CharCstItr, 
    class _Mapper = std::map<_PtnCharCstItr, _PtnCharCstItr>, class _Index = unsigned long >
    class kmp
    {
private:
    _Mapper         _M_nextval;
    _PtnCharCstItr  _M_ptrn_begin;
    _PtnCharCstItr  _M_ptrn_end;
    _PtnCharCstItr  _M_ptrn_scrt;
    _Index          _M_ptrn_len;
    _PtnCharCstItr  _M_p_ptrn;

public:

    void reset ()
        {
        _M_p_ptrn = _M_ptrn_begin;
        }

    bool find (const _CharCstItr &__begin, const _CharCstItr &__end, _CharCstItr &__pos)
        {
        __pos = __begin;
        while ( __pos < __end && _M_p_ptrn < _M_ptrn_end )
            {
            if ( _M_p_ptrn == _M_ptrn_scrt || *__pos == *_M_p_ptrn )
                {
                ++__pos; ++_M_p_ptrn;
                } 
            else 
                {
                _M_p_ptrn = _M_nextval[_M_p_ptrn];
                }
            }
        if ( _M_p_ptrn >= _M_ptrn_end )
            {
            __pos -= _M_ptrn_len;
            return true;
            } 
        else 
            {
            return false;
            }
        }
    
     bool find_onestep (const _CharCstItr &__pos)
        {
        if (_M_p_ptrn == _M_ptrn_scrt || *__pos == *_M_p_ptrn)
            {
            ++_M_p_ptrn;
			return true;
            }
        else
            {
            _M_p_ptrn = _M_nextval[_M_p_ptrn];
            return false;	/* unmatched */
            }
        }

	/* <code comment="step-by-step matching other than find_onestep"> */
	bool progress (const _CharCstItr &__pos)
		{
        if (*__pos == *_M_p_ptrn)
            {
            ++_M_p_ptrn;
			return true;
            }
        else
            {
            _M_p_ptrn = _M_nextval[_M_p_ptrn];
            return false;	/* unmatched, to regress */
            }
		}
	bool regress (const _CharCstItr &__pos)
		{
		find_onestep(__pos);
		}
	/* </code> */


	bool is_matched ()
		{
		return (_M_p_ptrn == _M_ptrn_end);
		}

    void get_p_ptrn (_PtnCharCstItr &__p_ptrn)
        {
        __p_ptrn = _M_p_ptrn;
        }

    void set_p_ptrn (_PtnCharCstItr &__p_ptrn)
        {
        _M_p_ptrn = __p_ptrn;
        }

    void set_ptrn (const _PtnCharCstItr &__begin, const _PtnCharCstItr &__end)
        {
        _M_nextval.clear();

        _M_ptrn_begin = __begin;
        --(_M_ptrn_scrt = __begin);
        _M_ptrn_end = __end;
        _M_ptrn_len = __end - __begin;
        

        _PtnCharCstItr __ci_i = __begin, __ci_j = _M_ptrn_scrt;
        _M_nextval[__begin] = _M_ptrn_scrt;

        _PtnCharCstItr __ci_i_next;
        for ( __ci_i = __begin, ++(__ci_i_next = __ci_i); __ci_i_next < __end; )
            {
            if ( __ci_j == _M_ptrn_scrt || *__ci_i == *__ci_j )
                {
                __ci_i = __ci_i_next, ++__ci_i_next; ++__ci_j;

                #ifdef _KMP_SIMPLE
                    _M_nextval[__ci_i] = __ci_j;
                #else
                    if ( *__ci_i != *__ci_j )
                        {
                        _M_nextval[__ci_i] = __ci_j;
                        } 
                    else 
                        {
                        _M_nextval[__ci_i] = _M_nextval[__ci_j];
                        }
                #endif
                }
            else
                {
                __ci_j = _M_nextval[__ci_j];
                }
            }

        reset();

        }

    };  // class kmp

_QTL_END

#endif  // ifndef _QTL_kmp_H
