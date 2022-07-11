#include "../inc/regs.h"

uint64_t interrupts_handler(uint64_t rsp)
{
    __asm__ volatile("cli");
    __asm__ volatile("hlt");

    return rsp;
}
