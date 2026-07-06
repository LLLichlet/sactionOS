# sactionOS

A toy operating system for x86 multiprocessor machines, forked from
[xv6](https://github.com/mit-pdos/xv6-public) and rebuilt with a modern
LLVM-based toolchain.

It is a work-in-progress learning project. The kernel boots on QEMU and
initializes memory, processes, interrupts, and disk I/O. User space and
the system call interface are being redesigned.

## Building and Running

**Prerequisites:**

- Clang / LLVM (compiler, linker, objcopy)
- Meson >= 1.0 (build system)
- Ninja (build executor)
- Python 3 (build scripts)
- QEMU (emulator: `qemu-system-i386`)

**Configure and build:**

```sh
meson setup build --cross-file cross/i686_elf_clang.ini
ninja -C build
```

**Run in QEMU (no GUI):**

```sh
ninja -C build qemu-nox
```

**Run with GUI and SMP:**

```sh
meson configure build -Dsmp_cpus=4
ninja -C build qemu
```

**Build options** (`meson_options.txt`):

| Option              | Description                        | Default            |
| ------------------- | ---------------------------------- | ------------------ |
| `smp_cpus`          | Number of emulated CPUs            | `2`                |
| `qemu_memory`       | QEMU memory size in MB             | `512`              |
| `qemu_binary`       | Path to QEMU                       | `qemu-system-i386` |
| `build_kernelmemfs` | Build diskless kernelmemfs variant | `false`            |

## Source Layout

```
boot/        Boot loader (16-bit -> 32-bit -> kernel)
kernel/      Kernel: memory, processes, FS, drivers, syscalls
include/     Shared kernel headers
tools/       Host tools (mkfs)
scripts/     Build helper scripts
cross/       Meson cross-compilation definitions
```

## License

sactionOS is licensed under the **GNU General Public License v3.0**.
See [LICENSE](LICENSE) for the full text.

Copyright (C) 2026 LLLichlet

## Origin

sactionOS is based on xv6, a re-implementation of UNIX Version 6 developed
at MIT for the 6.828 operating systems course.

xv6 was created by Russ Cox, Frans Kaashoek, Robert Morris, and many
contributors. The original code is:

> Copyright (c) 2006-2018 Frans Kaashoek, Robert Morris, Russ Cox,
> Massachusetts Institute of Technology

xv6 borrows code from: JOS, Plan 9, FreeBSD, and NetBSD.

The upstream xv6 project has since moved to RISC-V:
<https://github.com/mit-pdos/xv6-riscv>
