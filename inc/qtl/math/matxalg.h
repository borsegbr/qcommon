/**
 * === Math Library ===
 *  Unit Name:  matrix
 *  Author:     Lincoln Yu
 *  Purpose:    A simple implementation of matrix
 *
 * == History ==        
 *  2008-10-24: Created by moving algorithms from matrix.h
 *  2008-10-28: Bugs in GJE (leader elements picking, etc.) fixed.
 * == Comments ==
 *
 */

#ifndef _matxalg_H
#define _matxalg_H


_QTL_BEGIN

/*
 * get the determinant of a matrix
 */
template <class _ValTp = double, class _Int = int, 
    class _MTp = _ValTp *, class _OrderTbl = _Int *>
class square_matrix_det 
{
public:
    void operator()(_ValTp &detval, _MTp mbuf, _OrderTbl ip, _Int n)
    {
        det(detval, mbuf, ip, n);
    }

    static void det(_ValTp &detval, _MTp mbuf, _OrderTbl ip, _Int n)
    {
        detval = (_ValTp)1;
        for (_Int k = 0; k < n; k++)
            ip[k] = k;
        for (_Int k = 0; k < n; k++)
        {
            _Int rk = ip[k];
            if (mbuf[rk * n + k] == (_ValTp)0)
            {
                for (_Int i = k + 1; i < n; i++)
                {
                    _Int ri = ip[i];
                    if (mbuf[ri * n + k] != (_ValTp)0)
                    {
                        std::swap<_Int>(ip[i], ip[k]);
                        detval = -detval;
                        break;
                    }
                }
                rk = ip[k];
                if (mbuf[rk * n + k] == (_ValTp)0)
                {
                    detval = (_ValTp)0;
                    return;
                }
            }
            detval *= mbuf[rk * n + k];
            _ValTp kk = (_ValTp)1 / mbuf[rk * n + k];
            for (_Int i = k + 1; i < n; i++)
            {
                _Int ri = ip[i];
                _ValTp  __tmp = kk * mbuf[ri * n + k];
                for (_Int j = k + 1; j < n; j++)
                {
                    mbuf[ri * n + j] -= mbuf[rk * n + j] * __tmp;
                }
            }
        }
    }
};  /* class square_matrix_det */

template <class _ValTp = double, class _Int = int, 
    class _OrderTbl = _Int *, class _MTp = _ValTp *, 
    class _Abs = _ValTp (*)(_ValTp)>
class square_matrix_inv
{
protected:
    static void swap_row(_MTp buf, _Int a, _Int b, _Int n)
    {
        for (_Int i = 0; i < n; i++)
        {
            _ValTp tmp = buf[a * n + i];
            buf[a * n + i] = buf[b * n + i];
            buf[b * n + i] = tmp;
        }
    }
    static void swap_col(_MTp buf, _Int a, _Int b, _Int n)
    {
        for (_Int i = 0; i < n; i++)
        {
            _ValTp tmp = buf[i * n + a];
            buf[i * n + a] = buf[i * n + b];
            buf[i * n + b] = tmp;
        }
    }
public:
    bool operator()(_MTp buf, _OrderTbl ip, _Int n, _Abs absfunc)
    {
        return inv(buf, ip, n, absfunc);
    }

    bool operator()(_MTp buf, _ValTp &retval, _OrderTbl ip, _Int n, _Abs absfunc)
    {
        return inv(buf, retval, ip, n, absfunc);
    }

    /*
     *
     * the processed after (k-1)-th transforming:
     *
     * [ 1         t(1)               ]
     * [   1   0    .                 ]
     * [     .      .                 ]
     * [       .    .         *       ]
     * [         . t(k-1)             ]
     * [           t(k)               ]
     * [           t(k+1)             ]
     * [    0       .         *       ]
     * [            .                 ]
     * [            .                 ]
     * [           t(n)               ]
     *
     * the transforming of the k-th stage should be:
     *
     * [ 1         t(1)/t(k)               ]
     * [   1        .                      ]
     * [     .      .                      ]
     * [       .    .                      ]
     * [         . t(k-1)/t(k)             ]
     * [           1/t(k)                  ]
     * [           t(k+1)/t(k)  1          ]
     * [            .             1        ]
     * [            .               .      ]
     * [            .                 .    ]
     * [           t(n)/t(k)            1  ]
     *
     * the combined transforming matrix up to now is one with 1s on 
     * its diagonal and zeros on all columns but the first k columns
     * in the remaining part. 
     *
     * it turns the k-th col of the matrix being processed to zero 
     * except for the elements on its k-th row (which is turned to 1)
     * so this line. So the k-th matrix being processed retains the 
     * property that the first k columns are all zeros except for the 1s
     * on its diagonal.
     * the effect of it on the combined transforming matrix is no more
     * than add a single k-th line to it and modified values of elements
     * left to the (k-1)-th column.
     *
     * So, we can see, in the sense of information representation, a single 
     * matrix is enough to hold each stage of the process, with first 
     * (k-1) columns filled with the the ones of the combined 
     * transforming matrix till the (k-1)-th stage, and the rest of it
     * with the ones of the matrix under process.
     *
     * one more notice:
     * when performing row swapping during the process, the swapping is 
     * also imposed on the matrix being processed, it is as if the matrix
     * being processed was such line swapped before being processed
     * to counteract this unwanted
     * side effect, it is obvious we need to right multiply the swapping
     * matrix to the transforming matrix to produce the final inverse matrix
     * (the order of the swapping series is very important)
     */
    static bool inv(_MTp buf, _OrderTbl ip, _Int n, _Abs absfunc)
    {
        _ValTp tmp, tmp2;

        for (int k = 0; k < n; k++)
        {
            tmp = absfunc(buf[k * n + k]);
            ip[k] = k;
            for (int i = k + 1; i < n; i++)
            {
                tmp2 = absfunc(buf[i * n + k]);
                if (tmp < tmp2)
                {
                    tmp = tmp2;
                    ip[k] = i;
                }
            }
            tmp = buf[ip[k] * n + k];     // c0
            if (tmp == 0)
                return false;       // singular matrix
            if (ip[k] != k)
            {
                swap_row(buf, k, ip[k], n);
            }
            buf[k * n + k] = (_ValTp)1 / tmp;
            for (int i = 0; i < k; i++)
            {
                buf[i * n + k] *= -buf[k * n + k];
                for (int j = 0; j < k; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
                for (int j = k + 1; j < n; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
            }
            for (int i = k + 1; i < n; i++)
            {
                buf[i * n + k] *= -buf[k * n + k];
                for (int j = 0; j < k; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
                for (int j = k + 1; j < n; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
            }
            for (int j = 0; j < k; j++)
                buf[k * n + j] *= buf[k * n + k];
            for (int j = k + 1; j < n; j++)
                buf[k * n + j] *= buf[k * n + k];

        }

        for (int k = n - 2; k >= 0; k--)
            if (ip[k] != k)
                swap_col(buf, k, ip[k], n);
        return true;
    }

    static bool inv(_MTp buf, _ValTp &detval, _OrderTbl ip, _Int n, _Abs absfunc)
    {
        detval = 1;
        _ValTp tmp, tmp2;
        for (int k = 0; k < n; k++)
        {
            tmp = absfunc(buf[k * n + k]);
            ip[k] = k;
            for (int i = k + 1; i < n; i++)
            {
                tmp2 = absfunc(buf[i * n + k]);
                if (tmp < tmp2)
                {
                    tmp = tmp2;
                    ip[k] = i;
                }
            }
            tmp = buf[ip[k] * n + k];  // c0
            if (tmp == 0)
                return false;       // singular matrix
            if (ip[k] != k)
            {
                swap_row(buf, k, ip[k], n);
                detval = -detval;
            }
            detval *= tmp;
            buf[k * n + k] = _ValTp(1) / tmp;
            for (int i = 0; i < k; i++)
            {
                buf[i * n + k] *= -buf[k * n + k];
                for (int j = 0; j < k; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
                for (int j = k + 1; j < n; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
            }
            for (int i = k + 1; i < n; i++)
            {
                buf[i * n + k] *= -buf[k * n + k];
                for (int j = 0; j < k; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
                for (int j = k + 1; j < n; j++)
                    buf[i * n + j] += buf[i * n + k] * buf[k * n + j];
            }
            for (int j = 0; j < k; j++)
                buf[k * n + j] *= buf[k * n + k];
            for (int j = k + 1; j < n; j++)
                buf[k * n + j] *= buf[k * n + k];
        }
        for (int k = n - 2; k >= 0; k--)
            if (ip[k] != k)
                swap_col(buf, k, ip[k], n);
        return true;
    }

};  /* class square_matrix_inv */

template <class _ValTp = double, class _Int = int>
struct MoutSample
{
    _ValTp  *buf;
    _Int    n;
    MoutSample(_ValTp *__buf = 0, _Int __n = 0)
    {
        buf = __buf;
        n = __n;
    }
    void submul(_Int r1, _Int r2, _ValTp v)
    {
        for (int i = 0; i < n; i++)
        {
            buf[r1*n+i] -= buf[r2*n+i] * v; 
        }
    }
    void div(_Int r, _ValTp v)
    {
        for (int i = 0; i < n; i++)
        {
            buf[r*n+i] /= v; 
        }
    }
    void swaprow(_Int a, _Int b)
    {
        for (_Int i = 0; i < n; i++)
        {
            _ValTp tmp;
            tmp = buf[a*n+i];
            buf[a*n+i] = buf[b*n+i];
            buf[b*n+i] = tmp;
        }
    }
};

template <class _ValTp = double, class _Int = int, 
    class _MoutTp = MoutSample<_ValTp, _Int>, class _MTp = _ValTp *, 
    class _OrderTbl = _Int *, class _Abs = _ValTp (*)(_ValTp)>
class square_matrix_gje
{
public:
    bool operator()(_MTp bufin, _MoutTp mout, _OrderTbl ipf, 
        _OrderTbl ipb, _Int n, _Abs absfunc)
    {
        init(ipf, ipb, n);
        if (!dojob(bufin, mout, ipf, ipb, n, absfunc))
            return false;
        reorder(mout, ipf, ipb, n);
        return true;
    }

    static void init(_OrderTbl ipf, _OrderTbl ipb, _Int n)
    {
        for (_Int i = 0; i < n; i++)
        {
            ipf[i] = ipb[i] = i;
        }
    }

    /*
     * pre : mout has been initialized to be an identical matrix
     *       ipf and ipb are initialized
     * post: both bufin and mout are modified after process,
     *       mout carries the result without permuation
     */
    static bool dojob(_MTp bufin, _MoutTp mout, _OrderTbl ipf, 
        _OrderTbl ipb, _Int n, _Abs absfunc)
    {
        _Int i,j,k,r;
        _ValTp currLdr;

        for (i = 0; i < n; i++)
        {
            ipf[i] = i;
            ipb[i] = i;
        }

        for (i = 0; i < n - 1; i++)
        {
            _ValTp maxLdr;
            _Int pj = i;

            // find the biggest row leader
            maxLdr = absfunc(bufin[ipf[i]*n + i]);

            for (j = i + 1; j < n; j++)
            {
                currLdr = absfunc(bufin[ipf[j]*n + i]);
                if (maxLdr < currLdr)
                {
                    pj = j;
                    maxLdr = currLdr;
                }
            }

            if (maxLdr == 0)
            {   // error, singular matrix
                // TODO: further improvement on handling such case
                return false;
            }
            /* virtually swap these two lines */
            r = ipf[pj];
            ipf[pj] = ipf[i];
            ipf[i] = r;
            // i --> ipf[i]
            // pj --> ipf[pj]

            ipb[ipf[i]] = i;
            ipb[ipf[pj]] = pj;
            mout.div(r, bufin[r*n + i]);

            for (k = i + 1; k < n; k++)
            {
                bufin[r*n + k] /= bufin[r*n + i];
            }
            bufin[r*n + i] = 1;

            for (j = i + 1; j < n; j++)
            {
                mout.submul(ipf[j], r, bufin[ipf[j]*n + i]);
                for (k = i + 1; k < n; k++)
                {
                    bufin[ipf[j]*n + k] -= bufin[ipf[j]*n + i] * bufin[r*n + k];
                }
                bufin[ipf[j]*n + i] = 0;
            }
        }
        currLdr = bufin[ipf[n-1]*n + (n-1)];
        if (currLdr == 0)
        {
            return false;
        }
        mout.div(ipf[n-1], currLdr);
        bufin[ipf[n-1]*n + (n-1)] = 1;
        for (i = n - 2; i >= 0; i--)
        {
            r = ipf[i];
            for (j = i + 1; j < n; j++)
            {
                mout.submul(r, ipf[j], bufin[r*n + j]);
                bufin[r*n + j] = 0;
            }
        }
        return true;
    }
    /*
     * post: ipf and ipb are not guaranteed to be valid after use
     */
    static void reorder(_MoutTp mout, _OrderTbl ipf, 
        _OrderTbl ipb, _Int n)
    {
        for (_Int i = 0; i < n - 1; i++)
        {
            if (ipf[i] != i)
            {
                // index  ->  row
                // i         ipf[i]
                // ipb[i]      i
                // bufin is senseless
                // if you want to put it right
                // just assign it to identical
                //swap_row(bufin, i, ipf[i], n);
                mout.swaprow(i, ipf[i]);
                // index  ->  row
                // i           i
                // ipb[i]    ipf[i]
                ipf[ipb[i]] = ipf[i];
                // unnecessary
                //ipb[ipf[i]] = ipb[i];
                //ip[i] = i;
                //ipf[i] = i;
            }
        }
    }
};  /* class square_matrix_det */

_QTL_END

#endif /* ifndef _matxalg_H */
