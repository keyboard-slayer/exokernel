#include <unistd.h>
#include <lock.h>

#ifndef __kernel__

#define PAGE_SIZE 4096

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
    return (void *) syscall(SYS_ALLOC, pages * PAGE_SIZE);
}

int liballoc_free(void *ptr, int pages)
{
    syscall(SYS_FREE, (uint64_t) ptr, pages * PAGE_SIZE);
    return 0;
}

#endif /* !__kernel__ */
