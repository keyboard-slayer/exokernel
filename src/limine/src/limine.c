#include "../inc/limine.h"
#include <stddef.h>

#include <kernel/inc/loader.h>
#include <kernel/inc/logging.h>

volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

static memmaps_t memmaps = {0};

memmaps_t *loader_get_memmap(void)
{
    if (memmap_request.response == NULL)
    {
        return NULL;
    }

    if (memmaps.entries[0].length != 0)
    {
        return &memmaps;
    }

    for (size_t i = 0; i < memmap_request.response->entry_count; i++)
    {
        memmap_t *entry = &memmaps.entries[i];
        entry->base = memmap_request.response->entries[i]->base;
        entry->length = memmap_request.response->entries[i]->length;
        entry->type = memmap_request.response->entries[i]->type;
    }

    return &memmaps;
}
