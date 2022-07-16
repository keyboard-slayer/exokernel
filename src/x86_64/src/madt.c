#include "../inc/madt.h"
#include "../inc/acpi.h"
#include "../inc/asm.h"
#include "../inc/pit.h"

#include <kernel/inc/logging.h>
#include <kernel/inc/loader.h>

static uintptr_t lapic_addr = 0;
static uintptr_t ioapic_addr;
static uint32_t gsib;

static uint32_t lapic_read(uint32_t reg)
{
    return *((volatile uint32_t *)(lapic_addr + reg));
}

static void lapic_write(uint32_t reg, uint32_t value)
{
    *((volatile uint32_t *)(lapic_addr + reg)) = value;
}

void apic_init(void)
{
    madt_t *madt = (madt_t *) acpi_parse_rsdt("APIC");
    if (madt == NULL)
    {
        klog(ERROR, "No MADT found");
        halt();
    }  

    acpi_checksum((acpi_sdt_t *) madt);
    lapic_addr = madt->lapic_address + loader_get_hhdm();
    klog(INFO, "LAPIC found at %p", lapic_addr);

    uint8_t *ptr = (uint8_t *) madt + sizeof(madt_t);

    while (ptr < ((uint8_t *) madt + madt->header.length))
    {
        switch (ptr[0])
        {
            case MADT_IOAPIC: 
            {
                madt_ioapic_t *ioapic = (madt_ioapic_t *) ptr;
                ioapic_addr = ioapic->ioapic_address + loader_get_hhdm();
                gsib = ioapic->global_system_interrupt_base;
                klog(INFO, "IOAPIC found at %p", ioapic_addr);
                break;
            }
        }

        ptr += ptr[1];
    }

    lapic_init();
    lapic_timer_init();
}

void lapic_init(void)
{
    asm_write_msr(MSR_APIC, (asm_read_msr(MSR_APIC) | LAPIC_ENABLE) & ~((1 << 10)));
    lapic_write(LAPIC_SPURIOUS, lapic_read(LAPIC_SPURIOUS) | 0x1ff);
}

void lapic_timer_init(void)
{
    lapic_write(LAPIC_TPR, 0);

    lapic_write(LAPIC_TIMER_DIV, 3);
    lapic_write(LAPIC_INIT, 0xffffffff);

    pit_sleep(10);

    lapic_write(LAPIC_TIMER_REG, LAPIC_TIMER_MASK);

    uint32_t ticks = 0xffffffff - lapic_read(LAPIC_CURRENT);

    lapic_write(LAPIC_TIMER_REG, 32 | LAPIC_TIMER_PERIODIC);
    lapic_write(LAPIC_TIMER_DIV, 3);
    lapic_write(LAPIC_INIT, ticks);
}

void lapic_eoi(void)
{
    lapic_write(LAPIC_EOI, 0);
}

int lapic_current_cpu(void)
{
    if (lapic_addr == 0)
    {
        return 0;
    }

    return lapic_read(LAPIC_CPU_ID) >> 24;
}


/* TODO: Rewrite with SMP later */

static void io_apic_write(uint32_t reg, uint32_t data) 
{
    volatile uint32_t *base = (volatile uint32_t *) ioapic_addr;
    *base = reg;
    *(base + 4) = data;
}


void io_apic_setup_irq(uint8_t irq, bool enabled)
{
    uint64_t redirect = irq + 0x20;

    if (!enabled)
    {
        redirect |= (1 << 16);
    }

    uint32_t ioredtbl = (irq - gsib) * 2 + 16; 
    io_apic_write(ioredtbl, (uint32_t) redirect);
    io_apic_write(ioredtbl + 1, (uint32_t) (redirect >> 32));
}

/* ====== */
