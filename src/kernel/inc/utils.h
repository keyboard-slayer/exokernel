#ifndef _KERNEL_INC_UTILS_H
#define _KERNEL_INC_UTILS_H

#include <stddef.h>

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))
#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))

#define halt() klog(ERROR, "Halting..."); \
            for (;;)                      \
            {                             \
                __asm__ volatile("hlt");  \
            } 

#endif /* !_KERNEL_INC_UTILS_H */
