#include <klibc/inc/stdlib.h>

#include <stddef.h>

#include "../inc/arch.h"
#include "../inc/logging.h"
#include "../inc/sched.h"
#include "../inc/pmm.h"
#include "../inc/utils.h"
#include "../inc/loader.h"

static task_t *current = NULL;
static task_t *next = NULL;

void sched_init(void)
{
    current = calloc(sizeof(task_t), 1);
    current->space = vmm_get_kernel_pml();
}

void sched_yield(regs_t *regs)
{
    current->context.regs = *regs;

    if (next->space == NULL)
    {
        return;
    }

    task_t *tmp = current;
    current = next;
    next = tmp;

    context_switch(&current->context, regs);
    vmm_switch_space(current->space);
}

void sched_push(task_t *task)
{
    next = task;
}

task_t *sched_current(void)
{
    if (current->space == NULL)
    {
        return NULL;
    }

    return current;
}

