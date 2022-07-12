#include <stdint.h>

#include "../../inc/lock.h" 
#include "../../inc/pmm.h"
#include "../../inc/loader.h"

DECLARE_LOCK(liballoc);

int liballoc_lock(void)
{
    LOCK(liballoc);
    return 0;
}

int liballoc_unlock(void)
{
    UNLOCK(liballoc);
    return 0;
}

void *liballoc_alloc(int pages)
{
    return (void *) ((uint64_t) pmm_alloc(pages) + loader_get_hhdm());
}

int liballoc_free(void* ptr, int pages)
{
    pmm_free((uint64_t) ptr, pages);
    return 0;
}
