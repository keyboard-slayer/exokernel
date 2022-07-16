#include "../inc/syscall.h"

int _start(void)
{
    syscall(SYS_LOG, (uintptr_t) "Hello, World !\n");
    for (;;);
    __builtin_unreachable();
}
