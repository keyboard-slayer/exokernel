#define COM_PORT 0X3F8

#include <stdint.h>

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

static void com_putc(char c)
{
    while ((asm_in8(COM_PORT + 5) & 0x20) == 0);
    asm_out8(COM_PORT, c);
}

static void com_puts(char const *s)
{
    while (*s)
    {
        com_putc(*s++);
    }
}

int _start(void)
{
    com_puts("Hello, World !\n");

    for (;;);
    __builtin_unreachable();
}
