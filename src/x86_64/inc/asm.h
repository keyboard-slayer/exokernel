#ifndef ARCH_X86_64_INC_ASM_H
#define ARCH_X86_64_INC_ASM_H

#include <stdint.h>

#define MSR_GS_BASE 0xC0000101
#define MSR_KERN_GS_BASE 0xc0000102


static inline void asm_write_msr(uint64_t msr, uint64_t value)
{
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    __asm__ volatile("wrmsr" :: "c"((uint64_t)msr), "a"(low), "d"(high));
}

static inline uint64_t asm_read_msr(uint64_t msr)
{
    uint32_t low, high;
    __asm__ volatile("rdmsr" : "=a"(low), "=d"(high) : "c"((uint64_t)msr));
    return ((uint64_t) high << 32) | low;
}

static inline uint8_t asm_in8(uint16_t port)
{
    uint8_t data;
    __asm__ volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline void asm_out8(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %0, %1"
                 :
                 : "a"(data), "d"(port));
}


#endif /* !ARCH_X86_64_INC_ASM_H */
