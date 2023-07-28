#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <gelf.h>
#include <unistd.h>
#include"memory.h"
int getsym( char *file) {
    const char *filename = file;

    int fd = open(filename, O_RDONLY, 0);
    if (fd < 0) {
        printf("Error: could not open file '%s'\n", filename);
        return 1;
    }

    if (elf_version(EV_CURRENT) == EV_NONE) {
        printf("Error: ELF library initialization failed: %s\n", elf_errmsg(-1));
        return 1;
    }

    Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
    if (elf == NULL) {
        printf("Error: could not read ELF file: %s\n", elf_errmsg(-1));
        return 1;
    }

    if (elf_kind(elf) != ELF_K_ELF) {
        printf("Error: not an ELF file\n");
        return 1;
    }

    size_t symcount;
    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    Elf_Data *data;
    GElf_Sym sym;

    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            printf("Error: could not get section header: %s\n", elf_errmsg(-1));
            return 1;
        }

        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM) {
            data = elf_getdata(scn, NULL);

            if (data == NULL) {
                printf("Error: could not get section data: %s\n", elf_errmsg(-1));
                return 1;
            }

            symcount = shdr.sh_size / shdr.sh_entsize;

            for (size_t i = 0; i < symcount; i++) {
                if (gelf_getsym(data, i, &sym) != &sym) {
                    printf("Error: could not get symbol: %s\n", elf_errmsg(-1));
                    return 1;
                }

                const char *name = elf_strptr(elf, shdr.sh_link, sym.st_name);

                printf("Symbol: %s, Address: 0x%lx\n", name, (unsigned long)sym.st_value);
            }
        }
    }

    elf_end(elf);
    close(fd);

    return 0;
}
