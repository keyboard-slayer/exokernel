#include "../inc/gdt.h"

#include <kernel/inc/arch.h>
#include <kernel/inc/logging.h>

static gdt_t gdt;

static gdt_descriptor_t gdt_descriptor = {
    .limit = sizeof(gdt_t) - 1,
    .base = (uint64_t)&gdt,
};

extern void *malloc(size_t);

static tss_t tss;

static void gdt_init_entry(gdt_entry_t *self, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    self->flags = flags;
    self->access = GDT_PRESENT | GDT_RW | GDT_SEGMENT | access;

    self->base_low = base & 0xFFFF;
    self->base_middle = (base >> 16) & 0xFF;
    self->base_high = (base >> 24) & 0xFF;

    self->limit_low = limit & 0xFFFF;
    self->limit_high = (limit >> 16) & 0xF;
}

static tss_entry_t init_tss(uintptr_t tss)
{
    return (tss_entry_t)
    {
        .length = sizeof(tss_entry_t),
        .base_low16 = (uint16_t) tss & 0xffff,
        .base_mid8 = (uint8_t) (tss >> 16 ) & 0xff,
        .flags1 = 0x89,
        .flags2 = 0,
        .base_high8 = (uint8_t) (tss >> 24) & 0xff,
        .base_upper32 = tss >> 32,
        .reserved = 0,
    };
}

void gdt_init(void)
{
    klog(INFO, "Initializing GDT...");

    gdt_init_entry(&gdt.entries[GDT_NULL], 0, 0, 0, 0);
    gdt_init_entry(&gdt.entries[GDT_KERNEL_CODE], 0, 0, GDT_EXECUTABLE, GDT_LONG_MODE | GDT_GRANULARITY);
    gdt_init_entry(&gdt.entries[GDT_KERNEL_DATA], 0, 0, GDT_RW, GDT_LONG_MODE | GDT_GRANULARITY);
    gdt_init_entry(&gdt.entries[GDT_USER_CODE], 0, 0, GDT_EXECUTABLE | GDT_USER_MODE, GDT_LONG_MODE | GDT_GRANULARITY);

    gdt_init_entry(&gdt.entries[GDT_USER_DATA], 0, 0, GDT_RW | GDT_USER_MODE, GDT_LONG_MODE | GDT_GRANULARITY);

    gdt.tss_entry = init_tss((uintptr_t) &tss);
    tss.iopb = sizeof(tss_t);

    gdt_flush((uintptr_t) &gdt_descriptor);
    tss_flush();

    klog(OK, "GDT initialized.");
}

void intstack_init(void)
{
    tss.ist[0] = (uint64_t) malloc(STACK_SIZE) + STACK_SIZE;
    tss.ist[1] = (uint64_t) malloc(STACK_SIZE) + STACK_SIZE;
    tss.rsp[0] = (uint64_t) malloc(STACK_SIZE) + STACK_SIZE;
}
