#!/usr/bin/env python3
"""Sign a bootblock binary: pad to 510 bytes, append 0x55 0xAA.

Replaces sign.pl.
Usage: sign.py INPUT OUTPUT
"""

import sys


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} INPUT OUTPUT", file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1], "rb") as f:
        data = f.read()

    if len(data) > 510:
        print(f"ERROR: boot block too large: {len(data)} bytes (max 510)",
              file=sys.stderr)
        sys.exit(1)

    print(f"boot block is {len(data)} bytes (max 510)", file=sys.stderr)

    padded = data + b"\x00" * (510 - len(data)) + b"\x55\xAA"

    with open(sys.argv[2], "wb") as f:
        f.write(padded)


if __name__ == "__main__":
    main()
