#include "../inc/syscall.h"
#include "../inc/gdt.h"
#include "../inc/regs.h"
#include "../inc/asm.h"

#include <kernel/inc/logging.h>
#include <kernel/inc/sched.h>

#include <stdint.h>

typedef int64_t (*syscall_t)(regs_t *);
typedef void (*handler_t)(void);
typedef void (*prehandler_t)(handler_t, handler_t);

void syscall_init(void)
{
    klog(OK, "Initializing system calls");
    asm_write_msr(MSR_EFER, asm_read_msr(MSR_EFER) | 1);
    asm_write_msr(MSR_STAR, ((uint64_t)(GDT_KERNEL_CODE * 8) << STAR_KCODE_OFFSET) | ((uint64_t) (((GDT_USER_DATA - 1) * 8) | 3 ) << STAR_UCODE_OFFSET));
    asm_write_msr(MSR_LSTAR, (uint64_t) syscall_handle);
    asm_write_msr(MSR_SYSCALL_FLAG_MASK, 0xfffffffe);
    klog(OK, "System calls initialized");
}

int64_t syscall_reg_handler(regs_t *regs)
{
    task_t *task = sched_current();
    task->handlers[regs->rdx] = (void(*)(void)) regs->rbx;
    return 0;
}

int64_t syscall_reg_prehandler(regs_t *regs)
{
    task_t *task = sched_current();
    task->prehandler = (prehandler_t) regs->rbx;
    return 0;
}

syscall_t kernel_matrix[] = {
    [SYS_REG_HANDLER] = syscall_reg_handler,
    [SYS_REG_PREHANDLER] = syscall_reg_prehandler
};

int64_t syscall_handler(regs_t *regs)
{
    if ((int64_t) regs->rax < 0)
    {
        kernel_matrix[regs->rax * -1](regs);
    }

    return 0;
}

