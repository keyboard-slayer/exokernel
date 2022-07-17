#ifndef ARCH_X86_64_INC_CPU_H
#define ARCH_X86_64_INC_CPU_H

#include <kernel/inc/task.h>

#include <stddef.h>
#include <stdint.h>

typedef struct 
{
    bool int_enable;
    size_t tick;
    size_t current;
    vec_task_t tasks;
} cpu_t;

void _start_cpus(void);
void init_cpus(size_t count);
void cpu_enable_int(void);

cpu_t *cpu(int id);
cpu_t *cpu_self(void);

size_t cpu_get_ready(void);
size_t cpu_get_count(void);

#endif /* !ARCH_X86_64_INC_CPU_H */
