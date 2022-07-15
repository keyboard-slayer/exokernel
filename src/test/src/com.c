#include <stdint.h>

#include "../inc/com.h"

static uint8_t asm_in8(uint16_t port)
{
    uint8_t data;
    __asm__ volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static void asm_out8(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %0, %1"
                 :
                 : "a"(data), "d"(port));
}

void com_putc(char c)
{
    while ((asm_in8(COM_PORT + 5) & 0x20) == 0);
    asm_out8(COM_PORT, c);
}

void com_puts(char const *s)
{
    while (*s)
    {
        com_putc(*s++);
    }

    return;
}
