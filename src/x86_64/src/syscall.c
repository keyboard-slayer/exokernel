#include <kernel/inc/logging.h>
#include <kernel/inc/sched.h>
#include <kernel/inc/com.h>
#include <kernel/inc/pmm.h>
#include <kernel/inc/arch.h>

#include <stdint.h>
#include <unistd.h>

#include "../inc/syscall.h"
#include "../inc/gdt.h"
#include "../inc/regs.h"
#include "../inc/asm.h"

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
    klog_lock();
    com_puts((char const *) regs->rbx);
    klog_unlock();
    return 0;
}

int64_t syscall_getpid(__attribute__((unused)) regs_t *regs)
{
    return sched_current()->pid;
}

int64_t syscall_alloc(regs_t *regs)
{
    void *space = sched_current()->space;
    void *ptr = pmm_alloc(regs->rbx);
    if (ptr == NULL)
    {
        return 0;
    }

    vmm_map(space, (virtual_physical_map_t) {
        .physical = (uintptr_t) ptr,
        .virtual = (uintptr_t) ptr,
        .length = regs->rbx
    }, true);

    return (uintptr_t) ptr;
}

int64_t syscall_free(regs_t *regs)
{
    void *space = sched_current()->space;
    pmm_free(regs->rbx, regs->rcx);
    vmm_unmap(space, regs->rcx);
    
    return 0;
}

syscall_t syscall_matrix[] = {
    [SYS_LOG] = syscall_log,
    [SYS_GETPID] = syscall_getpid,
    [SYS_ALLOC] = syscall_alloc,
    [SYS_FREE] = syscall_free
};

int64_t syscall_handler(regs_t *regs)
{
    return syscall_matrix[regs->rax](regs);
}
