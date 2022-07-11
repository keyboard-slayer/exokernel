#ifndef _KERNEL_INC_LOGGING_H_
#define _KERNEL_INC_LOGGING_H_

#include <stddef.h>
#include <stdarg.h>

#define __FILENAME__                                                                               \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

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

#endif /* !_KERNEL_INC_LOGGING_H_ */
