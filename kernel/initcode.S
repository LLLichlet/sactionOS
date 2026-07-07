#include "sysnum.h"
#include "traps.h"

.globl start
start:
    pushl $msg
    pushl $0
    movl $SYS_DEBUG_PRINT, %eax
    int $T_SYSCALL

loop:
    jmp loop

msg:
    .string "Hello from sactionOS init!\n"
