#ifndef KERNEL_INC_TASK_H
#define KERNEL_INC_TASK_H

#include "arch.h"

typedef struct 
{
    void *space;
    uint8_t *stack;
    context_t context;
} task_t;

#endif /* !KERNEL_INC_TASK_H */
