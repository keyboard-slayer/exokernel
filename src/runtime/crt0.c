#include <stdint.h>

#define SYS_REG_PREHANDLER 0xFFFFFFFFFFFFFFFE

void event_prehandler(void(*ret)(void), void (*handler)(void))
{
    handler();
    ret();
}

extern int main(void);

int _start(void)
{
    __asm__ volatile(
        "syscall"
        :: "a"(SYS_REG_PREHANDLER), "b"(event_prehandler)
        : "memory"
    );

    main();

    __builtin_unreachable();
}

