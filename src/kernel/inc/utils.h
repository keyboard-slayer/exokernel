#ifndef _KERNEL_INC_UTILS_H
#define _KERNEL_INC_UTILS_H

#include <stddef.h>

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

void *memset(void *s, int c, size_t n);
char *strchr(char const *s, int c);
size_t strlen(char const *s);

#endif /* !_KERNEL_INC_UTILS_H */
