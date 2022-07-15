#include "../inc/com.h"
#include "../inc/syscall.h"


void timer_handler(void)
{
    com_puts("Tick !\n");
    return;
}

int _start(void)
{
    syscall(SYS_REG_HANDLER, (uint64_t) timer_handler);

    com_puts("Hello, World !\n");
    com_puts("It works !\n");

    for (;;);

    __builtin_unreachable();
}
