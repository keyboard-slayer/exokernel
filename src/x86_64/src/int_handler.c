#include "../inc/regs.h"

#include <kernel/inc/logging.h>

static char *exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Detected Overflow",
    "Out Of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad Tss",
    "Segment Not Present",
    "StackFault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

static void output_exception(regs_t const *regs)
{
    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
    uint64_t cr4;

    __asm__ volatile ("mov %%cr0, %0":"=r" (cr0));
    __asm__ volatile ("mov %%cr2, %0":"=r" (cr2));
    __asm__ volatile ("mov %%cr3, %0":"=r" (cr3));
    __asm__ volatile ("mov %%cr4, %0":"=r" (cr4));

    klog(ERROR, "Exception %s (0x%x) Err: %d\n\t", exception_messages[regs->intno], 
                 regs->intno, regs->err);

    klog(NONE, "RAX %016x RBX %016x RCX %016x RDX %016x", regs->rax,
                 regs->rbx, regs->rcx, regs->rdx);
    klog(NONE, "RSI %016x RDI %016x RBP %016x RSP %016x", regs->rsi,
                 regs->rdi, regs->rbp, regs->rsp);
    klog(NONE, "R8  %016x R9  %016x R10 %016x R11 %016x", regs->r8,
                 regs->r9, regs->r10, regs->r11);
    klog(NONE, "R12 %016x R13 %016x R14 %016x R15 %016x", regs->r12,
                 regs->r13, regs->r14, regs->r15);
    klog(NONE, "CR0 %016x CR2 %016x CR3 %016x CR4 %016x", cr0, cr2, cr3, cr4);
    klog(NONE, "CS  %016x SS  %016x FLG %016x", regs->cs, regs->ss, regs->rflags);
    klog(NONE, "RIP \033[7m%016x\033[0m", regs->rip);
}

uint64_t interrupts_handler(uint64_t rsp)
{
    regs_t *regs = (regs_t *) rsp;

    if (regs->intno < 32)
    {
        output_exception(regs);

        for(;;)
        {
            __asm__ volatile ("cli");
            __asm__ volatile ("hlt");
        }
    }

    return rsp;
}
