#include "../inc/arch.h"

int _start(void)
{
    arch_init();
    for (;;);
}
