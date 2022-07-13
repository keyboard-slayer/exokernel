#ifndef ARCH_X86_64_ACPI_H
#define ARCH_X86_64_ACPI_H

#include <stdint.h>

typedef struct 
{
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtAddress;
} __attribute__((packed)) rsdp_t;

typedef struct
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oemtableid[8];
    uint32_t oemrevision;
    uint32_t creatorid;
    uint32_t creatorrevision;
} __attribute__((packed)) acpi_sdt_t;

typedef struct
{
    acpi_sdt_t header;
    uint32_t sdtAddr[];
} __attribute__((packed)) rsdt_t;

acpi_sdt_t *acpi_parse_rsdt(char const *tablename);
void acpi_init(void);
void acpi_checksum(acpi_sdt_t *table);

#endif /* !ARCH_X86_64_ACPI_H */
