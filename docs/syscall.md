# Internal Syscall Reference

These are **not** part of the public API.  User programs never see
these numbers.  The `Sc*` wrappers in `user/lib/syswrap.S` are the
only callers.

`[+]` = implemented, `[ ]` = planned.

## Object Management (0–9)

```
[+] 0  SYS_CREATE_OBJECT     (type, name, access) → handle
                               Routes "CON:" → obj_console_create()
[+] 1  SYS_CLOSE              (handle) → frees handle, releases object
[ ] 2  SYS_DUPLICATE_HANDLE   (src_handle, dst_proc, access, flags)
[ ] 3  SYS_QUERY_INFO         (handle, info_class, buf, size)
[ ] 4  SYS_SET_INFO           (handle, info_class, buf, size)
```

`SYS_CREATE_OBJECT` allocates a kernel object of the given type,
optionally names it, and returns an opaque handle index.

## Process / Thread (10–19)

```
[+] 12 SYS_TERMINATE_PROCESS  (exit_code) → calls exit()
[ ] 10 SYS_CREATE_PROCESS     (path, flags, hThread_out)
[ ] 11 SYS_CREATE_THREAD      (entry, arg, flags, hThread_out)
[ ] 13 SYS_TERMINATE_THREAD   (exit_code)
[ ] 14 SYS_GET_CURRENT_PROCESS → returns process pseudo-handle
[ ] 15 SYS_GET_CURRENT_THREAD  → returns thread pseudo-handle
```

## Memory (20–29)

```
[ ] 20 SYS_ALLOC_VMEM         (addr, size, alloc_type, protect)
[ ] 21 SYS_FREE_VMEM          (addr, size, free_type)
[ ] 22 SYS_PROTECT_VMEM       (addr, size, protect, old_protect_out)
```

## File / Device I/O (30–39)

```
[+] 30 SYS_WRITE              (handle, buf_addr, size) → calls ops->write
[ ] 31 SYS_READ               (handle, buf_addr, size) → calls ops->read
[ ] 32 SYS_IO_CONTROL         (handle, code, inbuf, outbuf)
```

## Synchronization (40–49)

```
[ ] 40 SYS_WAIT_FOR_OBJECT    (handle, timeout_ms) → blocks until signalled
[ ] 41 SYS_SIGNAL_OBJECT      (handle, arg) → signals an event/semaphore
```

## Message Passing (50–59)

```
[ ] 50 SYS_SEND_MSG           (channel, msg, wParam, lParam) → sync send
[ ] 51 SYS_RECV_MSG           (channel, msg_out) → blocking receive
[ ] 52 SYS_PEEK_MSG           (channel, msg_out, flags) → non-blocking poll
```

## Argument Passing

Arguments are on the user stack (cdecl convention).  The kernel reads
them via:

```
argint(n, &val)   — nth 32-bit integer argument
argstr(n, &ptr)   — nth string pointer (fetchstr into user address space)
argptr(n, &ptr, size) — nth pointer with bounds check
```

These rely on `tf->esp` pointing to the user stack after the `int`
instruction pushed SS, ESP, EFLAGS, CS, EIP.  `Sc*` wrappers must be
assembly (no C prologue) to avoid shifting this frame.
