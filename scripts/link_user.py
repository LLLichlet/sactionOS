#!/usr/bin/env python3
"""Link an xv6 user program with ld.lld.

Usage: link_user.py --ld LD_PATH --output OUT --objs O1 O2 ...
Links at virtual address 0, entry point main, with -N (no page alignment).
"""

import argparse
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ld", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--objs", nargs="+", default=[])
    args = parser.parse_args()

    cmd = [
        args.ld, "-m", "elf_i386", "--image-base", "0",
        "-N", "-e", "main", "-Ttext", "0",
        "-o", args.output,
    ] + args.objs

    print(f"[link_user] {' '.join(cmd)}", file=sys.stderr)
    result = subprocess.run(cmd)
    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
