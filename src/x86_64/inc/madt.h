#ifndef ARCH_X86_64_MADT_H
#define ARCH_X86_64_MADT_H

#include "../inc/acpi.h"

#include <stdbool.h>

#define LAPIC_CPU_ID        0x020
#define LAPIC_EOI           0x0b0
#define LAPIC_SPURIOUS      0x0f0
#define LAPIC_TPR           0x080
#define LAPIC_TIMER_REG     0x320
#define LAPIC_INIT          0x380
#define LAPIC_CURRENT       0x390
#define LAPIC_TIMER_DIV     0x3e0
#define LAPIC_ENABLE        0x800

#define LAPIC_TIMER_MASK 0x10000
#define LAPIC_TIMER_PERIODIC 0x20000

#define MSR_APIC 0x1B

typedef struct
{
    acpi_sdt_t header;

    uint32_t lapic_address;
    uint32_t flags;
} __attribute__((packed)) madt_t;

typedef struct 
{
    uint16_t madt_header;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_address;
    uint32_t global_system_interrupt_base;
} __attribute__((packed)) madt_ioapic_t;

enum madt_entry 
{
    MADT_IOAPIC = 1,
    MADT_ISO = 2
};

void apic_init(void);
void lapic_eoi(void);
int lapic_current_cpu(void);
void lapic_init(void);
void lapic_timer_init(void);
void io_apic_setup_irq(uint8_t irq, bool enabled);

#endif /* !ARCH_X86_64_MADT_H */
