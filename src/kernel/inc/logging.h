#ifndef KERNEL_INC_LOGGING_H
#define KERNEL_INC_LOGGING_H

#include <klibc/inc/string.h>

#include <stddef.h>
#include <stdarg.h>

#define __FILENAME__                                                                               \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define klog(LEVEL, ...) \
    klog_impl(LEVEL, __FILENAME__, __LINE__, __VA_ARGS__);

typedef enum 
{
    NONE,
    OK,
    INFO,
    ERROR
} log_level_t;

void klog_impl(log_level_t level, char const *filename, size_t lineno, char const *format, ...);
void klog_lock(void);
void klog_unlock(void);

#endif /* !KERNEL_INC_LOGGING_H */
