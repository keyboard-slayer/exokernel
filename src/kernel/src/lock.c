#include <stddef.h>

static size_t lock_count = 0;

void lock_interrupt(void)
{
    if (lock_count == 0)
    {
        __asm__ volatile ("cli");
    }

    lock_count++;
}

void unlock_interrupt(void)
{
    lock_count--;

    if (lock_count == 0)
    {
        __asm__ volatile ("sti");
    }
}
