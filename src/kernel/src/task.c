#include <kernel/inc/pmm.h>
#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>

#include <libc/inc/stdlib.h>
#include <libc/inc/string.h>

#include "../inc/task.h"

task_t *task_create(void *space, char const *path, uintptr_t ip)
{
    task_t *self = calloc(sizeof(task_t), 1);
    self->space = space;
    self->stack = pmm_alloc(STACK_SIZE);
    
    memncpy(self->path, path, UNIX_PATH_LIMIT);

    if (self->stack == NULL)
    {
        klog(ERROR, "Failed to allocate stack for task");
        halt();
    }

    vmm_map(space, (virtual_physical_map_t) {
        .physical = (uintptr_t) self->stack,
        .virtual = USER_STACK_BASE,
        .length = STACK_SIZE
    }, true);

    self->context = context_create(ip);
    return self;
}
