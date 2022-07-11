#ifndef _ARCH_X86_64_IDT_H_
#define _ARCH_X86_64_IDT_H_

#include <stdint.h>

#define IDT_LENGTH 256

typedef struct 
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_descriptor_t;

typedef struct 
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef enum 
{
    INT_TASK_GATE = 0x5,
    INT_INTERRUPT_GATE = 0xE,
    INT_TRAP_GATE = 0xF,
    INT_PRESENT = 1 << 7
} interrupt_type_t;

extern uintptr_t __interrupt_vector[];
extern void idt_flush(uintptr_t);

void idt_init(void);

#endif /* !_ARCH_X86_64_IDT_H_ */
