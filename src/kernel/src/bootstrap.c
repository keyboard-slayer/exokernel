#include "../inc/arch.h"
#include "../inc/loader.h"
#include "../inc/pmm.h"
#include "../inc/logging.h"
#include "../inc/elf.h"
#include "../inc/sched.h"

#include <assert.h>

int _start(void)
{
    arch_init();
    sched_init();

    void *executable = loader_get_module("/bin/echo.elf");
    assert(executable != NULL);

    task_t *bin = loader_binary(executable, "/bin/echo.elf");
    sched_push(bin);
    sched_push(bin);

    for (;;);

    __builtin_unreachable();
}
