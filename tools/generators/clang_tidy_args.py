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
cross-compiler and reuses that entry's actual compile flags (target
triple, -mcpu/-mfpu/-mthumb, defines, -std, etc.) to ask the compiler for
its default include search paths, then emits
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
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

# Flags to drop when turning a real compile command into an "ask the
# compiler for its default include dirs" probe command: we want the same
# target/std/define flags, but not the ones that control compiling an
# actual object file.
_DROP_FLAG = {"-c"}
_DROP_FLAG_WITH_VALUE = {"-o", "-MF", "-MT", "-MQ"}
_DROP_VALUELESS_DEP_FLAGS = {"-MD", "-MMD"}

_SEARCH_START_RE = re.compile(r"search starts here:?\s*$")
_SEARCH_END_RE = re.compile(r"^End of search list\.?\s*$")
_FRAMEWORK_SUFFIX_RE = re.compile(r"\s*\(framework directory\)\s*$")


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


def _probe_args_from_compile_entry(entry: dict) -> list[str]:
    """Strip the "compile an object file" parts of a compile_commands.json
    entry's command, leaving the compiler + target/std/define flags so we
    can reuse them to probe default include dirs (`-E -Wp,-v`).
    """
    argv = shlex.split(entry["command"])
    source_file = entry.get("file", "")

    kept: list[str] = []
    skip_next = False
    for tok in argv:
        if skip_next:
            skip_next = False
            continue
        if tok in _DROP_FLAG_WITH_VALUE:
            skip_next = True
            continue
        if tok in _DROP_FLAG or tok in _DROP_VALUELESS_DEP_FLAGS:
            continue
        if tok == source_file:
            continue
        kept.append(tok)
    return kept


def _parse_include_dirs(cpp_verbose_output: str) -> list[str]:
    """Parse the `#include <...> search starts here` block that GCC/Clang
    print with `-Wp,-v` (or plain `-v`).
    """
    dirs: set[str] = set()
    in_search_list = False
    for line in cpp_verbose_output.splitlines():
        if _SEARCH_START_RE.search(line):
            in_search_list = True
            continue
        if _SEARCH_END_RE.match(line.strip()):
            in_search_list = False
            continue
        if in_search_list:
            cleaned = _FRAMEWORK_SUFFIX_RE.sub("", line.strip())
            if cleaned:
                dirs.add(cleaned)
    return sorted(dirs)


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

    entry = db[0]
    compiler = shlex.split(entry["command"])[0]
    if not shutil.which(compiler) and not Path(compiler).exists():
        print(f"error: compiler '{compiler}' not found", file=sys.stderr)
        sys.exit(1)

    probe_cmd = [*_probe_args_from_compile_entry(entry), "-E", "-Wp,-v", "-x", "c++", "-"]
    result = subprocess.run(
        probe_cmd,
        input="",
        capture_output=True,
        text=True,
        cwd=entry.get("directory"),
    )

    args = ["--extra-arg=--target=arm-none-eabi"]
    for inc_dir in _parse_include_dirs(result.stdout + result.stderr):
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
