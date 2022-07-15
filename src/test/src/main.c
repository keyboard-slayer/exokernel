#include "../inc/com.h"
#include "../inc/syscall.h"

#define IRQ(x) (x + 32)

void key_event(void)
{
    return;
}

int main(void)
{
    syscall(SYS_REG_HANDLER, (uint64_t) key_event, IRQ(1));

    com_puts("Hello, World !\n");
    com_puts("It works !\n");

    for (;;);

    __builtin_unreachable();
}
