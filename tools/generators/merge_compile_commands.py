#!/usr/bin/env python3
"""
Merge compile_commands.json from one or more build directories into a single
compile_commands.json, for clangd.

Usage (run from the repo root, e.g. via `just`):
    python tools/generators/merge_compile_commands.py -b build/stm32 -b build/host
"""

import argparse
import json
import sys
import logging
from pathlib import Path

DEFAULT_BUILD_DIRS = ["build/stm32", "build/host"]
DEFAULT_OUTPUT = "compile_commands.json"

logger = logging.getLogger(__name__)
logging.basicConfig(
    level=logging.INFO,
    format="[merge_compile_commands] %(message)s",
)

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "-b", "--build-dir",
        action="append",
        dest="build_dirs",
        metavar="DIR",
        help="Build directory containing a compile_commands.json. Can be given "
             "multiple times. Relative paths are resolved from the current "
             f"working directory. Defaults to: {', '.join(DEFAULT_BUILD_DIRS)}",
    )
    parser.add_argument(
        "-o", "--output",
        default=DEFAULT_OUTPUT,
        metavar="PATH",
        help=f"Output file path, relative to cwd unless absolute. Default: {DEFAULT_OUTPUT}",
    )
    parser.add_argument(
        "-v", "--verbose",
        action='store_true',
        help="Enable verbose output."
    )
    args = parser.parse_args()
    if not args.build_dirs:
        args.build_dirs = DEFAULT_BUILD_DIRS
    return args


def resolve(path_str: str) -> Path:
    path = Path(path_str)
    return path if path.is_absolute() else Path.cwd() / path


def load_compile_commands(build_dir: Path) -> list[dict]:
    path = build_dir / "compile_commands.json"
    if not path.exists():
        print(f"  skip: {path} not found (build tree not configured yet?)")
        return []
    with path.open("r", encoding="utf-8") as f:
        entries = json.load(f)
    logger.debug(f"  loaded {len(entries)} entries from {path}")
    return entries


def main() -> int:
    args = parse_args()
    output_path = resolve(args.output)

    merged: list[dict] = []
    seen_files: dict[str, str] = {}

    logger.level = logging.DEBUG if args.verbose else logging.INFO

    logger.debug("Merging compile_commands.json")
    for build_dir_str in args.build_dirs:
        build_dir = resolve(build_dir_str)
        entries = load_compile_commands(build_dir)

        for entry in entries:
            file_key = str(Path(entry["file"]).resolve())
            if file_key in seen_files:
                logger.debug(
                    f"  warning: '{entry['file']}' compiled in both "
                    f"{seen_files[file_key]} and {build_dir_str}; keeping first, "
                    f"dropping {build_dir_str}'s entry"
                )
                continue
            seen_files[file_key] = build_dir_str
            merged.append(entry)

    if not merged:
        logger.warning("No entries found in any build directory. Did you run cmake -B ... yet?")
        return 1

    with output_path.open("w", encoding="utf-8") as f:
        json.dump(merged, f, indent=2)

    logger.info(f"Wrote {len(merged)} total entries to {output_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())