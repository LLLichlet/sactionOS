#!/usr/bin/env python3
"""Link the xv6 kernel ELF with embedded binary blobs.

Usage: link_kernel.py --ld LD_PATH --script KERNEL_LD
        --output OUT [FILES ...]

All input files are scanned: .o files are linked as object files,
everything else is embedded via --format binary.  The entry object
(kernel/entry.S) must sort first alphabetically so it comes first
in the link order -- the linker processes files left to right.
"""

import argparse
import os
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ld", required=True)
    parser.add_argument("--script", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("files", nargs="*", default=[])
    args = parser.parse_args()

    obj_files = []
    blob_files = []

    for f in args.files:
        # Objects and blobs are distinguished by extension
        if f.endswith(".o"):
            obj_files.append(f)
        else:
            blob_files.append(f)

    # Sort objects so entry.S comes first (alphabetically "kernel_entry.S.o")
    obj_files.sort()

    cmd = [
        args.ld, "-m", "elf_i386", "--image-base", "0",
        "-T", args.script,
        "-o", args.output,
    ] + obj_files

    if blob_files:
        cmd.append("--format")
        cmd.append("binary")
        cmd.extend(blob_files)

    print(f"[link_kernel] {len(obj_files)} objs, {len(blob_files)} blobs",
          file=sys.stderr)
    result = subprocess.run(cmd)
    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
