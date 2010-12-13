/**
 * === Standard Algorithm Library ===
 *  Unit Name:  segs
 *  Author:     Lincoln Yu
 *  Purpose:    Segment manipulation. Usually for segmented bit series
 * == History ==        
 *  2005-12-07: Created.
 * == Comments ==
 *
 */

#ifndef _QTL_segs_H
#define _QTL_segs_H

#include "../all/predef.h"

_QTL_BEGIN

struct ulong_copier
{
    void operator() (unsigned long *__src_itr, size_t __src_bit_begin, size_t __src_bit_end,
            unsigned long *__dst_itr, size_t __dst_bit_begin)
        {
        unsigned long __temp = *__src_itr;
        unsigned long __mask = 0xffffffff;

        size_t __len = __src_bit_end - __src_bit_begin;
        size_t __dst_bit_end = __dst_bit_begin + __len;
        __mask >>= (32 - __len);
        __mask <<= (32 - __dst_bit_end);
        __temp >>= (32 - __src_bit_end);
        __temp <<= (32 - __dst_bit_end);
        __temp &= __mask;
        __mask = ~__mask;
        *__dst_itr &= __mask;
        *__dst_itr |= __temp;
        }
    };


template <class _SrcIter, size_t _SrcSize, class _DstIter = _SrcIter, size_t _DstSize = _SrcSize, 
    class _Segcopier = ulong_copier >
struct segs_copier
    {
    void operator() (const _SrcIter &__src_itr_first, size_t __src_bit_begin, 
            const _SrcIter &__src_itr_last, size_t __src_bit_end, 
            const _DstIter &__dst_itr_first, size_t __dst_bit_begin, 
            _Segcopier __segcopier = _Segcopier() )
        {
        _SrcIter __src_itr = __src_itr_first;
        _DstIter __dst_itr = __dst_itr_first;
        size_t __src_bitp = __src_bit_begin;
        size_t __dst_bitp = __dst_bit_begin;

        size_t __src_seg_rest = (__src_itr == __src_itr_last)? 
            (__src_bit_end - __src_bit_begin) : (_SrcSize - __src_bit_begin);
        size_t __dst_seg_rest = _DstSize - __dst_bit_begin;

        size_t __src_bitpend = __src_bitp;
        size_t __dst_bitpend = __dst_bitp;

        while (true)
            {
            if(__src_seg_rest > __dst_seg_rest)
                {
                __src_bitpend += __dst_seg_rest;
                __segcopier(__src_itr, __src_bitp, __src_bitpend, __dst_itr, __dst_bitp);

                __src_bitp = __src_bitpend;
                __src_seg_rest -= __dst_seg_rest;

                ++__dst_itr; 
                __dst_bitpend = __dst_bitp = 0;
                __dst_seg_rest = _DstSize;

                }
            else { 
                if (__src_seg_rest < __dst_seg_rest)
                    {
                    __dst_bitpend += __src_seg_rest;
                    __segcopier(__src_itr, __src_bitp, __src_bitp + __src_seg_rest, __dst_itr, __dst_bitp);

                    if(__src_itr == __src_itr_last)
                        break;      // finished

                    __dst_bitp = __dst_bitpend;
                    __dst_seg_rest -= __src_seg_rest;

                    }
                else
                    {
                    __segcopier(__src_itr, __src_bitp, __src_bitp + __src_seg_rest, __dst_itr, 
                        __dst_bitp + __dst_seg_rest);

                    if(__src_itr == __src_itr_last)
                        break;      // finished

                    __dst_bitp = __dst_bitpend = 0;
                    ++__dst_itr; 
                    __dst_seg_rest = _DstSize;

                    }

                    ++__src_itr;
                    __src_bitpend = __src_bitp = 0;
                    __src_seg_rest = (__src_itr == __src_itr_last)? __src_bit_end : _SrcSize;

                }
            }
        }
    };  // struct segs_copier


_QTL_END

#endif  // ifndef _QTL_segs_H
