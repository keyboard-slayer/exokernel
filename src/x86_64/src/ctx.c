#include <kernel/inc/arch.h>
#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>

#include "../inc/gdt.h"
#include "../inc/asm.h"
#include "../inc/vmm.h"
#include "kernel/inc/loader.h"

extern void *calloc(size_t, size_t);

binary_context_t context_create(uintptr_t ip, void *space)
{
    binary_context_t ret;

    ret.space = space;
    ret.regs = (regs_t) {0};
    ret.regs.rip = ip;
    ret.regs.rsp = USER_STACK_BASE + STACK_SIZE;
    ret.regs.rbp = USER_STACK_BASE;
    ret.regs.rflags = 0x202 | RFLAGS_IOPL;
    ret.regs.cs = (GDT_USER_CODE * 8) | 3;
    ret.regs.ss = (GDT_USER_DATA * 8) | 3;

    ret.syscall_kernel_bstack = (uintptr_t) calloc(1, STACK_SIZE);
    ret.syscall_kernel_stack = ret.syscall_kernel_bstack + STACK_SIZE;

    vmm_map(space, (virtual_physical_map_t) {
        .physical = ALIGN_DOWN(ret.syscall_kernel_bstack - loader_get_hhdm(), PAGE_SIZE),
        .virtual = USER_STACK_BASE,
        .length = STACK_SIZE + PAGE_SIZE,
    }, true);

    return ret;
}

void context_switch(binary_context_t ctx)
{
    asm_write_msr(MSR_GS_BASE, (uintptr_t) &ctx);
    asm_write_msr(MSR_KERN_GS_BASE, (uintptr_t) &ctx);
}
