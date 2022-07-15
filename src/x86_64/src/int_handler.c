#include "../inc/regs.h"
#include "../inc/madt.h"
#include "kernel/inc/loader.h"

#include <kernel/inc/logging.h>
#include <kernel/inc/com.h>
#include <kernel/inc/arch.h>
#include <kernel/inc/sched.h>

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

    klog(NONE, "RAX %p RBX %p RCX %p RDX %p", regs->rax,
                 regs->rbx, regs->rcx, regs->rdx);
    klog(NONE, "RSI %p RDI %p RBP %p RSP %p", regs->rsi,
                 regs->rdi, regs->rbp, regs->rsp);
    klog(NONE, "R8  %p R9  %p R10 %p R11 %p", regs->r8,
                 regs->r9, regs->r10, regs->r11);
    klog(NONE, "R12 %p R13 %p R14 %p R15 %p", regs->r12,
                 regs->r13, regs->r14, regs->r15);
    klog(NONE, "CR0 %p CR2 %p CR3 %p CR4 %p", cr0, cr2, cr3, cr4);
    klog(NONE, "CS  %p SS  %p FLG %p", regs->cs, regs->ss, regs->rflags);
    klog(NONE, "RIP \033[7m%p\033[0m", regs->rip);
}

uint64_t interrupts_handler(uint64_t rsp)
{
    regs_t *regs = (regs_t *) rsp;
    task_t *task = sched_current();

    if (task != NULL && task->handlers[regs->intno] != NULL)
    {
        regs->rip = (uint64_t) task->prehandler;
        regs->rdi = regs->rip;
        regs->rsi = (uint64_t) task->handlers[regs->intno];
    }
    else if (regs->intno < 32)
    {
        __asm__ volatile ("cli");
        output_exception(regs);

        for(;;)
        {
            __asm__ volatile ("cli");
            __asm__ volatile ("hlt");
        }
    }
    else if (regs->intno < 48)
    {
        uint8_t irq = regs->intno - 32;

        switch(irq)
        {
            case 0:
            {
                break;
            }
        }
    } 
    else if(regs->intno == 48)
    {
        sched_yield(regs);
    }

    lapic_eoi();
    return rsp;
}
