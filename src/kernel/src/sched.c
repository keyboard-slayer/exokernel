#include "../inc/arch.h"
#include "../inc/logging.h"
#include "x86_64/inc/vmm.h"

static binary_context_t to_switch = {0};
static binary_context_t current = {0};


void sched_init(void)
{
    current.space = vmm_get_kernel_pml();
}

void sched_yield(regs_t *regs)
{
    current.regs = *regs;

    if (to_switch.space == NULL)
    {
        return;
    }

    binary_context_t tmp = current;
    current = to_switch;
    to_switch = tmp;

    context_switch(current);
    vmm_switch_space(current.space);
    *regs = current.regs;
}

void sched_push(binary_context_t ctx)
{
    to_switch = ctx;
}
