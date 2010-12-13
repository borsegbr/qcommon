#ifndef _mld_H
#define _mld_H


enum
{
    MLD_RES_SUCCESS = 0,
    MLD_RES_OBSERVER_DISABLED = -1,
    MLD_RES_POINTER_ALREADY_REGISTERED = -2,    // possibly caused by memory management routine error
    MLD_RES_NOT_FOUND = -3,
    MLD_RES_NULL_PTR = -4,
    MLD_RES_DUPLICATED_ADDR = -5,
};

enum
{
    MLD_MODE_DISABLE_OBSERVER = 0,
    MLD_MODE_ENABLE_OBSERVER = 1,
};


void   mld_Init(unsigned long *pPool, unsigned long nPoolSize, int nMode);
void   mld_Deinit(void);
void * mld_Alloc(unsigned long nSize);  /* size in 32-byte */
void * mld_Realloc(void *p, unsigned long nSize);
int    mld_Free(void *p);


int    mld_GetRecentError(void);
int    mld_SnapshotCreate(void **ppSnapshot);
int    mld_SnapshotDestroy(void *pSnapshot);
int    mld_SnapshotCompare(void *pSnapshot1, void *pSnapshot2);
void   mld_SnapshotDisplay(void *pSnapshot, char *pszName);

#endif  /* ifndef _mld_H */
