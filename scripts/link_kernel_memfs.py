#!/usr/bin/env python3
"""Link the kernelmemfs variant: embed fs.img, replace ide.o with memide.o.

Usage: link_kernel_memfs.py --ld LD_PATH --script KERNEL_LD
        --output OUT [FILES ...]

Scan input files: .o files are objects, everything else is a binary blob.
Excludes ide.o, includes memide.o (both identified by filename).
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
        if f.endswith(".o"):
            # Exclude ide.o, keep everything else (including memide's objects)
            if os.path.basename(f) != "ide.o":
                obj_files.append(f)
        else:
            blob_files.append(f)

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

    print(f"[link_kernel_memfs] {len(obj_files)} objs, {len(blob_files)} blobs",
          file=sys.stderr)
    result = subprocess.run(cmd)
    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
