#include "../inc/syscall.h"
#include "../inc/gdt.h"
#include "../inc/regs.h"
#include "../inc/asm.h"

#include <kernel/inc/logging.h>

#include <stdint.h>

void syscall_init(void)
{
    klog(OK, "Initializing system calls");
    asm_write_msr(MSR_EFER, asm_read_msr(MSR_EFER) | 1);
    asm_write_msr(MSR_STAR, ((uint64_t)(GDT_KERNEL_CODE * 8) << STAR_KCODE_OFFSET) | ((uint64_t) (((GDT_USER_DATA - 1) * 8) | 3 ) << STAR_UCODE_OFFSET));
    asm_write_msr(MSR_LSTAR, (uint64_t) syscall_handle);
    asm_write_msr(MSR_SYSCALL_FLAG_MASK, 0xfffffffe);
    klog(OK, "System calls initialized");
}

int64_t syscall_handler(regs_t *regs)
{
    (void) regs;
    klog(INFO, "Syscall %d", regs->rax);
    return regs->rip;
}
