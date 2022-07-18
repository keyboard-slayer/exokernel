#include <stdlib.h>

long atol(const char *nptr)
{
    long ret = 0;
    int sign = 1;

    if (*nptr == '-')
    {
        sign = -1;
        nptr++;
    }

    while (*nptr)
    {
        ret = ret * 10 + (*nptr - '0');
        nptr++;
    }

    return ret * sign;
}

#ifndef __kernel__
void *malloc(size_t size)
{
    (void) size;
    return NULL;
}

void free(void *ptr)
{
    (void) ptr;
}


void *realloc(void *ptr, size_t size)
{
    (void) ptr;
    (void) size;
    return NULL;
}
#endif /* !__kernel__ */
