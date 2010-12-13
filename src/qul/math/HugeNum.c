#define _HugeNum_DEBUG

//--------------------------------------------------

#ifdef _HugeNum_DEBUG
#   include <stdio.h>
#   include <assert.h>
#endif

#include <string.h>
#include <malloc.h>

#include "HugeNum.h"

#ifdef _HugeNum_DEBUG
#   define DEBUG_PRINT(pHu)     {char *str; HU_ToHexStr(pHu, &str); printf(#pHu" = %s\n", str); free(str); }
#else
#   define DEBUG_PRINT(pHu)
#endif

void HU_Init (HugeUnsigned *p)
{
    assert(sizeof(uint32_t) == 4);
    p->len = 0;
    p->pNum = 0;
}

void HU_Deinit (HugeUnsigned *p)
{
    if (p->pNum)
    {
        free(p->pNum);
    }
    p->len = 0;
}

#define MAX_UINT32  (0xffffffff)
#define LO(v)       ((v)&0xffff)
#define HI(v)       ((v)>>16)
#define MAX(a,b)    (((a)>(b))?(a):(b))
#define MIN(a,b)	(((a)<(b))?(a):(b))

static void HU_TempPart (HugeUnsigned *part, const HugeUnsigned *a, int hi, int lo)
{
    part->len = hi - lo + 1;
    part->pNum = a->pNum + lo;
}

void HU_Compact (HugeUnsigned *r)
{
    r->pNum = (uint32_t *)realloc(r->pNum, r->len * sizeof(uint32_t));
}

int HU_Cmp(const HugeUnsigned *a, const HugeUnsigned *b)
{
    int i;

    if (a->len > b->len)
    {
        return 1;
    }
    else if (a->len < b->len)
    {
        return -1;
    }

    for (i = a->len - 1; i >= 0; i--)
    {
        if (a->pNum[i] > b->pNum[i])
        {
            return 1;
        }
        else if (a->pNum[i] < b->pNum[i])
        {
            return -1;
        }
    }

    return 0;
}

/* [:r:] and [:a:]/[:b:] can be the same */
int HU_Add (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b)
{
    int i;
    int maxLen = MAX(a->len, b->len);
	int minLen = MIN(a->len, b->len);
    int resLen = maxLen + 1;
    uint32_t carry = 0;

	if (a->len < b->len)
	{
		const HugeUnsigned *t = a;
		a = b;
		b = t;
	}
    if (r->len != resLen)
    {
        r->len = resLen;
        r->pNum = (uint32_t*)realloc(r->pNum, r->len * sizeof(uint32_t));
        r->pNum[maxLen] = 0;
    }
    for (i = 0; i < minLen; i++)
    {
        uint32_t va = a->pNum[i];
        uint32_t vb = b->pNum[i];
        uint32_t vr = va + vb;
        r->pNum[i] = vr + carry;
        carry = (va > MAX_UINT32 - vb || carry == 1 && va + vb == MAX_UINT32)? 1 : 0;
    }
	for (i = minLen; i < maxLen; i++)
	{
        uint32_t va = a->pNum[i];
        r->pNum[i] = va + carry;
        carry = (carry == 1 && va == MAX_UINT32)? 1 : 0;
	}

    if ((r->pNum[maxLen] = carry) == 0)
    {
        r->len = maxLen;
    }

    return (r->len);
}

static int _Sub (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b)
{
    int carry = 0;
    int nz = 0;
    int i;
    int resLen, minLen;
    int rCmp = HU_Cmp(a,b);

    if (rCmp == -1)
    {
        return -1;  /* [:r:] unchanged */
    }
    if (rCmp == 0)
    {
        r->len = 0;
        return 0;   /* The result is 0 */
    }

    resLen = MAX(a->len, b->len);
	minLen = MIN(a->len, b->len);

    for (i = 0; i < minLen; i++)
    {
        uint32_t va = a->pNum[i];
        uint32_t vb = b->pNum[i];
        uint32_t vr = va - vb;
        if ((r->pNum[i] = vr - carry) != 0)
        {
            nz = i;
        }
        carry = (va < vb || carry == 1 && va == vb)? 1 : 0;
    }
	for (i = minLen; i < resLen; i++)
	{
		uint32_t va = a->pNum[i];
		if ((r->pNum[i] = va - carry) != 0)
		{
			nz = i;
		}
		carry = (carry == 1 && va == 0)? 1 : 0;
	}

    nz++;
    r->len = nz;

    return r->len;
}

/* [:r:] and [:a:]/[:b:] can be the same */
int HU_Sub (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b)
{
    int resLen, minLen;
    int carry = 0;
    int nz = 0;
    int i;
    int rCmp = HU_Cmp(a,b);

    if (rCmp == -1)
    {
        return -1;  /* [:r:] unchanged */
    }
    if (rCmp == 0)
    {
        r->len = 0;
        r->pNum = (uint32_t*)realloc(r->pNum, r->len * sizeof(uint32_t));
        return 0;   /* The result is 0 */
    }

    resLen = MAX(a->len, b->len);
    if (r->len != resLen)
    {
        r->len = resLen;
        r->pNum = (uint32_t*)realloc(r->pNum, r->len * sizeof(uint32_t));
    }

	minLen = MIN(a->len, b->len);
    for (i = 0; i < minLen; i++)
    {
        uint32_t va = a->pNum[i];
        uint32_t vb = b->pNum[i];
        uint32_t vr = va - vb;
        if ((r->pNum[i] = vr - carry) != 0)
        {
            nz = i;
        }
        carry = (va < vb || carry == 1 && va == vb)? 1 : 0;
    }
	for (i = minLen; i < resLen; i++)
	{
		uint32_t va = a->pNum[i];
		if ((r->pNum[i] = va - carry) != 0)
		{
			nz = i;
		}
		carry = (carry == 1 && va == 0)? 1 : 0;
	}
    nz++;

    if (r->len != nz)
    {
        r->len = nz;
        r->pNum = (uint32_t*)realloc(r->pNum, r->len * sizeof(uint32_t));
    }
    return (r->len);
}

int HU_Mul (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b)
{
    int i, j, k;
    int aLen = a->len;
    int bLen = b->len;
    int resLen;

	if (aLen == 0 || bLen == 0)
	{
		r->len = 0;
		if (r->pNum)
		{
			free(r->pNum);
			r->pNum = NULL;
		}
		return 0;
	}

	resLen = aLen + bLen;
    if (r->len != resLen)
    {
        r->len = resLen;
        r->pNum = (uint32_t*)realloc(r->pNum, r->len * sizeof(uint32_t));
    }
    memset(r->pNum, 0, resLen * sizeof(uint32_t));

    for (i = 0; i < aLen; i++)
    {
        for (j = 0; j < bLen; j++)
        {
            uint32_t va = a->pNum[i];
            uint32_t vb = b->pNum[j];
            uint32_t valo = LO(va);
            uint32_t vahi = HI(va);
            uint32_t vblo = LO(vb);
            uint32_t vbhi = HI(vb);
            uint32_t lo, hi;
            uint32_t tmp1, tmp2, tmp3, tmp4;

            lo = valo * vblo;
            hi = vahi * vbhi;
            tmp1 = vahi * vblo;
            tmp2 = valo * vbhi;

            tmp3 = LO(tmp1)+LO(tmp2);
            tmp4 = HI(tmp1)+HI(tmp2);

            tmp1 = HI(lo);
            tmp1 += tmp3;

            tmp2 = LO(tmp1);
            lo &= 0xffff;
            lo |= tmp2 << 16;

            hi += tmp4;
            hi += HI(tmp1);

            k = i + j;

            if (r->pNum[k] > MAX_UINT32 - lo)
            {
                hi++;
            }
            if (r->pNum[k+1] > MAX_UINT32 - hi)
            {
                int t = 2;
                while(1)
                {
                    r->pNum[k+t]++;
                    if (r->pNum[k+t] == 0)
                    {
                        t++;
                        continue;
                    }
                    break;
                }
            }
            r->pNum[k+1] += hi;
            r->pNum[k] += lo;
        }
    }
    if (r->pNum[resLen - 1] == 0)
    {
        resLen--;
        r->len = resLen;
    }
    return resLen;
}

int HU_Div (HugeUnsigned *r, HugeUnsigned *a, const HugeUnsigned *b)
{
    int j;
    int hay;
    int step;
    int shLeft = 0;
    int resLen = a->len - b->len + 1;
    int bHigh = b->len - 1;

	if (a->len == 0)
	{
		r->len = 0;
		if (r->pNum)
		{
			free(r->pNum);
			r->pNum = NULL;
		}
		return 0;
	}
	if (b->len == 0)
	{
		return -1;	/* Division by zero */
	}

    HugeUnsigned    t0, t1;
	HugeUnsigned    tp0;

    HU_Init(&t0);
    HU_Init(&t1);

    if (r->len != resLen)
    {
        r->len = resLen;
        r->pNum = (uint32_t*)realloc(r->pNum, r->len * sizeof(uint32_t));
        memset(r->pNum, 0, resLen * sizeof(uint32_t));
    }

    hay = b->pNum[bHigh];
    for (shLeft = 0; !(hay & 0x80000000); shLeft++)
    {
        hay <<= 1;
    }

    for (j = a->len - 1; j >= b->len; j -= step)
    {
        uint32_t        tempt = 0;
        int             oriTp0Len;

        int i = j - b->len + 1;
        HU_TempPart(&tp0, a, j, i);
        
        if (a->pNum[j] > b->pNum[bHigh])
        {
            HugeUnsigned    tp0;
            tempt = a->pNum[j] / (b->pNum[bHigh] + 1);

            oriTp0Len = tp0.len;
            HU_SetU32(&t1, tempt);
            HU_Mul(&t0, b, &t1);
            _Sub(&tp0, &tp0, &t0);

            while (HU_Cmp(&tp0, b) >= 0)
            {
                _Sub(&tp0, &tp0, b);
                tempt++;
            }
            step = oriTp0Len - tp0.len;
            r->pNum[i] = tempt;
        }
        else
        {
            if (HU_Cmp(&tp0, b) >= 0)   // it must be the case that [:a->pNum[j] == b->pNum[bHigh]:]
            {
                tempt = 0;
                oriTp0Len = tp0.len;
                while (_Sub(&tp0, &tp0, b) >= 0)
                {
                    tempt++;
                }
                step = tp0.len - oriTp0Len;
                r->pNum[i] = tempt;
                continue;
            }
            else if (j >= b->len)
            {
                uint32_t sum;
                i--;
                HU_TempPart(&tp0, a, j, i);
                oriTp0Len = tp0.len;

                sum = 0;
                do 
                {
                    tempt = tp0.pNum[tp0.len-1];
                    tempt <<= shLeft;
                    HU_SetU32(&t1, tempt);
                    HU_Mul(&t0, b, &t1);
                    _Sub(&tp0, &tp0, &t0);
                    sum += tempt;
                } while (tp0.len == oriTp0Len);

                tempt = 0;
                while (_Sub(&tp0, &tp0, b) >= 0)
                {
                    tempt++;
                }
                sum += tempt;

                r->pNum[i] = sum;
                step = oriTp0Len - tp0.len;
            }
            else
            {
                break;
            }
        }
    }
	
	a->len = tp0.len;
	HU_Compact(a);

    HU_Deinit(&t0);
    HU_Deinit(&t1);

	if (r->pNum[resLen - 1] == 0)
    {
        resLen--;
        r->len = resLen;
    }

	return r->len;
}

int HU_SetU32 (HugeUnsigned *r, uint32_t v)
{
    if (r->len != 1)
    {
        r->len = 1;
        r->pNum = (uint32_t*)realloc(r->pNum, 1*sizeof(uint32_t));
    }
    r->pNum[0] = v;
    return 1;
}

int HU_ToHexStr (const HugeUnsigned *r, char **ppStr)
{
    int len;
    char *p;
    int i;
	if (r->len == 0)
	{
		*ppStr = (char*)malloc(8+1);
		strcpy(*ppStr, "00000000");
		return 8;
	}
	len = r->len * 8;
    p = *ppStr = (char *)malloc(len+1);
    for (i = r->len - 1; i >= 0; i--, p+=8)
    {
        sprintf(p, "%08x", r->pNum[i]);
    }
    return len;
}

int HU_ToDecStr (const HugeUnsigned *r, char **ppStr)
{
    return 0;
}

#ifdef _HugeNum_DEBUG

int main (void)
{
    uint32_t    a, b;
    HugeUnsigned    ha, hb, hc, hd;
    char *pstra = NULL;
    char *pstrb = NULL;
    char *pstrc = NULL;
    char *pstrd = NULL;
	char *pstrrem = NULL;

    a = 0x1a3d0123;
    b = 0x3e4d790a;
    HU_Init(&ha);
    HU_Init(&hb);
    HU_Init(&hc);
    HU_Init(&hd);

    HU_SetU32(&ha, a);
    HU_SetU32(&hb, b);
	HU_SetU32(&hd, 0x364d5);
    HU_Mul(&hc, &ha, &hb);
	HU_Add(&hc, &hc, &hd);
    HU_ToHexStr(&ha, &pstra);
    HU_ToHexStr(&hb, &pstrb);
    HU_ToHexStr(&hc, &pstrc);
    HU_ToHexStr(&hd, &pstrd);
    printf("0x%s = 0x%s * 0x%s + 0x%s\n", pstrc, pstra, pstrb, pstrd);

    HU_Div(&hd, &hc, &hb);
    HU_ToHexStr(&hd, &pstrd);
	HU_ToHexStr(&hc, &pstrrem);
    printf("0x%s = 0x%s / 0x%s, rem 0x%s\n", pstrd, pstrc, pstrb, pstrrem);

    HU_Deinit(&ha);
    HU_Deinit(&hb);
    HU_Deinit(&hc);
    HU_Deinit(&hd);

    free(pstra);
    free(pstrb);
    free(pstrc);
    free(pstrd);
	free(pstrrem);

    return 0;
}

#endif  /* ifdef _HugeNum_DEBUG */
