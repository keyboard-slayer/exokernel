#include "../inc/arch.h"
#include "../inc/loader.h"
#include "../inc/pmm.h"
#include "../inc/logging.h"
#include "../inc/elf.h"

int _start(void)
{
    arch_init();

    void *executable = loader_get_module("/bin/test.elf");
    if (executable == NULL)
    {
        klog(ERROR, "Could not load executable");
        halt();
    }

    __attribute__((unused)) binary_context_t bin = loader_binary(executable);

    for (;;);

    __builtin_unreachable();
}
