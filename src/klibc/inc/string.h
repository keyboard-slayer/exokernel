#ifndef KLIBC_INC_STRING_H
#define KLIBC_INC_STRING_H

#include <stddef.h>


void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
size_t strlen(const char *s);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dst, void const *src, size_t n);
void *memncpy(void *dst, void const *src, size_t n);
char *strrchr(char const *s, int c);

#endif /* !KLIBC_INC_STRING_H */
