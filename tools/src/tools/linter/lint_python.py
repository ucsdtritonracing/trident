#!/usr/bin/env python3
"""Python formatting/lint: ruff format and check"""

from tools.linter.lint_common import find_files, run, run_batched

PY_EXCLUDE_DIRS = {
    "build",
    ".git",
    ".venv",
    "venv",
    "node_modules",
    "dist",
    "site-packages",
    ".pixi",
}


def python_check() -> bool:
    py_files = list(map(str, find_files({".py"}, PY_EXCLUDE_DIRS)))
    if not py_files:
        return True

    ok = True
    print("Checking Python formatting...")
    fmt_cmd = ["ruff", "format", "--check", "--diff", "--config", "ruff.toml"]
    if not run_batched(fmt_cmd, py_files):
        ok = False

    print("Checking Python lint...")
    lint_cmd = ["ruff", "check", "--config", "ruff.toml"]
    if not run_batched(lint_cmd, py_files):
        ok = False

    return ok


def python_fix() -> None:
    py_files = list(map(str, find_files({".py"}, PY_EXCLUDE_DIRS)))
    if not py_files:
        return

    print("Formatting Python files...")
    run(["ruff", "format", "--config", "ruff.toml", *py_files])

    print("Applying safe Python lint fixes...")
    run(["ruff", "check", "--fix", "--config", "ruff.toml", *py_files])
