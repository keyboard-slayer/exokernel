#include "../inc/arch.h"
#include "../inc/loader.h"
#include "../inc/lock.h"
#include "../inc/logging.h"

DECLARE_LOCK(lock);

int _start(void)
{
    arch_init();
    for (;;);

    __builtin_unreachable();
}
