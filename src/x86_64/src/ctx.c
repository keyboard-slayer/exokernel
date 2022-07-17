#include <kernel/inc/arch.h>
#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>
#include <kernel/inc/pmm.h>
#include <kernel/inc/loader.h>

#include <klibc/inc/stdlib.h>

#include "../inc/gdt.h"
#include "../inc/asm.h"
#include "../inc/vmm.h"

context_t context_create(uintptr_t ip)
{
    context_t ret;

    ret.regs = (regs_t) {0};
    ret.regs.rip = ip;
    ret.regs.rsp = USER_STACK_BASE + STACK_SIZE;
    ret.regs.rbp = USER_STACK_BASE;
    ret.regs.rflags = 0x202;
    ret.regs.cs = (GDT_USER_CODE * 8) | 3;
    ret.regs.ss = (GDT_USER_DATA * 8) | 3;

    ret.syscall_kernel_bstack = (uintptr_t) pmm_alloc(STACK_SIZE) + loader_get_hhdm();
    ret.syscall_kernel_stack = ret.syscall_kernel_bstack + STACK_SIZE;

    vmm_map(vmm_get_kernel_pml(), (virtual_physical_map_t) {
        .physical = ret.syscall_kernel_bstack,
        .virtual = ret.syscall_kernel_bstack + loader_get_hhdm(),
        .length = STACK_SIZE
    }, true);

    if (ret.syscall_kernel_bstack == 0)
    {
        klog(ERROR, "Failed to allocate stack for context");
        halt();
    }

    return ret;
}

void context_switch(context_t *ctx, regs_t *regs)
{
    asm_write_msr(MSR_GS_BASE, (uintptr_t) ctx);
    asm_write_msr(MSR_KERN_GS_BASE, (uintptr_t) ctx);

    *regs = ctx->regs;
}
