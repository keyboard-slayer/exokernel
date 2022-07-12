#include "../inc/arch.h"
#include "../inc/pmm.h"
#include "../inc/loader.h"
#include "../inc/logging.h"
#include "../inc/lock.h"
#include "../inc/utils.h"

#include <stdint.h>
#include <stdbool.h>

DECLARE_LOCK(pmm);

static uint8_t *bitmap;
static size_t bitmap_size = 0;
static size_t last_index = 0;

static void pmm_free(uint64_t base, uint64_t length)
{
    LOCK(pmm);
    
    size_t target = base / PAGE_SIZE;

    for (uint64_t i = target; i < length; i++)
    {
        bitmap[i / 8] &= ~(1 << (i % 8));
    }

    UNLOCK(pmm);
}

static void pmm_set_used(uint64_t base, uint64_t length)
{
    size_t target = base / PAGE_SIZE;

    for (size_t i = 0; i < length / PAGE_SIZE; i++)
    {
        bitmap[(i + target ) / 8] |= (1 << ((i + target) % 8));
    }
}

static bool bitmap_is_bit_set(size_t index)
{
    return bitmap[index / 8] & (1 << (index % 8));
}

void *pmm_alloc(size_t length)
{
    LOCK(pmm);

    size_t size = 0;
    uintptr_t base = 0;

    for (size_t i = last_index; i < bitmap_size && size < length; i++)
    {
        if (!bitmap_is_bit_set(i))
        {
            if (size == 0)
            {
                base = i * PAGE_SIZE;
            }

            size += PAGE_SIZE;
        }
        else  
        {
            size = 0;
        }
    }

    if (size >= length)
    {
        last_index = (base + length) / PAGE_SIZE;
        pmm_set_used(base, length);
    }
    else  
    {
        if (last_index == 0)
        {
            klog(ERROR, "Not enough memory.");
            halt();
        }

        last_index = 0;
        return pmm_alloc(length);
    }

    UNLOCK(pmm);
    return (void *) base;
}

void pmm_init(void)
{
    klog(INFO, "Initializing PMM."); 

    uint64_t hhdm = loader_get_hhdm();
    
    if (hhdm == 0)
    {
        klog(ERROR, "Could not get HHDM.");
        halt();
    }

    memmaps_t *memmaps = loader_get_memmap();

    if (memmaps == NULL)
    {
        klog(ERROR, "Could not get memory map.");
        halt();
    }

    memmap_t last = memmaps->entries[memmaps->length - 1];
    bitmap_size = ALIGN_UP((last.base + last.length) / PAGE_SIZE * 8, PAGE_SIZE);

    klog(INFO, "PMM bitmap size: %zu bytes.", bitmap_size);

    for (size_t i = 0; i < memmaps->length; i++)
    {
        memmap_t *entry = &memmaps->entries[i];

        if (entry->type == MEMMAP_USABLE && entry->length >= bitmap_size)
        {
            bitmap = (uint8_t *) (entry->base + hhdm);
            klog(INFO, "PMM bitmap allocaed at %p.", bitmap);

            entry->base += bitmap_size;
            entry->length -= bitmap_size;
            break;
        }
    }

    if (bitmap == NULL)
    {
        klog(ERROR, "Couldn't allocate PMM bitmap.");
        halt();
    }

    __builtin_memset(bitmap, 0xff, bitmap_size);

    for (size_t i = 0; i < memmaps->length; i++)
    {
        memmap_t *entry = &memmaps->entries[i];

        if (entry->type == MEMMAP_USABLE)
        {
            for (size_t j = 0; j < entry->length; j += PAGE_SIZE)
            {
                pmm_free(entry->base + j, PAGE_SIZE);
            }
        }
    }

    pmm_set_used(((uintptr_t) bitmap) - hhdm, bitmap_size);
    klog(OK, "PMM initialized.");
}
