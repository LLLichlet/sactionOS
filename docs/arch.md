# sactionOS Architecture

## Layers

```
User programs (init, shell, ...)
    |  call public API  (#include "saction_api.h")
    v
saction32 (user/lib/*.c)       -- validation, encoding, orchestration
    |  call Sc* wrappers
    v
syswrap.S                       -- Sc* stubs: movl $num,%eax; int $0x40
========== ring 3 | ring 0 ==========
syscall.c                       -- dispatch table, argint/argstr/fetchint
    |  call object ops vtable
    v
object.c                        -- handle table, obj_acquire/release
obj_console.c / obj_file.c ...  -- concrete object types
```

**Rules:**
- User code never issues `int $0x40` directly.
- `Sc*` functions in `syswrap.S` are internal to the library.
- Public API functions (in `*.c`) validate and translate before calling `Sc*`.
- The kernel never exposes syscall numbers to user space.

## Object Model

Every kernel resource is an **object** with a common header:

```
struct object_header {
    struct object_ops *ops;  // vtable
    enum objtype type;       // OBJ_PROCESS, OBJ_FILE, OBJ_EVENT, ...
    uint refcnt;             // reference count
    uint flags;              // OBJ_FLAG_NAMED, ...
    wchar_t *name;           // NULL for unnamed objects
};
```

Concrete objects (console, file, process, event) embed this header as
their first field.  Each object type provides implementations for the
`object_ops` vtable:

```
struct object_ops {
    void (*close)(...);
    int  (*read)(...);
    int  (*write)(...);
    int  (*signal)(...);
    int  (*query)(...);
    int  (*set)(...);
};
```

Unsupported operations are NULL; dispatch returns `-1`.

## Handle Table

Per-process array of `struct handle_entry` (`MAX_HANDLES = 64`):

```
handle_entry {
    struct object_header *obj;
    uint access;   // GENERIC_READ | GENERIC_WRITE | ...
    uint flags;    // HANDLE_FLAG_INHERIT, PROTECT
};
```

Handles are opaque `void *` values: `(void *)(uintptr_t)(index + 1)`.
NULL (`0`) means invalid handle.  `handle_alloc` / `handle_free` /
`handle_lookup` manage the table.

## Memory Layout

```
0x00000000 +-- user text / rodata / data / bss
           +-- user stack (grows down)
           +-- guard page
           +-- user heap (grows up)
0x80000000 +-- kernel space (KERNBASE)
```

## Boot Sequence

1. BIOS → bootblock (0x7C00) → kernel ELF at 0x100000
2. `entry.S` enables paging (4MB large pages), jumps to `main()`
3. `main()` initializes hardware, creates first process via `userinit()`
4. `userinit()` embeds the init binary, sets up page table and trapframe
5. `scheduler()` picks init process; `forkret()` mounts root FS
6. `trapret` drops to ring 3; init calls `CreateFile("CON:")`, prints banner, idles

## Message Passing (planned)

Multi-Channel model inspired by Windows message queues and
MessageC's queue/target pattern:

- `CreateChannel(name)` creates a named kernel Channel object.
- `SendMessage` is synchronous (blocks until receiver processes).
- `PostMessage` is asynchronous (enqueue, return immediately).
- `GetMessage` blocks; `PeekMessage` polls.

Channels are the foundation for the future GUI subsystem.
