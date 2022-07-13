#include <kernel/inc/arch.h>
#include "../inc/gdt.h"

extern void *malloc(uint64_t);

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

    ret.syscall_kernel_bstack = (uintptr_t) malloc(STACK_SIZE);
    ret.syscall_kernel_stack = ret.syscall_kernel_bstack + STACK_SIZE;

    return ret;
}
