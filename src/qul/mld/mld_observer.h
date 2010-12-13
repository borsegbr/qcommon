#ifndef _mld_observer_H
#define _mld_observer_H

#include "mld_priv.h"


int mld_observer_Init(void);
int mld_observer_Deinit(void);
int mld_observer_Register(void *pAddr, U32 nSize);
int mld_observer_Unregister(void *pAddr);

int mld_observer_SnapshotCreate(void **ppSnapshot);
int mld_observer_SnapshotVisit(void *p, void **ppAddr, U32 *pSize, void **ppNext);
int mld_observer_SnapshotDestroy(void *pSnapshot);

#endif  /* ifndef _mld_observer_H */
