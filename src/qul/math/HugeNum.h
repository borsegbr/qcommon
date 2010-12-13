#ifndef _HugeNum_H
#define _HugeNum_H

typedef unsigned long   uint32_t;

typedef struct _HugeUnsigned
{
    uint32_t    *pNum;
    int         len;
} HugeUnsigned;

void HU_Init (HugeUnsigned *p);
void HU_Deinit (HugeUnsigned *p);
void HU_Compact (HugeUnsigned *r);
int HU_Cmp (const HugeUnsigned *a, const HugeUnsigned *b);
int HU_Add (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b);
int HU_Sub (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b);
int HU_Mul (HugeUnsigned *r, const HugeUnsigned *a, const HugeUnsigned *b);
int HU_Div (HugeUnsigned *r, HugeUnsigned *a, const HugeUnsigned *b);
int HU_SetU32 (HugeUnsigned *r, uint32_t v);
int HU_ToHexStr (const HugeUnsigned *r, char **ppStr);
int HU_ToDecStr (const HugeUnsigned *r, char **ppStr);


#endif  /* ifndef _HugeNum_H */

