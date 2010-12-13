#ifndef _mld_memoper_H
#define _mld_memoper_H

#include "mld_priv.h"

void   mld_memoper_Init(U32Ptr pPool, U32 nPoolSize);
void   mld_memoper_Deinit(void);
U32Ptr mld_memoper_Alloc(U32 nSize);    /* size in 32-byte */
U32Ptr mld_memoper_Realloc(U32Ptr p, U32 nSize);
int    mld_memoper_Free(U32Ptr p);

#endif  /* ifndef _mld_memoper_H */

