#include "../inc/limine.h"
#include <stddef.h>

volatile struct limine_hhdm_request hhdm_request = {
    LIMINE_HHDM_REQUEST,
    0,
    NULL
};
