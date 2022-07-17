#ifndef ARCH_X86_64_INC_REGS_H
#define ARCH_X86_64_INC_REGS_H

#include <stdint.h>

enum rflags 
{
    RFLAGS_CF = 1 << 0,
    RFLAGS_PF = 1 << 2,
    RFLAGS_AF = 1 << 4,
    RFLAGS_ZF = 1 << 6,
    RFLAGS_SF = 1 << 7,
    RFLAGS_TF = 1 << 8,
    RFLAGS_IF = 1 << 9,
    RFLAGS_DF = 1 << 10,
    RFLAGS_OF = 1 << 11,
    RFLAGS_IOPL = 3 << 12,
    RFLAGS_NT = 1 << 14,
    RFLAGS_RF = 1 << 16,
    RFLAGS_VM = 1 << 17,
    RFLAGS_AC = 1 << 18,
    RFLAGS_VIF = 1 << 19,
    RFLAGS_VIP = 1 << 20,
    RFLAGS_ID = 1 << 21,
};

typedef struct
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t intno;
    uint64_t err;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) regs_t;

typedef struct stackframe
{
    struct stackframe *rbp;
    uint64_t rip;
} stackframe_t;

#endif /* !ARCH_X86_64_INC_REGS_H */
