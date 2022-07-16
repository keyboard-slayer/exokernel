#include "../inc/cpu.h"
#include "../inc/madt.h"
#include "../inc/gdt.h"
#include "../inc/idt.h"
#include "../inc/syscall.h"
#include "../inc/vmm.h"

#include <kernel/inc/arch.h>
#include <kernel/inc/lock.h>
#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>

DECLARE_LOCK(smp);

static cpu_t *cpus = NULL;
static size_t cpus_count = 0;
static size_t cpus_ready = 1;

extern void *calloc(size_t, size_t);

void init_cpus(size_t count)
{
    cpus_count = count;

    cpus = calloc(count, sizeof(cpu_t));
    if (cpus == NULL)
    {
        klog(ERROR, "Could not allocate memory for cpus");
        halt();
    }
}

size_t cpu_get_count(void)
{
    return cpus_count;
}

size_t cpu_get_ready(void)
{
    return cpus_ready;
}

void _start_cpus(void)
{
    if (cpus == NULL)
    {
        klog(ERROR, "CPUs not initialized");
        halt();
    }

    LOCK(smp);

    klog(INFO, "Booting up...", lapic_current_cpu());

    gdt_init();
    idt_init();

    vmm_switch_space(vmm_get_kernel_pml());

    syscall_init();
    intstack_init();
    lapic_init();
    lapic_timer_init();

    UNLOCK(smp);

    cpus_ready++;

    while (cpus[lapic_current_cpu()].goto_func == NULL)
    {
        __asm__ volatile ("pause");
    }

    cpus[lapic_current_cpu()].goto_func();
}
