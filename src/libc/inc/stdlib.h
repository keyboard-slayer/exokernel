#ifndef LIBC_INC_STDLIB_H
#define LIBC_INC_STDLIB_H

#include <stddef.h>

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
long atol(const char *nptr);

__attribute__((noreturn)) void abort(void);

#endif /* !LIBC_INC_STDLIB_H */
