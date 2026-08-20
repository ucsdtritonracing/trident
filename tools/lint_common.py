#!/usr/bin/env python3
"""Shared helpers for tools/lint_cpp.py and tools/lint_python.py.

Kept separate from tools/lint.py so the entry point stays a thin
dispatcher and the cpp/python implementations aren't reaching into each
other's modules.
"""

import subprocess
from pathlib import Path

# Brittle, replace later
REPO_ROOT = Path(__file__).resolve().parent.parent

# Directory names excluded at any depth when walking the tree for files.
COMMON_EXCLUDE_DIRS = {"build", ".git", ".venv", "venv", "node_modules", "dist", "site-packages"}

# Don't print more than this many argv entries for a single command; long
# batched file lists get summarized instead so the terminal doesn't flood.
_MAX_PRINTED_ARGS = 8


def run(cmd: list[str]) -> int:
    """Run `cmd` from REPO_ROOT, echoing a (possibly truncated) preview first."""
    if len(cmd) > _MAX_PRINTED_ARGS:
        head = cmd[: _MAX_PRINTED_ARGS - 1]
        shown = [*head, f"... (+{len(cmd) - len(head)} more)"]
    else:
        shown = cmd
    print(f"$ {' '.join(shown)}")
    return subprocess.run(cmd, cwd=REPO_ROOT).returncode


def run_batched(base_cmd: list[str], files: list[str], max_arg_chars: int = 6000) -> bool:
    """Run base_cmd with files appended, splitting into multiple invocations
    if needed so no single command line gets too long.

    Returns True if every chunk's invocation exited 0.
    """
    if not files:
        return True

    ok = True
    base_len = sum(len(a) + 1 for a in base_cmd)
    chunk: list[str] = []
    chunk_len = base_len
    for f in files:
        if chunk and chunk_len + len(f) + 1 > max_arg_chars:
            if run([*base_cmd, *chunk]) != 0:
                ok = False
            chunk = []
            chunk_len = base_len
        chunk.append(f)
        chunk_len += len(f) + 1
    if chunk:
        if run([*base_cmd, *chunk]) != 0:
            ok = False
    return ok


def find_files(extensions: set[str], exclude_dirs: set[str]) -> list[Path]:
    """Find files under REPO_ROOT with the given extensions, skipping excluded dirs.

    exclude_dirs entries may be a bare directory name (e.g. "build" or
    ".venv"), which is excluded at ANY depth, or a "a/b" style repo-relative
    path, which is only excluded at that exact location.
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
