#include "../inc/elf.h"
#include "../inc/arch.h"
#include "../inc/logging.h"
#include "../inc/utils.h"
#include "../inc/pmm.h"
#include "../inc/loader.h"
#include "../inc/sched.h"

#include <stddef.h>

extern void *calloc(size_t, size_t);

task_t *loader_binary(void *elf)
{
    Elf64_Ehdr *header = elf;
    
    if (header->e_ident[EI_MAG0] != ELFMAG0 ||
        header->e_ident[EI_MAG1] != ELFMAG1 ||
        header->e_ident[EI_MAG2] != ELFMAG2 ||
        header->e_ident[EI_MAG3] != ELFMAG3)
    {
        klog(ERROR, "Invalid ELF magic");
        halt();
    }

    if (header->e_ident[EI_CLASS] != ELFCLASS64)
    {
        klog(ERROR, "Invalid ELF class");
        halt();
    }

    Elf64_Phdr *program_header = (Elf64_Phdr *)((uint64_t)elf + header->e_phoff);
    void *space = vmm_create_space(); 

    for (size_t i = 0; i < header->e_phnum; i++)
    {
        if (program_header[i].p_type == PT_LOAD)
        {
            klog(INFO, "Loading Pheader %p -> %p", program_header[i].p_vaddr, program_header[i].p_vaddr + program_header[i].p_memsz);
            void *addr = pmm_alloc(ALIGN_UP(program_header[i].p_memsz, PAGE_SIZE));

            if (addr == NULL)
            {
                klog(ERROR, "Failed to allocate memory for ELF");
                halt();
            }

            vmm_map(space, (virtual_physical_map_t) {
                .virtual = ALIGN_DOWN(program_header[i].p_vaddr, PAGE_SIZE),
                .physical = (uint64_t) addr,
                .length = ALIGN_UP(program_header[i].p_memsz, PAGE_SIZE)
            }, true);

            __builtin_memcpy((void *)((uint64_t) addr + loader_get_hhdm()), (void *)((uint64_t)elf + program_header[i].p_offset), program_header[i].p_filesz);
            __builtin_memcpy((void *)((uint64_t) addr + loader_get_hhdm() + program_header[i].p_filesz), (void *)((uint64_t)elf + program_header[i].p_offset + program_header[i].p_filesz), program_header[i].p_memsz - program_header[i].p_filesz);
        }

        program_header = (Elf64_Phdr *)((uint64_t)program_header + (i * header->e_phentsize));
    }

    klog(INFO, "Entry point: %p", header->e_entry);

    return create_task(space, header->e_entry);
}
