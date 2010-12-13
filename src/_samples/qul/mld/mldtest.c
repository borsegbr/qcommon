#include "mld.h"

typedef unsigned long   U32;
typedef U32 *           U32Ptr;

#define MEMPOOL_SIZE    (16*1024)
static U32 g_MemPool[MEMPOOL_SIZE];

int main(void)
{
    U32Ptr p[8];
    void *pSnapshot1;
    void *pSnapshot2;
    void *pSnapshot3;

    mld_Init(g_MemPool, MEMPOOL_SIZE, MLD_MODE_ENABLE_OBSERVER);
    mld_SnapshotCreate(&pSnapshot1);
    p[0] = mld_Alloc(50);
    printf("p[0] = %d\n", p[0] - g_MemPool);
    p[1] = mld_Alloc(100);
    printf("p[1] = %d\n", p[1] - g_MemPool);
    p[2] = mld_Alloc(200);
    printf("p[2] = %d\n", p[2] - g_MemPool);
    mld_SnapshotCreate(&pSnapshot2);
    printf("freeing p[1] returns %d\n", mld_Free(p[1]));
    p[0] = mld_Realloc(p[0], 120);
    printf("p[0] = %d\n", p[0] - g_MemPool);
    p[1] = mld_Alloc(30);
    printf("p[1] = %d\n", p[1] - g_MemPool);
    mld_SnapshotCreate(&pSnapshot3);

    mld_SnapshotDisplay(pSnapshot1, "snapshot1");
    mld_SnapshotDisplay(pSnapshot2, "snapshot2");
    mld_SnapshotDisplay(pSnapshot3, "snapshot3");

    mld_SnapshotDestroy(pSnapshot1);
    mld_SnapshotDestroy(pSnapshot2);
    mld_SnapshotDestroy(pSnapshot3);

    mld_Deinit();

    return 0;
}
