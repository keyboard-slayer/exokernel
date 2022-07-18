#include <kernel/inc/arch.h>
#include <kernel/inc/logging.h>
#include <kernel/inc/pmm.h>
#include <kernel/inc/loader.h>

#include "../inc/acpi.h"
#include "../inc/madt.h"
#include "../inc/gdt.h"
#include "../inc/idt.h"
#include "../inc/vmm.h"
#include "../inc/syscall.h"
#include "../inc/pit.h"
#include "../inc/pic.h"

void arch_init(void)
{
    gdt_init();
    idt_init();
    pmm_init();
    vmm_init();
    acpi_init();
    pit_init();
    pic_init();
    pic_mask_interrupts();
    pic_disable();
    apic_init();
    syscall_init();
    intstack_init();

    loader_boot_other_cpus();
    cpu_enable_int();

    __asm__ volatile ("sti");
}
