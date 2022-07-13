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

volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
};

volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
};

volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
};

static memmaps_t memmaps = {0};

memmaps_t *loader_get_memmap(void)
{
    if (memmap_request.response == NULL)
    {
        return NULL;
    }

    if (memmaps.length > 0)
    {
        return &memmaps;
    }

    memmaps.length = memmap_request.response->entry_count;

    for (size_t i = 0; i < memmap_request.response->entry_count; i++)
    {
        memmap_t *entry = &memmaps.entries[i];
        entry->base = memmap_request.response->entries[i]->base;
        entry->length = memmap_request.response->entries[i]->length;
        entry->type = memmap_request.response->entries[i]->type;
    }

    return &memmaps;
}

uint64_t loader_get_hhdm(void)
{
    if (hhdm_request.response == NULL)
    {
        return 0;
    }

    return hhdm_request.response->offset;
}

uint64_t loader_get_pbase(void)
{
    if (kernel_address_request.response == NULL)
    {
        return 0;
    }

    return kernel_address_request.response->physical_base;
}

uint64_t loader_get_vbase(void)
{
    if (kernel_address_request.response == NULL)
    {
        return 0;
    }

    return kernel_address_request.response->virtual_base;
}

void *loader_get_rsdp(void)
{
    if (rsdp_request.response == NULL)
    {
        return NULL;
    }

    return rsdp_request.response->address;
}
