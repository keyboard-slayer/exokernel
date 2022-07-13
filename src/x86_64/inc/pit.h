#ifndef ARCH_X86_64_INC_PIT_H
#define ARCH_X86_64_INC_PIT_H

#include <stdint.h>

void pit_init(void);
void pit_sleep(uint16_t ms);

#endif /* !ARCH_X86_64_INC_PIT_H */
