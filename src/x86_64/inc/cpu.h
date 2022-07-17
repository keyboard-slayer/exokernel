#ifndef ARCH_X86_64_INC_CPU_H
#define ARCH_X86_64_INC_CPU_H

#include <stddef.h>
#include <stdint.h>

typedef struct 
{
    void(*goto_func)(void);
} cpu_t;

void _start_cpus(void);
void init_cpus(size_t count);
size_t cpu_get_ready(void);
size_t cpu_get_count(void);

#endif /* !ARCH_X86_64_INC_CPU_H */
