#include "../inc/arch.h"
#include "../inc/logging.h"
#include "../inc/sched.h"
#include "../inc/pmm.h"
#include "../inc/utils.h"
#include "../inc/loader.h"

#include <stddef.h>

static task_t *current = NULL;
static task_t *next = NULL;

extern void *calloc(size_t, size_t);

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

task_t *create_task(void *space, uintptr_t ip)
{
    task_t *self = calloc(sizeof(task_t), 1);
    self->space = space;
    self->stack = pmm_alloc(STACK_SIZE);

    if (self->stack == NULL)
    {
        klog(ERROR, "Failed to allocate stack for task");
        halt();
    }

    vmm_map(space, (virtual_physical_map_t) {
        .physical = (uintptr_t) self->stack,
        .virtual = USER_STACK_BASE,
        .length = STACK_SIZE
    }, true);

    self->context = context_create(ip);
    return self;
}
