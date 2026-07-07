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
#include "sysnum.h"
#include "object.h"

typedef int (*syscall_fn)(void);

static int sys_debug_print(void);
static int sys_terminate_process(void);
static int sys_create_object(void);
static int sys_write(void);
static int sys_close(void);

static syscall_fn syscalls[256] = {
    [SYS_CREATE_OBJECT] = sys_create_object,
    [SYS_CLOSE] = sys_close,
    [SYS_WRITE] = sys_write,
    [SYS_TERMINATE_PROCESS] = sys_terminate_process,
    [SYS_DEBUG_PRINT] = sys_debug_print,
};

int fetchint(uint addr, int* ip)
{
    struct proc* curproc = myproc();

    if (addr >= curproc->sz || addr + 4 > curproc->sz)
        return -1;
    *ip = *(int*) (addr);
    return 0;
}

int fetchstr(uint addr, char** pp)
{
    char *s, *ep;
    struct proc* curproc = myproc();

    if (addr >= curproc->sz)
        return -1;
    *pp = (char*) addr;
    ep = (char*) curproc->sz;
    for (s = *pp; s < ep; s++) {
        if (*s == 0)
            return s - *pp;
    }
    return -1;
}

int argint(int n, int* ip)
{
    return fetchint((myproc()->tf->esp) + 4 + 4 * n, ip);
}

int argptr(int n, char** pp, int size)
{
    int i;
    struct proc* curproc = myproc();

    if (argint(n, &i) < 0)
        return -1;
    if (size < 0 || (uint) i >= curproc->sz || (uint) i + size > curproc->sz)
        return -1;
    *pp = (char*) i;
    return 0;
}

int argstr(int n, char** pp)
{
    int addr;
    if (argint(n, &addr) < 0)
        return -1;
    return fetchstr(addr, pp);
}

void syscall(void)
{
    struct proc* curproc = myproc();
    int num = curproc->tf->eax;

    if (num >= 0 && num < 256 && syscalls[num]) {
        int ret = syscalls[num]();
        curproc->tf->eax = ret;
        return;
    }

    cprintf("syscall: unknown call %d from pid %d\n", num, curproc->pid);
    curproc->killed = 1;
}

static int sys_debug_print(void)
{
    char* str;

    if (argstr(0, &str) < 0)
        return -1;
    cprintf("%s", str);
    return 0;
}

static int sys_terminate_process(void)
{
    exit();
    return 0;
}

static int sys_create_object(void)
{
    uint type, access;
    char* name;
    struct object_header* obj;
    void* h;

    if (argint(0, (int*) &type) < 0)
        return -1;
    if (argstr(1, &name) < 0)
        return -1;
    if (argint(2, (int*) &access) < 0)
        return -1;

    if (type == OBJ_FILE && strncmp(name, "CON:", 4) == 0) {
        extern struct object_header* obj_console_create(void);
        obj = obj_console_create();
    } else {
        return -1;
    }

    if (!obj)
        return -1;
    h = handle_alloc(myproc()->handles, obj, access, 0);
    if (!h) {
        if (obj->ops->close)
            obj->ops->close(obj);
        return -1;
    }
    return (int) (uintptr_t) h;
}

static int sys_write(void)
{
    int handle_val;
    uint buf_addr, size;
    struct object_header* obj;
    void* h;

    if (argint(0, &handle_val) < 0)
        return -1;
    if (argint(1, (int*) &buf_addr) < 0)
        return -1;
    if (argint(2, (int*) &size) < 0)
        return -1;

    if (buf_addr + size < buf_addr || buf_addr + size > myproc()->sz)
        return -1;

    h = (void*) (uintptr_t) handle_val;
    obj = handle_lookup(myproc()->handles, h);
    if (!obj || !obj->ops->write)
        return -1;

    return obj->ops->write(obj, (const char*) buf_addr, size, NULL);
}

static int sys_close(void)
{
    int handle_val;

    if (argint(0, &handle_val) < 0)
        return -1;
    handle_free(myproc()->handles, (void*) (uintptr_t) handle_val);
    return 0;
}
