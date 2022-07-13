#ifndef KERNEL_INC_SCHED_H
#define KERNEL_INC_SCHED_H

#include "arch.h"

void sched_yield(regs_t *regs);
void sched_init(void);
void sched_push(binary_context_t ctx);

#endif /* !KERNEL_INC_SCHED_H */
