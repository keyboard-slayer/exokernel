#include "../inc/arch.h"
#include "../inc/loader.h"

int _start(void)
{
    arch_init();
    for (;;);
}
