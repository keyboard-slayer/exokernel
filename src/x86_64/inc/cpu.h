#ifndef X86_64_INC_CPU_H_
#define X86_64_INC_CPU_H_

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

#endif /* !X86_64_INC_CPU_H_ */
