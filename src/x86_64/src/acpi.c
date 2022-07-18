#include <kernel/inc/loader.h>
#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>

#include <libc/inc/string.h>

#include "../inc/acpi.h"

static rsdt_t *rsdt;

void acpi_init(void)
{
    rsdp_t *rsdp = loader_get_rsdp();
    if (rsdp == NULL)
    {
        klog(ERROR, "No RSDP found");
        halt();
    }

    rsdt = (rsdt_t *) (rsdp->rsdtAddress + loader_get_hhdm());
}

acpi_sdt_t *acpi_parse_rsdt(char const *tablename)
{
    klog(INFO, "Looking for %s", tablename);
    for (size_t i = 0; i < rsdt->header.length; i++)
    {
        acpi_sdt_t *sdt = (acpi_sdt_t *) (rsdt->sdtAddr[i] + loader_get_hhdm());

        if (memcmp(tablename, sdt->signature, 4) == 0)
        {
            klog(INFO, "Found %s at %p", tablename, sdt);
            return sdt;
        }
    }

    return NULL;
}

void acpi_checksum(acpi_sdt_t *table)
{
    unsigned char sum = 0;

    for (size_t i = 0; i < table->length; i++)
    {
        sum += ((char *) table)[i];
    }

    if (sum != 0)
    {
        klog(ERROR, "Checksum for %s failed", table->signature);
        halt();
    }
}
