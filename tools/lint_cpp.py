#!/usr/bin/env python3
"""C++ formatting/lint: clang-format + clang-tidy (host and stm32 targets).

File discovery has one source of truth: find_files(), the same filesystem
walk clang-format uses. The compile databases are only consulted to
figure out (a) which target(s) a given file was compiled for, and (b)
what flags to hand clang-tidy for that file - clang-tidy has to know a
file's real compiler flags to parse it correctly, so it can only ever
run on files that have an entry in *some* compile database. Any
translation-unit file find_files() sees but neither database knows about
is reported explicitly (UNTRACKED_WARN_EXTENSIONS below) rather than
silently skipped.
"""

import json
from pathlib import Path

from generators.clang_tidy_args import host_args, stm32_args
from lint_common import REPO_ROOT, find_files, run, run_batched

CPP_EXTENSIONS = {".cpp", ".cc", ".cxx", ".c", ".h", ".hpp"}
CPP_EXCLUDE_DIRS = {
    "build",
    ".git",
    ".venv",
    "venv",
    "node_modules",
    "dist",
    "site-packages",
    "embedded/boards/compute_module/cubemx",
    ".pixi",
}

# Headers never appear as their own entry in compile_commands.json (nothing
# compiles a header directly - it only gets tidied indirectly via a TU that
# includes it). So only warn about *these* extensions being absent from
# both compile databases; a .h/.hpp not being in either database is normal,
# not a gap.
UNTRACKED_WARN_EXTENSIONS = {".cpp", ".cc", ".cxx", ".c"}


def _db_files(compile_commands_path: Path) -> set[Path]:
    """Repo-relative source files a compile_commands.json actually covers.

    Reads and filters the compile database directly.
    """
    bare_names = {d for d in CPP_EXCLUDE_DIRS if "/" not in d}
    anchored_parts = [tuple(d.split("/")) for d in CPP_EXCLUDE_DIRS if "/" in d]

    with compile_commands_path.resolve().open() as handle:
        entries = json.load(handle)

    files: set[Path] = set()
    for entry in entries:
        file_value = entry.get("file")
        if not file_value:
            continue
        file_path = Path(file_value).resolve()
        try:
            rel_path = file_path.relative_to(REPO_ROOT)
        except ValueError:
            continue
        dir_parts = rel_path.parts[:-1]
        if any(name in dir_parts for name in bare_names):
            continue
        if any(rel_path.parts[: len(prefix)] == prefix for prefix in anchored_parts):
            continue
        if file_path.suffix not in CPP_EXTENSIONS:
            continue
        files.add(rel_path)
    return files


def _warn_untracked(all_files: set[Path], known: set[Path]) -> None:
    """Flag translation-unit files that exist in the tree but aren't in
    either compile database, so they're not silently skipped by clang-tidy.
    """
    untracked = sorted(
        f for f in all_files if f.suffix in UNTRACKED_WARN_EXTENSIONS and f not in known
    )
    if not untracked:
        return
    print(
        "warning: the following files are not part of the host or stm32 "
        "CMake targets and were NOT clang-tidy'd (add them to CMakeLists.txt "
        "if that's not intentional):"
    )
    for f in untracked:
        print(f"  {f}")


def cpp_check() -> bool:
    ok = True

    # find_files() is the single source of truth for "every C++ file that
    # exists" - used for clang-format directly, and as the universe we
    # cross-check the compile databases against for clang-tidy coverage.
    cpp_files = find_files(CPP_EXTENSIONS, CPP_EXCLUDE_DIRS)
    if cpp_files:
        print("Checking C++ formatting...")
        if not run_batched(
            ["clang-format", "--style=file", "--dry-run", "--Werror"], list(map(str, cpp_files))
        ):
            ok = False

    all_rel_files = {p.relative_to(REPO_ROOT) for p in cpp_files}
    host_files = _db_files(REPO_ROOT / "build/host/compile_commands.json")
    stm32_files = _db_files(REPO_ROOT / "build/stm32/compile_commands.json")
    _warn_untracked(all_rel_files, host_files | stm32_files)

    print("Linting host code with build/host compile database...")
    extra_args = host_args()
    for source_file in sorted(host_files):
        if run(["clang-tidy", "-p", "build/host", *extra_args, str(source_file)]) != 0:
            ok = False

    print("Linting STM32 code with build/stm32 compile database...")
    extra_args = stm32_args(REPO_ROOT / "build/stm32/compile_commands.json")
    for source_file in sorted(stm32_files):
        if run(["clang-tidy", "-p", "build/stm32", *extra_args, str(source_file)]) != 0:
            ok = False

    return ok


def cpp_fix() -> None:
    cpp_files = find_files(CPP_EXTENSIONS, CPP_EXCLUDE_DIRS)
    if not cpp_files:
        return
    print("Formatting C++ files...")
    run_batched(["clang-format", "--style=file", "-i"], list(map(str, cpp_files)))
