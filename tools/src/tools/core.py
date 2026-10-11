import os
from pathlib import Path

REPO_ROOT = Path(os.environ["PIXI_PROJECT_ROOT"])
COMPILE_COMMANDS_OUTPUT_PATH = REPO_ROOT / "build/compile_commands.json"
COMPILE_COMMANDS_HOST_PATH = REPO_ROOT / "build/host/compile_commands.json"
COMPILE_COMMANDS_STM32_PATH = REPO_ROOT / "build/stm32/compile_commands.json"