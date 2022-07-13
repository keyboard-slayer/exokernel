#ifndef _KERNEL_INC_ARCH_H_
#define _KERNEL_INC_ARCH_H_

#ifdef __x86_64
#include <x86_64/inc/regs.h>
#define PAGE_SIZE 4096
#define STACK_SIZE (0x4000)
#endif

#define USER_STACK_BASE (0xc0000000)

#include <stdbool.h>
#include <stdint.h>

typedef struct 
{
    uintptr_t physical;
    uintptr_t virtual;
    uintptr_t length;
} virtual_physical_map_t;

typedef struct 
{
    uintptr_t syscall_kernel_stack;
    uintptr_t syscall_user_stack;
    uintptr_t syscall_kernel_bstack;

    void *space;
    regs_t regs;
} binary_context_t;

void arch_init(void);
void *vmm_create_space(void);
void vmm_map(void *pml, virtual_physical_map_t map, bool user);
binary_context_t context_create(uintptr_t ip, void *space);

#endif /* !_KERNEL_INC_ARCH_H_ */
