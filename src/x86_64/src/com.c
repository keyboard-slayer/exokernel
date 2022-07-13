#include <stdint.h>
#include "../inc/asm.h"

#define COM_PORT 0x3F8

static int is_com_init = 0;

static void com_init(void)
{
    asm_out8(COM_PORT + 1, 0x00);
    asm_out8(COM_PORT + 0, 0x03);
    asm_out8(COM_PORT + 1, 0x00);
    asm_out8(COM_PORT + 3, 0x03);
    asm_out8(COM_PORT + 2, 0xc7);
    asm_out8(COM_PORT + 4, 0x0b);
    asm_out8(COM_PORT + 4, 0x1e);
    asm_out8(COM_PORT + 0, 0xae);
    asm_out8(COM_PORT + 4, 0x0f);

    is_com_init = 1;
}

void com_putc(char c)
{
    if (!is_com_init)
    {
        com_init();
    }

    while ((asm_in8(COM_PORT + 5) & 0x20) == 0);
    asm_out8(COM_PORT, c);
}

void com_puts(char const *s)
{
    while (*s)
    {
        com_putc(*s++);
    }
}
