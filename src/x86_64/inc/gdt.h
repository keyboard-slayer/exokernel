#ifndef ARCH_X86_64_INC_GDT_H
#define ARCH_X86_64_INC_GDT_H

#include <stdint.h>

#define GDT_ENTRIES 5

#define GDT_NULL 0
#define GDT_KERNEL_CODE 1 
#define GDT_KERNEL_DATA 2 
#define GDT_USER_DATA 3
#define GDT_USER_CODE 4

typedef struct 
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_descriptor_t;

typedef struct 
{
    uint16_t base_low;
    uint16_t limit_low;
    uint8_t base_middle;

    uint8_t access;
    uint8_t limit_high: 4;
    uint8_t flags: 4;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef enum 
{
    GDT_PRESENT = 1 << 7,
    GDT_USER_MODE = 3 << 5,
    GDT_SEGMENT = 1 << 4,
    GDT_EXECUTABLE = 1 << 3,
    GDT_DIRECTION = 1 << 2,
    GDT_RW = 1 << 1,
    GDT_ACCESS = 0
} gdt_entry_access_t;

typedef enum 
{
    GDT_GRANULARITY = 1 << 3,
    GDT_SIZE_FLAG = 1 << 2,
    GDT_LONG_MODE = 1 << 1,
} gdt_entry_flags_t;

typedef struct 
{
    uint32_t reserved;
    uint64_t rsp[3];
    uint64_t reserved2;
    uint64_t ist[7];
    unsigned __int128 reserved3 : 80;
    uint16_t iopb;
} __attribute__((packed)) tss_t;

typedef struct 
{
    uint16_t length;
    uint16_t base_low16;
    uint8_t base_mid8;
    uint8_t flags1;
    uint8_t flags2;
    uint8_t base_high8;
    uint32_t base_upper32;
    uint32_t reserved;
} __attribute__((packed)) tss_entry_t;

typedef struct 
{
    gdt_entry_t entries[GDT_ENTRIES];
    tss_entry_t tss_entry;
} __attribute__((packed)) gdt_t;

void gdt_init(void);

extern void gdt_flush(uintptr_t);
extern void tss_flush(void);
void intstack_init(void);

#endif /* !ARCH_X86_64_INC_GDT_H */
