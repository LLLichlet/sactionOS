project(
  'sactionOS',
  'c',
  version: '0.1.0',
  default_options: [
    'buildtype=debug',
    'warning_level=3',
    'c_std=gnu99',
    'optimization=2',
  ],
  meson_version: '>=1.0.0',
)

# ============================================================================
# Part 1: Tool discovery
# ============================================================================

prog_objcopy = find_program('llvm-objcopy')
prog_objdump = find_program('llvm-objdump')
prog_ld      = find_program('ld.lld')
prog_python  = find_program('python3', 'python')

root_inc = include_directories('include')

scripts_dir = meson.current_source_dir() / 'scripts'

# ============================================================================
# Part 2: Compiler flags
# ============================================================================

# Cross-compile to i686-elf bare metal.
# nostdinc: we provide all headers in include/ (no libc headers needed).
xv6_base_c_args = [
  '--target=i686-elf',
  '-m32',
  '-ffreestanding',
  '-nostdinc',
  '-fno-pic',
  '-fno-builtin',
  '-fno-strict-aliasing',
  '-Wall',
  '-ggdb',
  '-Wno-unused-command-line-argument',
  '-fno-omit-frame-pointer',
  '-mno-mmx',
  '-mno-sse',
]

# -O2 for kernel and user programs
xv6_o2_c_args = xv6_base_c_args + ['-O2']

# -Os for bootblock (must fit in 510 bytes)
xv6_os_c_args = xv6_base_c_args + ['-Os']

# Optional flags -- detect support but don't fail if missing
cc = meson.get_compiler('c')
foreach opt : ['-fno-stack-protector']
  if cc.has_argument(opt)
    xv6_o2_c_args += opt
    xv6_os_c_args += opt
  endif
endforeach

# ============================================================================
# Part 3: Generated sources (vectors.S)
# ============================================================================

gen_vectors = custom_target('vectors.S',
  output: 'vectors.S',
  command: [prog_python, scripts_dir / 'gen_vectors.py'],
  capture: true,
)

# ============================================================================
# Part 4: User-space library (saction32)
# ============================================================================

# 4a: syscall wrappers (assembly -- must not have C prologue)
libsaction = static_library('saction',
  'user/lib/syswrap.S',
  c_args: xv6_o2_c_args,
  include_directories: root_inc,
  pic: false,
)

# ============================================================================
# Part 5: Kernel
# ============================================================================

# 5a: Entry object (must come first in link order)
entry_lib = static_library('entry_obj',
  'kernel/entry.S',
  c_args: xv6_o2_c_args,
  include_directories: root_inc,
  pic: false,
)

# 4b: Kernel core objects (all .c + asm + generated vectors)
kernel_core_lib = static_library('kernel_core_objs',
  # Kernel C sources
  'kernel/bio.c',
  'kernel/console.c',
  'kernel/exec.c',
  'kernel/file.c',
  'kernel/fs.c',
  'kernel/ide.c',
  'kernel/ioapic.c',
  'kernel/kalloc.c',
  'kernel/kbd.c',
  'kernel/lapic.c',
  'kernel/log.c',
  'kernel/main.c',
  'kernel/object.c',
  'kernel/mp.c',
  'kernel/picirq.c',
  'kernel/proc.c',
  'kernel/sleeplock.c',
  'kernel/spinlock.c',
  'kernel/string.c',
  'kernel/syscall.c',
  'kernel/trap.c',
  'kernel/uart.c',
  'kernel/vm.c',
  # Kernel assembly (excluding entry.S)
  'kernel/swtch.S',
  'kernel/trapasm.S',
  # Generated interrupt vectors
  gen_vectors,
  c_args: xv6_o2_c_args,
  include_directories: root_inc,
  pic: false,
)

# 4c: entryother (AP CPU startup code, embedded as raw binary)
entryother_lib = static_library('entryother_obj',
  'kernel/entryother.S',
  c_args: xv6_o2_c_args,
  include_directories: root_inc,
  pic: false,
)

entryother_elf = custom_target('entryother_elf',
  output: 'entryother_elf',
  input: entryother_lib.extract_all_objects(recursive: false),
  command: [prog_ld, '-m', 'elf_i386', '--image-base', '0',
            '-N', '-e', 'start', '-Ttext', '0x7000',
            '-o', '@OUTPUT@', '@INPUT@'],
)

entryother_bin = custom_target('entryother',
  output: 'entryother',
  input: entryother_elf,
  command: [prog_objcopy, '-S', '-O', 'binary', '-j', '.text',
            '@INPUT@', '@OUTPUT@'],
)

# 4d: init (first user process, compiled C + libsaction, embedded as raw binary)
init_start_lib = static_library('init_start_obj',
  'user/init/start.S',
  c_args: xv6_o2_c_args,
  include_directories: root_inc,
  pic: false,
)

init_main_lib = static_library('init_main_obj',
  'user/init/init.c',
  c_args: xv6_o2_c_args,
  include_directories: root_inc,
  pic: false,
)

init_elf = custom_target('init_elf',
  output: 'init_elf',
  input: [
    init_start_lib.extract_all_objects(recursive: false),
    init_main_lib.extract_all_objects(recursive: false),
    libsaction.extract_all_objects(recursive: false),
  ],
  command: [prog_ld, '-m', 'elf_i386', '--image-base', '0',
            '-N', '-e', '_start', '-Ttext', '0',
            '-o', '@OUTPUT@', '@INPUT@'],
  depends: [libsaction],
)

initcode_bin = custom_target('initcode',
  output: 'initcode',
  input: init_elf,
  command: [prog_objcopy, '-S', '-O', 'binary',
            '@INPUT@', '@OUTPUT@'],
)

# 4e: Kernel ELF -- link via ld.lld with kernel.ld, embedding binary blobs
kernel_ld_script = meson.current_source_dir() / 'kernel.ld'

kernel_elf = custom_target('kernel',
  build_by_default: true,
  output: 'kernel',
  input: [
    entry_lib.extract_all_objects(recursive: false),
    kernel_core_lib.extract_all_objects(recursive: false),
    entryother_bin,
    initcode_bin,
  ],
  command: [
    prog_python, scripts_dir / 'link_kernel.py',
    '--ld', prog_ld.full_path(),
    '--script', kernel_ld_script,
    '--output', '@OUTPUT@',
    '@INPUT@',
  ],
  depends: [entry_lib, kernel_core_lib, entryother_bin, initcode_bin],
)

# Kernel disassembly (for debugging)
kernel_asm = custom_target('kernel.asm',
  output: 'kernel.asm',
  input: kernel_elf,
  command: [prog_objdump, '-S', '@INPUT@'],
  capture: true,
  build_by_default: false,
)

# ============================================================================
# Part 5: Bootblock
# ============================================================================

# Bootblock uses -O (not -O2) to stay within 510 bytes
bootasm_lib = static_library('bootasm_obj',
  'boot/bootasm.S',
  c_args: xv6_os_c_args,
  include_directories: root_inc,
  pic: false,
)

bootmain_lib = static_library('bootmain_obj',
  'boot/bootmain.c',
  c_args: xv6_os_c_args,
  include_directories: root_inc,
  pic: false,
)

bootblock_elf = custom_target('bootblock.o',
  output: 'bootblock.o',
  input: [
    bootasm_lib.extract_all_objects(recursive: false),
    bootmain_lib.extract_all_objects(recursive: false),
  ],
  command: [prog_ld, '-m', 'elf_i386', '--image-base', '0',
            '-N', '-e', 'start', '-Ttext', '0x7C00',
            '-o', '@OUTPUT@', '@INPUT@'],
)

bootblock_raw = custom_target('bootblock_unsigned',
  output: 'bootblock_unsigned',
  input: bootblock_elf,
  command: [prog_objcopy, '-S', '-O', 'binary', '-j', '.text',
            '@INPUT@', '@OUTPUT@'],
)

bootblock = custom_target('bootblock',
  build_by_default: true,
  output: 'bootblock',
  input: bootblock_raw,
  command: [prog_python, scripts_dir / 'sign.py', '@INPUT@', '@OUTPUT@'],
)

# ============================================================================
# Part 6: User programs -- TODO: build additional user ELFs and add to fs.img
# ============================================================================



# ============================================================================
# Part 7: Host tools (mkfs)
# ============================================================================

# mkfs runs on the BUILD machine, not the target -- use native: true.
# It needs xv6 headers (types.h, fs.h, stat.h, param.h) but must NOT pick up
# xv6 versions of system headers (fcntl.h, stat.h).  Use -idirafter so the
# system include dirs win when the same header name exists in both places.
mkfs_exe = executable('mkfs',
  'tools/mkfs.c',
  native: true,
  c_args: [
    '-Wall',
    '-Wno-error=implicit-function-declaration',
    '-Wno-builtin-declaration-mismatch',
    '-Wno-sign-compare',
    '-idirafter', meson.current_source_dir() / 'include',
    '-include', meson.current_source_dir() / 'tools' / 'compat.h',
  ],
)

# ============================================================================
# Part 8: Filesystem image + disk image
# ============================================================================

fs_img = custom_target('fs.img',
  output: 'fs.img',
  command: [
    prog_python, '-c',
    'import shutil, subprocess, sys;
shutil.copy2(sys.argv[1], "README.md");
subprocess.run(sys.argv[2:])',
    '@CURRENT_SOURCE_DIR@/README.md',
    mkfs_exe,
    '@OUTPUT@',
    'README.md',
  ],
  depends: [mkfs_exe],
  build_by_default: true,
)

xv6_img = custom_target('xv6.img',
  output: 'xv6.img',
  input: [bootblock, kernel_elf],
  command: [
    prog_python, scripts_dir / 'make_disk.py',
    '--output', '@OUTPUT@',
    '--bootblock', '@INPUT0@',
    '--kernel', '@INPUT1@',
  ],
  depends: [bootblock, kernel_elf],
  build_by_default: true,
)

# ============================================================================
# Part 9: Run targets + kernelmemfs (optional)
# ============================================================================

qemu = get_option('qemu_binary')
smp = get_option('smp_cpus')
mem = get_option('qemu_memory')
qemu_extra = get_option('qemu_extra_args')

qemu_base_opts = [
  '-drive', 'file=' + fs_img.full_path() + ',index=1,media=disk,format=raw',
  '-drive', 'file=' + xv6_img.full_path() + ',index=0,media=disk,format=raw',
  '-smp', smp.to_string(),
  '-m', mem,
]

if qemu_extra != ''
  qemu_base_opts += qemu_extra.split()
endif

prog_clang_format = find_program('clang-format', required: false)

if prog_clang_format.found()
  run_target('format',
    command: [
      prog_python, scripts_dir / 'run_clang_format.py',
      '--src-root', meson.current_source_dir(),
      '--clang-format', prog_clang_format.full_path(),
    ],
  )

  run_target('check-format',
    command: [
      prog_python, scripts_dir / 'run_clang_format.py',
      '--src-root', meson.current_source_dir(),
      '--clang-format', prog_clang_format.full_path(),
      '--check',
    ],
  )
endif

run_target('qemu',
  command: [qemu, '-serial', 'mon:stdio'] + qemu_base_opts,
  depends: [fs_img, xv6_img],
)

run_target('qemu-nox',
  command: [qemu, '-nographic'] + qemu_base_opts,
  depends: [fs_img, xv6_img],
)

# ============================================================================
# Part 10: kernelmemfs variant (optional)
# ============================================================================

if get_option('build_kernelmemfs')
  memide_lib = static_library('memide_obj',
    'kernel/memide.c',
    c_args: xv6_o2_c_args,
    include_directories: root_inc,
    pic: false,
  )

  kernelmemfs_elf = custom_target('kernelmemfs',
    output: 'kernelmemfs',
    input: [
      entry_lib.extract_all_objects(recursive: false),
      kernel_core_lib.extract_all_objects(recursive: false),
      memide_lib.extract_all_objects(recursive: false),
      entryother_bin,
      initcode_bin,
      fs_img,
    ],
    command: [
      prog_python, scripts_dir / 'link_kernel_memfs.py',
      '--ld', prog_ld.full_path(),
      '--script', kernel_ld_script,
      '--output', '@OUTPUT@',
      '@INPUT@',
    ],
    depends: [entry_lib, kernel_core_lib, memide_lib,
              entryother_bin, initcode_bin, fs_img],
  )

  xv6memfs_img = custom_target('xv6memfs.img',
    output: 'xv6memfs.img',
    input: [bootblock, kernelmemfs_elf],
    command: [
      prog_python, scripts_dir / 'make_disk.py',
      '--output', '@OUTPUT@',
      '--bootblock', '@INPUT0@',
      '--kernel', '@INPUT1@',
    ],
    depends: [bootblock, kernelmemfs_elf],
  )

  run_target('qemu-memfs',
    command: [
      qemu,
      '-drive',
      'file=' + xv6memfs_img.full_path() + ',index=0,media=disk,format=raw',
      '-smp', smp.to_string(),
      '-m', '256',
    ],
    depends: [xv6memfs_img],
  )
endif
