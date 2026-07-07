/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * sysnum -- Internal system call numbers.
 *
 * These are NOT part of the public API.  User programs call saction32.dll
 * wrappers; the DLL translates to these numbers for int 0x40.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#ifndef SACTION_SYSNUM_H
#define SACTION_SYSNUM_H

/* Object management */
#define SYS_CREATE_OBJECT 0
#define SYS_CLOSE 1
#define SYS_DUPLICATE_HANDLE 2
#define SYS_QUERY_INFO 3
#define SYS_SET_INFO 4

/* Process / Thread */
#define SYS_CREATE_PROCESS 10
#define SYS_CREATE_THREAD 11
#define SYS_TERMINATE_PROCESS 12
#define SYS_TERMINATE_THREAD 13
#define SYS_GET_CURRENT_PROCESS 14
#define SYS_GET_CURRENT_THREAD 15

/* Memory */
#define SYS_ALLOC_VMEM 20
#define SYS_FREE_VMEM 21
#define SYS_PROTECT_VMEM 22

/* File / Device I/O */
#define SYS_READ 30
#define SYS_WRITE 31
#define SYS_IO_CONTROL 32

/* Synchronization */
#define SYS_WAIT_FOR_OBJECT 40
#define SYS_SIGNAL_OBJECT 41

/* Message passing */
#define SYS_SEND_MSG 50
#define SYS_RECV_MSG 51
#define SYS_PEEK_MSG 52

/* Debug (temporary) */
#define SYS_DEBUG_PRINT 255

#endif
