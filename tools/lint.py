#!/usr/bin/env python3
"""Check or fix formatting/lint for C++ and/or Python code.

Usage:
    python3 tools/lint.py check [all|cpp|python]
    python3 tools/lint.py fix   [all|cpp|python]

"check" runs clang-format --dry-run, clang-tidy, and ruff format/check
checks, and exits non-zero if any of them fail.

"fix" runs clang-format -i and ruff format/check --fix to apply changes
in place.

This is just a dispatcher: the actual cpp/python implementations live in
tools/lint_cpp.py and tools/lint_python.py so changes to one don't risk
touching the other.
"""

import argparse
import sys

from cmake_target import run_cmake
from lint_cpp import cpp_check, cpp_fix
from lint_python import python_check, python_fix


def _check(target: str) -> bool:
    ok = True
    if target in ("all", "cpp"):
        # cmake needs to be configured before clang-tidy can read compile_commands.json
        run_cmake("configure", "all")
        ok &= cpp_check()
    if target in ("all", "python"):
        ok &= python_check()
    return ok


def _fix(target: str) -> None:
    if target in ("all", "cpp"):
        cpp_fix()
    if target in ("all", "python"):
        python_fix()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=["check", "fix"])
    parser.add_argument("target", nargs="?", default="all", choices=["all", "cpp", "python"])
    args = parser.parse_args()

    if args.mode == "fix":
        _fix(args.target)
        return

    sys.exit(0 if _check(args.target) else 1)


if __name__ == "__main__":
    main()
