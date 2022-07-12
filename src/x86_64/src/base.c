#include "../inc/gdt.h"
#include "../inc/idt.h"
#include "../inc/vmm.h"
#include "../inc/syscall.h"
#include <kernel/inc/pmm.h>

void arch_init(void)
{
    gdt_init();
    idt_init();
    pmm_init();
    vmm_init();
    syscall_init();
}
