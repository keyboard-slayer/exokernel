#include "../inc/syscall.h"
#include "../inc/gdt.h"
#include "../inc/regs.h"
#include "../inc/asm.h"

#include <kernel/inc/logging.h>
#include <kernel/inc/sched.h>
#include <kernel/inc/com.h>

#include <stdint.h>

typedef int64_t (*syscall_t)(regs_t *);

void syscall_init(void)
{
    klog(INFO, "Initializing system calls");
    asm_write_msr(MSR_EFER, asm_read_msr(MSR_EFER) | 1);
    asm_write_msr(MSR_STAR, ((uint64_t)(GDT_KERNEL_CODE * 8) << STAR_KCODE_OFFSET) | ((uint64_t) (((GDT_USER_DATA - 1) * 8) | 3 ) << STAR_UCODE_OFFSET));
    asm_write_msr(MSR_LSTAR, (uint64_t) syscall_handle);
    asm_write_msr(MSR_SYSCALL_FLAG_MASK, 0xfffffffe);
    klog(OK, "System calls initialized");
}

int64_t syscall_log(regs_t *regs)
{
    com_puts((char const *) regs->rbx);
    return 0;
}

syscall_t syscall_matrix[] = {
    [SYS_LOG] = syscall_log
};

int64_t syscall_handler(regs_t *regs)
{
    syscall_matrix[regs->rax](regs);
    return 0;
}
