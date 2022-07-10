#include "../inc/utils.h"

size_t strlen(char const *s)
{
    size_t i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

void *memset(void *s, int c, size_t n)
{
    if (n)
    {
        char *d = (char *) s;

        while (n--)
        {
            *d++ = c;
        }
    }

    return s;
}

char *strrchr(const char *s, int c)
{
    size_t length = __builtin_strlen(s);
    size_t pos = length;

    while (s[pos] != c && pos-- > 0);

    if (pos == length)
    {
        return NULL;
    }

    return (char *) s + pos;
}
