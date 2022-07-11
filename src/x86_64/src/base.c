#include "../inc/gdt.h"
#include "../inc/idt.h"

void arch_init(void)
{
    gdt_init();
    idt_init();

    __asm__ volatile ("int $1");
}
