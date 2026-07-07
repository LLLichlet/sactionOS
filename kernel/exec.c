/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * exec -- ELF binary loader.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"

int exec_elf_load(struct inode* ip, pde_t* pgdir, uint* p_entry, uint* p_sz)
{
    struct elfhdr elf;
    struct proghdr ph;
    uint i, off, sz, memsz;

    if (readi(ip, (char*) &elf, 0, sizeof(elf)) != sizeof(elf))
        return -1;
    if (elf.magic != ELF_MAGIC)
        return -1;

    sz = 0;
    for (i = 0, off = elf.phoff; i < elf.phnum; i++, off += sizeof(ph)) {
        if (readi(ip, (char*) &ph, off, sizeof(ph)) != sizeof(ph))
            return -1;
        if (ph.type != ELF_PROG_LOAD)
            continue;
        if (ph.memsz < ph.filesz)
            return -1;

        memsz = ph.vaddr + ph.memsz;
        if (memsz > sz) {
            if (allocuvm(pgdir, sz, memsz) == 0)
                return -1;
            sz = memsz;
        }
        if (loaduvm(pgdir, (char*) ph.vaddr, ip, ph.off, ph.filesz) < 0)
            return -1;
        /* .bss zeroing: allocuvm gives zeroed pages; loaduvm only writes
           ph.filesz bytes.  Remainder of the last page stays zero. */
    }

    *p_entry = elf.entry;
    *p_sz = sz;
    return 0;
}
