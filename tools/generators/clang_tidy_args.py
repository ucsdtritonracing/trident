#!/usr/bin/env python3
"""Print extra clang-tidy arguments for a given build target.

Usage:
    python3 tools/generators/clang_tidy_args.py host
    python3 tools/generators/clang_tidy_args.py stm32

For "host": on macOS, resolves the active SDK path via `xcrun` and emits
    --extra-arg=--sysroot=<sdk_path>
so clang-tidy can find the system headers. On other platforms it prints
nothing.

For "stm32": inspects build/stm32/compile_commands.json to find the
cross-compiler, asks it for its default include search paths, and emits
    --extra-arg=--target=arm-none-eabi
    --extra-arg=-isystem<dir>   (one per system include dir)
so clang-tidy can parse code compiled for the ARM target.

Args are printed space-separated on a single line, ready to be captured
into a shell variable, e.g.:
    stm32_tidy_args=$(python3 tools/generators/clang_tidy_args.py stm32)
"""

import argparse
import json
import platform
import shutil
import subprocess
import sys
from pathlib import Path


def host_args() -> list[str]:
    if platform.system() != "Darwin":
        return []

    try:
        sdk_path = subprocess.run(
            ["xcrun", "--show-sdk-path"],
            check=True,
            capture_output=True,
            text=True,
        ).stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return []

    if not sdk_path:
        return []

    return [f"--extra-arg=--sysroot={sdk_path}"]


def stm32_args(compile_commands_path: Path) -> list[str]:
    if not compile_commands_path.exists():
        print(
            f"error: {compile_commands_path} not found; run `just configure stm32` first",
            file=sys.stderr,
        )
        sys.exit(1)

    db = json.loads(compile_commands_path.read_text())
    if not db:
        print(f"error: {compile_commands_path} is empty", file=sys.stderr)
        sys.exit(1)

    compiler = db[0]["command"].split()[0]
    if not shutil.which(compiler) and not Path(compiler).exists():
        print(f"error: compiler '{compiler}' not found", file=sys.stderr)
        sys.exit(1)

    result = subprocess.run(
        [compiler, "-std=gnu++20", "-xc++", "-E", "-Wp,-v", "-"],
        input="",
        capture_output=True,
        text=True,
    )

    include_dirs: set[str] = set()
    in_search_list = False
    for line in (result.stdout + result.stderr).splitlines():
        if "search starts here" in line:
            in_search_list = True
            continue
        if "End of search list" in line:
            in_search_list = False
            continue
        if in_search_list:
            include_dirs.add(line.strip())

    args = ["--extra-arg=--target=arm-none-eabi"]
    for inc_dir in sorted(include_dirs):
        if inc_dir:
            args.append(f"--extra-arg=-isystem{inc_dir}")
    return args


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("target", choices=["host", "stm32"])
    args = parser.parse_args()

    if args.target == "host":
        extra_args = host_args()
    else:
        extra_args = stm32_args(Path("build/stm32/compile_commands.json"))

    print(" ".join(extra_args))


if __name__ == "__main__":
    main()
