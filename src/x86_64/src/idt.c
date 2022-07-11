#include "../inc/idt.h"
#include "../inc/gdt.h"

#include <kernel/inc/logging.h>

static idt_entry_t idt_entries[IDT_LENGTH] = {0};

static idt_descriptor_t idt = {
    .limit = sizeof(idt_entry_t) - 1,
    .base = (uint64_t) &idt_entries,
};

static void idt_init_entry(idt_entry_t *self, uint64_t base, interrupt_type_t type)
{
    self->offset_low = base & 0xFFFF;
    self->offset_mid = (base >> 16) & 0xFFFF;
    self->offset_high = (base >> 32) & 0xFFFFFFFF;

    self->ist = 0;
    self->zero = 0;
    self->selector = GDT_KERNEL_CODE * 8;
    self->type_attr = type;
}

void idt_init(void)
{
    klog(INFO, "Initializing IDT...");

    for (int i = 0; i < 32; i++)
    {
        idt_init_entry(&idt_entries[i], __interrupt_vector[i], i == 1 ?  INT_INTERRUPT_GATE : INT_TRAP_GATE);
    }

    idt_flush((uint64_t) &idt);
    klog(OK, "IDT initialized.");
}
