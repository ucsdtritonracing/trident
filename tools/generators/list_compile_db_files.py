#!/usr/bin/env python3
"""List source files from a compile_commands.json, excluding build/vendor dirs."""

import json
import sys
from pathlib import Path

EXCLUDE_PARTS = {".git", ".venv", "venv", "build", "node_modules", "dist", "site-packages"}
EXCLUDE_REL_PATHS = [
    Path("embedded/boards/compute_module/cubemx/Core"),
    Path("embedded/boards/compute_module/cubemx/Drivers"),
]
SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".cxx", ".h", ".hpp"}


def main() -> None:
    root = Path(sys.argv[1]).resolve()
    db_path = Path(sys.argv[2]).resolve()

    with db_path.open() as handle:
        entries = json.load(handle)

    for entry in entries:
        file_value = entry.get("file")
        if not file_value:
            continue
        file_path = Path(file_value).resolve()
        try:
            rel_path = file_path.relative_to(root)
        except ValueError:
            continue
        if any(part in EXCLUDE_PARTS for part in rel_path.parts):
            continue
        if any(
            rel_path == excluded or excluded in rel_path.parents for excluded in EXCLUDE_REL_PATHS
        ):
            continue
        if file_path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        print(rel_path.as_posix())


if __name__ == "__main__":
    main()
