#!/usr/bin/env bash

set -e

# Colors
if [[ -t 1 ]]; then
    YELLOW="\033[0;33m"
    CYAN="\033[1;36m"
    RESET="\033[0m"
else
    YELLOW=""
    CYAN=""
    RESET=""
fi

# Always start in the workspace if VS Code passed it.
if [[ -n "${VSCODE_CWD:-}" ]]; then
    cd "$VSCODE_CWD"
fi

if command -v pixi >/dev/null 2>&1; then
    exec pixi shell --change-ps1=false
fi

printf "%b\n"                                                                       \
"${YELLOW}═══════════════════════════════════════════════════════${RESET}"          \
"${YELLOW}           ⚠    Pixi is not installed.    ⚠           ${RESET}"          \
"${YELLOW}═══════════════════════════════════════════════════════${RESET}\n"        \
"This project uses ${CYAN}Pixi${RESET} to manage its development environment.\n"    \
"Install it by running:"                                                            \
"    ${CYAN}./tools/scripts/install.sh${RESET}\n"                                   \
"After installation, simply open a new terminal.\n"                                 \
"Falling back to a normal shell..."

exec bash -l