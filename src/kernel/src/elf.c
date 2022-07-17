
#include <klibc/inc/string.h>
#include <klibc/inc/stdlib.h>

#include <stddef.h>

#include "../inc/elf.h"
#include "../inc/arch.h"
#include "../inc/logging.h"
#include "../inc/utils.h"
#include "../inc/pmm.h"
#include "../inc/loader.h"
#include "../inc/sched.h"

task_t *loader_binary(void *elf, char const *path)
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

    void *space = vmm_create_space(); 

    for (size_t i = 0; i < header->e_phnum; i++)
    {
        Elf64_Phdr *program_header = (Elf64_Phdr *)((uintptr_t) elf + header->e_phoff + i * header->e_phentsize);

        if (program_header->p_type == PT_LOAD)
        {
            klog(INFO, "Loading Pheader %p -> %p", program_header->p_vaddr, program_header->p_vaddr + program_header->p_memsz);
            void *addr = pmm_alloc(ALIGN_UP(program_header->p_memsz, PAGE_SIZE));

            if (addr == NULL)
            {
                klog(ERROR, "Failed to allocate memory for ELF");
                halt();
            }

            vmm_map(space, (virtual_physical_map_t) {
                .virtual = ALIGN_DOWN(program_header->p_vaddr, PAGE_SIZE),
                .physical = (uint64_t) addr,
                .length = ALIGN_UP(program_header->p_memsz, PAGE_SIZE)
            }, true);

            memcpy((void *)((uint64_t) addr + loader_get_hhdm()), (void *)((uint64_t)elf + program_header->p_offset), program_header->p_filesz);
            memcpy((void *)((uint64_t) addr + loader_get_hhdm() + program_header[i].p_filesz), (void *)((uint64_t)elf + program_header->p_offset + program_header->p_filesz), program_header->p_memsz - program_header->p_filesz);
        }
    }

    klog(INFO, "Entry point: %p", header->e_entry);

    return task_create(space, path, header->e_entry, sched_next_pid());
}
