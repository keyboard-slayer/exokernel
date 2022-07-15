#ifndef KERNEL_INC_SCHED_H
#define KERNEL_INC_SCHED_H

#include "arch.h"

typedef struct 
{
    void *space;
    uint8_t *stack;
    context_t context;
    void (*handlers[256])(void);
    void (*prehandler)(void(*)(void), void(*)(void));
} task_t;

void sched_yield(regs_t *regs);
void sched_init(void);
void sched_push(task_t *ctx);
task_t *sched_current(void);
task_t *create_task(void *space, uintptr_t ip);

#endif /* !KERNEL_INC_SCHED_H */
