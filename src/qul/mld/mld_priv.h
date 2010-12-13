#ifndef _mld_priv_H
#define _mld_priv_H

#include "mld.h"

#define USE_STD_C

#ifdef USE_STD_C
#   include <stdlib.h>
#   define sys_malloc   malloc
#   define sys_realloc  realloc
#   define sys_free     free
#endif


#ifndef NULL
#   define NULL    (0)
#endif

typedef unsigned long   U32;
typedef U32 *           U32Ptr;
typedef int             BOOL;

enum
{
    FALSE = 0,
    TRUE = (1==1)
};



#endif  /* ifndef _mld_priv_H */

