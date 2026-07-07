#!/usr/bin/env python3
"""Run clang-format on all sactionOS C, H, and assembly sources."""

import argparse
import subprocess
import sys
from pathlib import Path

EXTENSIONS = {'.c', '.h'}

DIRS = ['kernel', 'boot', 'include', 'tools']


def run_fmt(src_root: Path, clang_fmt: str, check_only: bool, verbose: bool):
    args = [clang_fmt]
    if not check_only:
        args.append('-i')
    else:
        args.extend(['--dry-run', '-Werror'])

    files = []
    for d in DIRS:
        dpath = src_root / d
        if dpath.is_dir():
            for f in dpath.iterdir():
                if f.suffix in EXTENSIONS and f.is_file():
                    files.append(f)

    if not files:
        print('No source files found.')
        return 0

    cmd = args + [str(f) for f in files]
    if verbose:
        print(f'Running: {" ".join(cmd)}')

    result = subprocess.run(cmd, cwd=str(src_root))
    return result.returncode


def main():
    parser = argparse.ArgumentParser(description='Run clang-format on sactionOS sources')
    parser.add_argument('--check', action='store_true',
                        help='Check formatting without modifying (dry-run with -Werror)')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Print the command being run')
    parser.add_argument('--clang-format', default='clang-format',
                        help='Path to clang-format binary')
    parser.add_argument('--src-root', required=True,
                        help='Path to sactionOS source root')
    args = parser.parse_args()

    src_root = Path(args.src_root).resolve()
    if not src_root.is_dir():
        print(f'Error: {src_root} is not a directory')
        return 1

    return run_fmt(src_root, args.clang_format, args.check, args.verbose)


if __name__ == '__main__':
    sys.exit(main())
