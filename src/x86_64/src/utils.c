#include <stddef.h>

void *memset(void *s, int c, size_t n)
{
    __asm__ volatile(
        "rep stosb"
        : "=D"(s), "=c"(n)
        : "0"(s), "a"(c), "1"(n)
        : "memory");

    return s;
}
