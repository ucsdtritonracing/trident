#!/usr/bin/env python3
"""Check or fix formatting/lint for C++ and/or Python code.

Usage:
    python3 tools/lint.py check [all|cpp|python]
    python3 tools/lint.py fix   [all|cpp|python]

"check" runs clang-format --dry-run, clang-tidy, and ruff format/check
checks, and exits non-zero if any of them fail.

"fix" runs clang-format -i and ruff format/check --fix to apply changes
in place.
"""

import argparse
import subprocess
import sys
from pathlib import Path

from generators.clang_tidy_args import host_args, stm32_args

REPO_ROOT = Path(__file__).resolve().parent.parent

CPP_EXTENSIONS = {".cpp", ".cc", ".cxx", ".c", ".h", ".hpp"}
CPP_EXCLUDE_DIRS = {
    "build",
    ".git",
    ".venv",
    "venv",
    "node_modules",
    "dist",
    "site-packages",
    "embedded/boards/compute_module/cubemx/Core",
    "embedded/boards/compute_module/cubemx/Drivers",
}
PY_EXCLUDE_DIRS = {"build", ".git", ".venv", "venv", "node_modules", "dist", "site-packages"}


def run(cmd: list[str]) -> int:
    print(f"$ {' '.join(cmd)}")
    return subprocess.run(cmd, cwd=REPO_ROOT).returncode


def find_files(extensions: set[str], exclude_dirs: set[str]) -> list[Path]:
    """Find files under REPO_ROOT with the given extensions, skipping excluded dirs.

    exclude_dirs entries may be a single directory name (e.g. "build") or a
    slash-separated relative path (e.g. "embedded/boards/.../Core").
    """
    bare_names = {d for d in exclude_dirs if "/" not in d}
    anchored_parts = [tuple(d.split("/")) for d in exclude_dirs if "/" in d]

    matches: list[Path] = []
    for path in sorted(REPO_ROOT.rglob("*")):
        if not path.is_file() or path.suffix not in extensions:
            continue
        rel_parts = path.relative_to(REPO_ROOT).parts
        dir_parts = rel_parts[:-1]  # exclude checks apply to directory components only
        if any(name in dir_parts for name in bare_names):
            continue
        if any(rel_parts[: len(prefix)] == prefix for prefix in anchored_parts):
            continue
        matches.append(path)
    return matches


def compile_db_files(compile_commands_path: Path) -> list[Path]:
    result = subprocess.run(
        [
            "python3",
            "tools/generators/list_compile_db_files.py",
            str(REPO_ROOT),
            str(compile_commands_path),
        ],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
        check=True,
    )
    return [Path(line) for line in result.stdout.splitlines() if line.strip()]


def cpp_check() -> bool:
    ok = True

    cpp_files = find_files(CPP_EXTENSIONS, CPP_EXCLUDE_DIRS)
    if cpp_files:
        print("Checking C++ formatting...")
        if (
            run(["clang-format", "--style=file", "--dry-run", "--Werror", *map(str, cpp_files)])
            != 0
        ):
            ok = False

    print("Linting host code with build/host compile database...")
    extra_args = host_args()
    for source_file in compile_db_files(REPO_ROOT / "build/host/compile_commands.json"):
        if run(["clang-tidy", "-p", "build/host", *extra_args, str(source_file)]) != 0:
            ok = False

    print("Linting STM32 code with build/stm32 compile database...")
    extra_args = stm32_args(REPO_ROOT / "build/stm32/compile_commands.json")
    for source_file in compile_db_files(REPO_ROOT / "build/stm32/compile_commands.json"):
        if run(["clang-tidy", "-p", "build/stm32", *extra_args, str(source_file)]) != 0:
            ok = False

    return ok


def cpp_fix() -> None:
    cpp_files = find_files(CPP_EXTENSIONS, CPP_EXCLUDE_DIRS)
    if not cpp_files:
        return
    print("Formatting C++ files...")
    run(["clang-format", "--style=file", "-i", *map(str, cpp_files)])


def python_check() -> bool:
    py_files = list(map(str, find_files({".py"}, PY_EXCLUDE_DIRS)))
    if not py_files:
        return True

    ok = True
    print("Checking Python formatting...")
    fmt_cmd = [
        "uv",
        "run",
        "--extra",
        "dev",
        "ruff",
        "format",
        "--check",
        "--diff",
        "--config",
        "pyproject.toml",
    ]
    if run([*fmt_cmd, *py_files]) != 0:
        ok = False

    print("Checking Python lint...")
    lint_cmd = ["uv", "run", "--extra", "dev", "ruff", "check", "--config", "pyproject.toml"]
    if run([*lint_cmd, *py_files]) != 0:
        ok = False

    return ok


def python_fix() -> None:
    py_files = list(map(str, find_files({".py"}, PY_EXCLUDE_DIRS)))
    if not py_files:
        return

    print("Formatting Python files...")
    run(["uv", "run", "--extra", "dev", "ruff", "format", "--config", "pyproject.toml", *py_files])

    print("Applying safe Python lint fixes...")
    run(
        [
            "uv",
            "run",
            "--extra",
            "dev",
            "ruff",
            "check",
            "--fix",
            "--config",
            "pyproject.toml",
            *py_files,
        ]
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=["check", "fix"])
    parser.add_argument("target", nargs="?", default="all", choices=["all", "cpp", "python"])
    args = parser.parse_args()

    if args.mode == "check":
        ok = True
        if args.target in ("all", "cpp"):
            # cmake needs to be configured before clang-tidy can read compile_commands.json
            subprocess.run(
                ["python3", "tools/cmake_target.py", "configure", "all"], cwd=REPO_ROOT, check=True
            )
            if not cpp_check():
                ok = False
        if args.target in ("all", "python"):
            if not python_check():
                ok = False
        sys.exit(0 if ok else 1)
    else:
        if args.target in ("all", "cpp"):
            cpp_fix()
        if args.target in ("all", "python"):
            python_fix()


if __name__ == "__main__":
    main()
