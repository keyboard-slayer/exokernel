#ifndef KLIBC_INC_STDLIB_H
#define KLIBC_INC_STDLIB_H

#include <stddef.h>

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
long atol(const char *nptr);

#endif /* !KLIBC_INC_STDLIB_H */
