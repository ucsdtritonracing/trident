#!/usr/bin/env python3
"""Run a cmake action (configure, build, or clean) against one or all presets.

Usage:
    python3 tools/cmake_target.py configure [all|host|stm32]
    python3 tools/cmake_target.py build [all|host|stm32]
    python3 tools/cmake_target.py clean [all|host|stm32]

"all" (the default) runs stm32 then host
"""

import argparse
import subprocess
import sys

PRESETS = ["stm32", "host"]

ACTIONS = {
    "configure": lambda preset: ["cmake", "--preset", preset],
    "build": lambda preset: ["cmake", "--build", "--preset", preset],
    "clean": lambda preset: ["cmake", "--build", "--preset", preset, "--target", "clean"],
}


def run(cmd: list[str]) -> None:
    print(f"$ {' '.join(cmd)}")
    result = subprocess.run(cmd)
    if result.returncode != 0:
        sys.exit(result.returncode)


def run_cmake(action: str, target: str = "all") -> None:
    """Run `action` (configure/build/clean) against `target` (all/host/stm32).

    Pulled out of main() so other tools (tools/lint.py) can call this
    directly.
    """
    presets = PRESETS if target == "all" else [target]
    for preset in presets:
        run(ACTIONS[action](preset))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=sorted(ACTIONS))
    parser.add_argument("target", nargs="?", default="all", choices=["all", "host", "stm32"])
    args = parser.parse_args()

    run_cmake(args.action, args.target)


if __name__ == "__main__":
    main()
