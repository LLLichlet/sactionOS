/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * syswrap -- Assembly syscall wrappers for saction32.dll.
 *
 * Each wrapper sets %eax to the syscall number and issues int $T_SYSCALL.
 * The function arguments are already on the stack (cdecl); the kernel reads
 * them via argint / argstr from the user stack without further marshalling.
 *
 * These MUST be in assembly -- a C prologue would push %ebp and shift the
 * argument positions, breaking the kernel's argint(n) offset calculation.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "sysnum.h"
#include "traps.h"

#define WRAP(name, num)   \
    .globl name;          \
    .type name, @function;\
    name:                 \
        movl $num, %eax;  \
        int $T_SYSCALL;   \
        ret

WRAP(DebugPrint,  SYS_DEBUG_PRINT)
WRAP(ExitProcess, SYS_TERMINATE_PROCESS)
