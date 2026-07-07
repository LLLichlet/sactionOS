# saction32 Public API Reference

All functions are declared in `include/saction_api.h`.
User programs link against `libsaction.a`.

`[+]` = implemented, `[ ]` = planned.

## Process / Thread

```
[+] void   ExitProcess(int exit_code)
[ ] HANDLE CreateProcess(const wchar_t *path, uint flags, HANDLE *hThread)
[ ] HANDLE GetCurrentProcess(void)
[ ] HANDLE GetCurrentThread(void)
```

`ExitProcess` terminates the calling process.  On return from the
first syscall the process becomes ZOMBIE and its parent is woken.

## Memory

```
[ ] void  *VirtualAlloc(void *addr, uint size, uint alloc_type, uint protect)
[ ] int    VirtualFree(void *addr, uint size, uint free_type)
```

Flags: `MEM_COMMIT`, `MEM_RESERVE`, `MEM_RELEASE`.
Protection: `PAGE_READONLY`, `PAGE_READWRITE`, `PAGE_EXECUTE_READWRITE`.

## File I/O

```
[+] HANDLE CreateFile(const wchar_t *path, uint access, uint share,
                      uint create_disp, uint flags)
[ ] int    ReadFile(HANDLE h, void *buf, uint size, uint *bytes_read)
[+] int    WriteFile(HANDLE h, const void *buf, uint size, uint *bytes_written)
[+] int    CloseHandle(HANDLE h)
[ ] int    GetFileSize(HANDLE h, uint *size)
```

`CreateFile` currently supports the device name `"CON:"` (console).
Disk file support is planned.  `WriteFile` returns 0 on success,
non-zero on failure; `*bytes_written` receives the byte count.

Access masks: `GENERIC_READ` (0x80000000), `GENERIC_WRITE` (0x40000000).

Creation disposition: `CREATE_NEW`, `OPEN_EXISTING`, `CREATE_ALWAYS`, etc.

`INVALID_HANDLE_VALUE` (`(HANDLE)-1`) signals failure.

## Synchronization

```
[ ] HANDLE CreateEvent(int manual_reset, int initial_state)
[ ] int    SetEvent(HANDLE h)
[ ] int    ResetEvent(HANDLE h)
[ ] int    WaitForSingleObject(HANDLE h, uint timeout_ms)
```

`timeout_ms` of `INFINITE` (0xFFFFFFFF) blocks forever.
Returns `WAIT_OBJECT_0` (0) on success, `WAIT_TIMEOUT` on expiry.

## Message Passing

```
[ ] HANDLE CreateChannel(const wchar_t *name)
[ ] int    SendMessage(HANDLE ch, uint msg, uint wParam, int lParam)
[ ] int    GetMessage(MSG *out, HANDLE ch, uint filter_min, uint filter_max)
[ ] int    PostMessage(HANDLE ch, uint msg, uint wParam, int lParam)
[ ] int    PeekMessage(MSG *out, HANDLE ch, uint filter_min, uint filter_max, uint flags)
```

**MSG structure:**
```
typedef struct {
    HANDLE hwnd;
    uint   message;   // MSG_KEYDOWN, MSG_QUIT, MSG_USER, ...
    uint   wParam;
    int    lParam;
    uint   time;
    uint   reserved[2];
} MSG;
```

Peek flags: `PM_REMOVE` (dequeue), `PM_NOYIELD`.

Standard message IDs reserved below `MSG_USER` (0x0400): `MSG_CREATE`,
`MSG_DESTROY`, `MSG_CLOSE`, `MSG_QUIT`, `MSG_PAINT`, `MSG_KEYDOWN`,
`MSG_CHAR`, `MSG_MOUSEMOVE`, `MSG_LBUTTONDOWN`, etc.

## Constants

```
GENERIC_READ       0x80000000    PAGE_NOACCESS        0x01
GENERIC_WRITE      0x40000000    PAGE_READONLY        0x02
GENERIC_EXECUTE    0x20000000    PAGE_READWRITE       0x04
GENERIC_ALL        0x10000000    PAGE_EXECUTE_READWRITE 0x40

CREATE_NEW         1             WAIT_OBJECT_0        0x00000000
CREATE_ALWAYS      2             WAIT_TIMEOUT         0x00000102
OPEN_EXISTING      3             WAIT_FAILED          0xFFFFFFFF
OPEN_ALWAYS        4             INFINITE             0xFFFFFFFF
TRUNCATE_EXISTING  5

MEM_COMMIT         0x00001000    INVALID_HANDLE_VALUE ((HANDLE)-1)
MEM_RESERVE        0x00002000
MEM_RELEASE        0x00008000
```
