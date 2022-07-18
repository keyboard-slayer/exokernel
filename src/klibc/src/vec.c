#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>

#include <assert.h>
#include <stdlib.h>

#include <vec.h>

void vec_expand_(char **data, size_t *length, size_t *capacity, int memsz)
{
    if (*length + 1 > *capacity)
    {
        void *ptr;
        size_t n = (*capacity == 0) ? 1 : *capacity << 1;
        ptr = realloc(*data, n * memsz);

        if (ptr == NULL)
        {
            klog(ERROR, "vec_expand: realloc failed");
            halt();
        }

        *data = ptr;
        *capacity = n;
    }
}
