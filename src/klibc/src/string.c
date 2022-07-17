#include "../inc/string.h"

size_t strlen(char const *s)
{
    size_t len = 0;
    while (*s++)
    {
        len++;
    }
    return len;
}

int memcmp(void const *s1, void const *s2, size_t n)
{
    unsigned char const *p1 = s1;
    unsigned char const *p2 = s2;
    while (n--)
    {
        if (*p1 != *p2)
        {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

void *memcpy(void *dst, void const *src, size_t n)
{
    unsigned char *p1 = dst;
    unsigned char const *p2 = src;
    while (n--)
    {
        *p1++ = *p2++;
    }
    return dst;
}

char *strrchr(char const *s, int c)
{
    char *last = NULL;

    while (*s)
    {
        if (*s == c)
        {
            last = (char *)s;
        }
        s++;
    }

    return last;
}
