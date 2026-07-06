/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * syscall -- System call dispatch and argument helpers.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

/* Fetch the int at addr from the current process. */
int
fetchint(uint addr, int *ip)
{
    struct proc *curproc = myproc();

    if (addr >= curproc->sz || addr + 4 > curproc->sz)
        return -1;
    *ip = *(int *)(addr);
    return 0;
}

/* Fetch the nul-terminated string at addr from the current process.
   Returns length of string, not including nul. *pp points into the
   process address space. */
int
fetchstr(uint addr, char **pp)
{
    char *s, *ep;
    struct proc *curproc = myproc();

    if (addr >= curproc->sz)
        return -1;
    *pp = (char *)addr;
    ep = (char *)curproc->sz;
    for (s = *pp; s < ep; s++) {
        if (*s == 0)
            return s - *pp;
    }
    return -1;
}

/* Fetch the nth 32-bit system call argument. */
int
argint(int n, int *ip)
{
    return fetchint((myproc()->tf->esp) + 4 + 4 * n, ip);
}

/* Fetch the nth word-sized system call argument as a pointer
   to a block of memory of size bytes.  Checks bounds. */
int
argptr(int n, char **pp, int size)
{
    int i;
    struct proc *curproc = myproc();

    if (argint(n, &i) < 0)
        return -1;
    if (size < 0 || (uint)i >= curproc->sz || (uint)i + size > curproc->sz)
        return -1;
    *pp = (char *)i;
    return 0;
}

/* Fetch the nth system call argument as a string pointer. */
int
argstr(int n, char **pp)
{
    int addr;
    if (argint(n, &addr) < 0)
        return -1;
    return fetchstr(addr, pp);
}

void
syscall(void)
{
    struct proc *curproc = myproc();
    int num = curproc->tf->eax;

    cprintf("syscall: unknown call %d from pid %s\n", num, curproc->name);
    curproc->killed = 1;
}
