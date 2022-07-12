#ifndef _KERNEL_INC_ARCH_H_
#define _KERNEL_INC_ARCH_H_

void arch_init(void);

#ifdef __x86_64__
#define PAGE_SIZE 4096
#endif

#endif /* !_KERNEL_INC_ARCH_H_ */
