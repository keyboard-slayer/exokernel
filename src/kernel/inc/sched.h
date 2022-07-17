#ifndef KERNEL_INC_SCHED_H
#define KERNEL_INC_SCHED_H

#include "arch.h"
#include "task.h"


void sched_yield(regs_t *regs);
void sched_init(void);
void sched_push(task_t *ctx);
task_t *sched_current(void);
task_t *create_task(void *space, uintptr_t ip);

#endif /* !KERNEL_INC_SCHED_H */
