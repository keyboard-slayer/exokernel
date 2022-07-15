#ifndef TEST_INC_ASM_H
#define TEST_INC_ASM_H

#include <stdint.h>

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
#endif /* !TEST_INC_ASM */
