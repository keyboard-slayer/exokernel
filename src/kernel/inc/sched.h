#ifndef KERNEL_INC_SCHED_H
#define KERNEL_INC_SCHED_H

#include "arch.h"
#include "task.h"


void sched_yield(regs_t *regs);
void sched_init(void);
void sched_push(task_t *ctx);
task_t *sched_current(void);
pid_t sched_next_pid(void);
bool sched_is_running(void);

#endif /* !KERNEL_INC_SCHED_H */
