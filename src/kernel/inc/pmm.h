#ifndef KERNEL_INC_PMM_H
#define KERNEL_INC_PMM_H

#include <stddef.h>

void pmm_init(void);
void *pmm_alloc(size_t length);

#endif /* !KERNEL_INC_PMM_H */
