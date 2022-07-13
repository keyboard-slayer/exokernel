#include "../inc/pic.h"
#include "../inc/asm.h"

#include <stddef.h>

static void io_wait(void)
{
    __asm__ volatile ("jmp 1f\n\t" "1:jmp 2f\n\t" "2:");
}

void pic_disable(void)
{
    asm_out8(PIC2_DATA, 0xff);
    asm_out8(PIC1_DATA, 0xff);
}   

void pic_mask_interrupts(void)
{
    uint16_t port;
    uint8_t value;
    uint8_t irq;

    for (size_t i = 0; i < 16; i++)
    {
        if (i < 8) 
        {
            port = PIC1_DATA;
            irq = i;
        }
        else  
        {
            port = PIC2_DATA;
            irq = i - 8; 
        }

        value = asm_in8(port) | (1 << irq);
        asm_out8(port, value);
    }
}

void pic_init(void)
{
    asm_out8(PIC1_COMMAND, 0x10 | 0x01);
    io_wait();
    asm_out8(PIC2_COMMAND, 0x10 | 0x01);
    io_wait();

    asm_out8(PIC1_DATA, PIC1);
    io_wait();
    asm_out8(PIC2_DATA, PIC2);
    io_wait();

    asm_out8(PIC1_DATA, 4);
    io_wait();
    asm_out8(PIC2_DATA, 2);
    io_wait();

    asm_out8(PIC1_DATA, 0x01);
    io_wait();
    asm_out8(PIC2_DATA, 0x01);
    io_wait();

    asm_out8(PIC1_DATA, 0);
    io_wait();
    asm_out8(PIC2_DATA, 0);
    io_wait();
}
