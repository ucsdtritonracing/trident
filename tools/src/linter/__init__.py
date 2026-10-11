from pathlib import Path

# Brittle, replace later
REPO_ROOT = Path(__file__).resolve().parent.parent
COMPILE_COMMANDS_OUTPUT_PATH = REPO_ROOT / "build/compile_commands.json"
COMPILE_COMMANDS_HOST_PATH = REPO_ROOT / "build/host/compile_commands.json"
COMPILE_COMMANDS_STM32_PATH = REPO_ROOT / "build/stm32/compile_commands.json"
