#!/usr/bin/env python3
"""Create xv6 disk image: 5MB zero-filled with bootblock at sector 0,
kernel at sector 1.

Replaces: dd if=/dev/zero of=xv6.img count=10000
          dd if=bootblock of=xv6.img conv=notrunc
          dd if=kernel of=xv6.img seek=1 conv=notrunc

Usage: make_disk.py --output DISK_IMG --bootblock BOOTBLOCK --kernel KERNEL
"""

import argparse
import sys

SECTOR_SIZE = 512
TOTAL_SECTORS = 10000


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", required=True)
    parser.add_argument("--bootblock", required=True)
    parser.add_argument("--kernel", required=True)
    args = parser.parse_args()

    total_bytes = TOTAL_SECTORS * SECTOR_SIZE

    with open(args.output, "wb") as img:
        img.write(b"\x00" * total_bytes)

    with open(args.bootblock, "rb") as f:
        boot_data = f.read()

    with open(args.kernel, "rb") as f:
        kernel_data = f.read()

    with open(args.output, "r+b") as img:
        img.seek(0)
        img.write(boot_data)
        img.seek(SECTOR_SIZE)
        img.write(kernel_data)

    print(
        f"disk: {TOTAL_SECTORS} sectors, "
        f"bootblock {len(boot_data)} bytes at sector 0, "
        f"kernel {len(kernel_data)} bytes at sector 1",
        file=sys.stderr,
    )


if __name__ == "__main__":
    main()
