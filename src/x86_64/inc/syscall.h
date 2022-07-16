#ifndef _ARCH_X86_64_INC_SYSCALL_H
#define _ARCH_X86_64_INC_SYSCALL_H

#define STAR_KCODE_OFFSET 32
#define STAR_UCODE_OFFSET 48

#define MSR_EFER 0xC0000080
#define MSR_STAR 0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_COMPAT_STAR 0xC0000083
#define MSR_SYSCALL_FLAG_MASK 0xC0000084
#define MSR_FS_BASE 0xC0000100
#define MSR_GS_BASE 0xC0000101
#define MSR_KERN_GS_BASE 0xc0000102

enum SYSCALL {
    SYS_LOG = 0,
};

extern void syscall_handle(void);
void syscall_init(void);

#endif /* !_ARCH_X86_64_INC_SYSCALL_H */
