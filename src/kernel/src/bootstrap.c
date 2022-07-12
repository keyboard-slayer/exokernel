#include "../inc/arch.h"
#include "../inc/pmm.h"

int _start(void)
{
    arch_init();
    pmm_init();
    for (;;);

    __builtin_unreachable();
}
