# sactionOS Roadmap

## Completed

| Subsystem | Feature | Kernel | DLL |
|-----------|---------|--------|-----|
| **Foundation** | Build system (Meson+Ninja, Clang/LLVM) | x | x |
| | Cross-compile to i686-elf | x | x |
| | Kernel boots on QEMU with SMP | x | |
| **Object System** | object_header + vtable (object_ops) | x | |
| | Handle table (alloc/free/lookup) | x | |
| | Object lifecycle (acquire/release, refcounting) | x | |
| **Syscall** | Dispatch table (256 entries) | x | |
| | argint / argstr / argptr helpers | x | |
| | `SYS_CREATE_OBJECT` — device routing ("CON:") | x | |
| | `SYS_CLOSE` — handle_free | x | |
| | `SYS_WRITE` — handle_lookup + ops->write | x | |
| | `SYS_TERMINATE_PROCESS` — exit() | x | |
| **Console** | Console device object (obj_console) | x | |
| | `CreateFile("CON:")`, `WriteFile`, `CloseHandle` | | x |
| **ELF Loader** | `exec_elf_load` (parse headers, load segments) | x | |
| **Init** | First user process (init) prints banner | | x |
| **Library** | Sc* wrapper / public API two-layer split | | x |
| | `ExitProcess` API wrapper | | x |

## Upcoming

### Phase 1 — Disk File I/O
- `SYS_READ` — dispatch to object ops->read
- `obj_file` for disk-backed inode files
- `CreateFile` support for real paths (`/README`, ...)
- `ReadFile` for disk files
- `GetFileSize`

### Phase 2 — Process Management
- `SYS_CREATE_PROCESS` — `exec_elf_load` from FS
- `SYS_CREATE_THREAD`
- `CreateProcess` API wrapper
- `GetCurrentProcess` / `GetCurrentThread`

### Phase 3 — Memory Management
- `SYS_ALLOC_VMEM` / `SYS_FREE_VMEM`
- Expand user heap beyond init page
- `VirtualAlloc` / `VirtualFree` wrappers
- `malloc` / `free` in user space (on top of VirtualAlloc)

### Phase 4 — Synchronization
- `obj_event` (manual-reset / auto-reset)
- `obj_mutex`, `obj_semaphore`
- `SYS_WAIT_FOR_OBJECT` — block on handle
- `SYS_SIGNAL_OBJECT` — signal event/semaphore
- `CreateEvent`, `WaitForSingleObject`, etc.

### Phase 5 — Message Passing
- `obj_channel` kernel object
- `SYS_SEND_MSG` / `SYS_RECV_MSG` / `SYS_PEEK_MSG`
- `CreateChannel`, `GetMessage`, `PostMessage`, `PeekMessage`
- Message loop pattern (`MSG_QUIT`, filter, idle loop)

### Phase 6 — GUI / Shell
- Framebuffer console
- Window manager on top of Channel message passing
- Interactive shell
