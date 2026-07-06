/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * saction_api -- Public API for user-space programs (saction32.dll surface).
 *
 * This header defines the Windows-style API that all user programs use.
 * Syscalls are hidden behind this layer -- never call int 0x40 directly.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#ifndef SACTION_API_H
#define SACTION_API_H

#include "types.h"

typedef void *HANDLE;

#define INVALID_HANDLE_VALUE ((HANDLE)(uintptr_t)-1)

#define GENERIC_READ    0x80000000
#define GENERIC_WRITE   0x40000000
#define GENERIC_EXECUTE 0x20000000
#define GENERIC_ALL     0x10000000

#define FILE_SHARE_READ  0x00000001
#define FILE_SHARE_WRITE 0x00000002
#define FILE_SHARE_DELETE 0x00000004

#define CREATE_NEW        1
#define CREATE_ALWAYS     2
#define OPEN_EXISTING     3
#define OPEN_ALWAYS       4
#define TRUNCATE_EXISTING 5

#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_FLAG_OVERLAPPED  0x40000000

#define MEM_COMMIT  0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_RELEASE 0x00008000

#define PAGE_NOACCESS          0x01
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_EXECUTE           0x10
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40

#define WAIT_OBJECT_0 0x00000000
#define WAIT_TIMEOUT  0x00000102
#define WAIT_FAILED   0xFFFFFFFF
#define INFINITE 0xFFFFFFFF

#define CREATE_SUSPENDED 0x00000001
#define DETACHED_PROCESS 0x00000008

typedef struct {
    HANDLE hwnd;
    uint message;
    uint wParam;
    int lParam;
    uint time;
    uint reserved[2];
} MSG;

#define MSG_NULL         0x0000
#define MSG_CREATE       0x0001
#define MSG_DESTROY      0x0002
#define MSG_CLOSE        0x0010
#define MSG_QUIT         0x0012
#define MSG_PAINT        0x000F
#define MSG_KEYDOWN      0x0100
#define MSG_KEYUP        0x0101
#define MSG_CHAR         0x0102
#define MSG_MOUSEMOVE    0x0200
#define MSG_LBUTTONDOWN  0x0201
#define MSG_LBUTTONUP    0x0202
#define MSG_RBUTTONDOWN  0x0204
#define MSG_RBUTTONUP    0x0205
#define MSG_TIMER        0x0113
#define MSG_USER         0x0400

#define PM_REMOVE  0x0001
#define PM_NOYIELD 0x0002

HANDLE CreateProcess(const wchar_t *path, uint flags, HANDLE *hThread);
void   ExitProcess(int exit_code);
HANDLE GetCurrentProcess(void);
HANDLE GetCurrentThread(void);

void  *VirtualAlloc(void *addr, uint size, uint alloc_type, uint protect);
int    VirtualFree(void *addr, uint size, uint free_type);

HANDLE CreateFile(const wchar_t *path, uint access, uint share,
                  uint create_disp, uint flags);
int    ReadFile(HANDLE h, void *buf, uint size, uint *bytes_read);
int    WriteFile(HANDLE h, const void *buf, uint size, uint *bytes_written);
int    CloseHandle(HANDLE h);
int    GetFileSize(HANDLE h, uint *size);

HANDLE CreateEvent(int manual_reset, int initial_state);
int    SetEvent(HANDLE h);
int    ResetEvent(HANDLE h);
int    WaitForSingleObject(HANDLE h, uint timeout_ms);

HANDLE CreateChannel(const wchar_t *name);
int    SendMessage(HANDLE ch, uint msg, uint wParam, int lParam);
int    GetMessage(MSG *out, HANDLE ch, uint filter_min, uint filter_max);
int    PostMessage(HANDLE ch, uint msg, uint wParam, int lParam);
int    PeekMessage(MSG *out, HANDLE ch, uint filter_min, uint filter_max, uint flags);

/* Temporary: will be replaced by console subsystem */
void DebugPrint(const char *msg);

#endif
