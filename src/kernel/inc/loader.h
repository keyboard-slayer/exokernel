#ifndef KERNEL_INC_LOADER_H_
#define KERNEL_INC_LOADER_H_

#include <stdint.h>
#include <stddef.h>

#include "utils.h"

#define LIMIT_ENTRIES 64

#define FOREACH_MEMMAP_TYPE(FUNC)         \
    FUNC(MEMMAP_USABLE)                   \
    FUNC(MEMMAP_RESERVED)                 \
    FUNC(MEMMAP_RECLAIMABLE)              \
    FUNC(MEMMAP_BAD_MEMORY)               \
    FUNC(MEMMAP_BOOTLOADER_RECLAIMABLE)   \
    FUNC(MEMMAP_KERNEL_AND_MODULES)       \
    FUNC(MEMMAP_FRAMEBUFFER)

typedef enum
{
    FOREACH_MEMMAP_TYPE(GENERATE_ENUM)
} memmap_type_t;

__attribute__((used)) static char const *mmap_type_str[] = 
{
    FOREACH_MEMMAP_TYPE(GENERATE_STRING)
};

typedef struct 
{
    uintptr_t base;
    size_t length;
    memmap_type_t type;
} memmap_t;

typedef struct
{
    memmap_t entries[LIMIT_ENTRIES];
} memmaps_t;

memmaps_t *loader_get_memmap(void);

#endif /* !KERNEL_INC_LOADER_H_ */