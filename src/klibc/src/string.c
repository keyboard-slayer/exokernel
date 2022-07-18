#include <stdlib.h>

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

void *memncpy(void *dst, void const *src, size_t n)
{
    unsigned char *p1 = dst;
    unsigned char const *p2 = src;
    while (n--)
    {
        if (*p2)
        {
            *p1++ = *p2++;
        }
        else
        {
            break;
        }
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

char *strndup(const char *s, size_t n)
{
    char *dup = malloc(n + 1);

    if (dup)
    {
        memcpy(dup, s, n);
        dup[n] = '\0';
    }
    return dup;
}

char *strdup(char const *s)
{
    return strndup(s, strlen(s));
}

int strcmp(char const *s1, char const *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
        {
            return *s1 - *s2;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}
