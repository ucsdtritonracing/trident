#!/usr/bin/env python3
"""Python formatting/lint: ruff format + ruff check, run via uv."""

from lint_common import find_files, run, run_batched

PY_EXCLUDE_DIRS = {"build", ".git", ".venv", "venv", "node_modules", "dist", "site-packages"}

_RUFF = ["uv", "run", "--extra", "dev", "ruff"]


def python_check() -> bool:
    py_files = list(map(str, find_files({".py"}, PY_EXCLUDE_DIRS)))
    if not py_files:
        return True

    ok = True
    print("Checking Python formatting...")
    fmt_cmd = [*_RUFF, "format", "--check", "--diff", "--config", "pyproject.toml"]
    if not run_batched(fmt_cmd, py_files):
        ok = False

    print("Checking Python lint...")
    lint_cmd = [*_RUFF, "check", "--config", "pyproject.toml"]
    if not run_batched(lint_cmd, py_files):
        ok = False

    return ok


def python_fix() -> None:
    py_files = list(map(str, find_files({".py"}, PY_EXCLUDE_DIRS)))
    if not py_files:
        return

    print("Formatting Python files...")
    run([*_RUFF, "format", "--config", "pyproject.toml", *py_files])

    print("Applying safe Python lint fixes...")
    run([*_RUFF, "check", "--fix", "--config", "pyproject.toml", *py_files])
