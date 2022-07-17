#include "../inc/arch.h"
#include "../inc/loader.h"
#include "../inc/pmm.h"
#include "../inc/logging.h"
#include "../inc/elf.h"
#include "../inc/sched.h"

int _start(void)
{
    arch_init();
    sched_init();

    void *executable = loader_get_module("/bin/test.elf");
    if (executable == NULL)
    {
        klog(ERROR, "Could not load executable");
        halt();
    }

    task_t *bin = loader_binary(executable, "/bin/test.elf");
    sched_push(bin);
    cpu_enable_int();

    for (;;);

    __builtin_unreachable();
}
