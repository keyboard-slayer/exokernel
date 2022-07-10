#ifndef _KERNEL_UTILS_H
#define _KERNEL_UTILS_H

#include <stddef.h>

void *memset(void *s, int c, size_t n);
char *strchr(char const *s, int c);
size_t strlen(char const *s);

#endif /* !_KERNEL_UTILS_H */
