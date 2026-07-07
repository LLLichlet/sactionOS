/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * syswrap -- Internal Sc* syscall stubs.
 *
 * These are NOT part of the public API.  Each stub sets %eax to the
 * syscall number and issues int $T_SYSCALL.  Public wrappers in *.c call
 * these to perform the actual ring transition.
 *
 * Must be pure assembly -- a C prologue would push %ebp and shift the
 * user stack, breaking argint() argument offsets in the kernel.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "sysnum.h"
#include "traps.h"

#define SC_WRAP(name, num)  \
    .globl name;            \
    .type name, @function;  \
    name:                   \
        movl $num, %eax;    \
        int $T_SYSCALL;     \
        ret

SC_WRAP(ScDebugPrint,       SYS_DEBUG_PRINT)
SC_WRAP(ScTerminateProcess, SYS_TERMINATE_PROCESS)
