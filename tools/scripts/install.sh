#!/usr/bin/env bash
set -euo pipefail

# Colors
if [[ -t 1 ]]; then
    GREEN="\033[0;32m"
    BLUE="\033[0;34m"
    YELLOW="\033[0;33m"
    RED="\033[0;31m"
    CYAN="\033[1;36m"
    RESET="\033[0m"
else
    GREEN=""
    BLUE=""
    YELLOW=""
    RED=""
    CYAN=""
    RESET=""
fi

# Helpers
step() {
    echo -e "\n${BLUE}▶ $1${RESET}"
}
success() {
    echo -e "${GREEN}✓${RESET} $1"
}
warn() {
    echo -e "${YELLOW}!${RESET} $1"
}
fail() {
    echo -e "${RED}✗${RESET} $1"
}

install_pixi() {
    echo "Installing Pixi..."

    if ! curl -fsSL https://pixi.sh/install.sh | bash; then
        printf "\n"
        printf "${YELLOW}Pixi installation failed.${RESET}\n\n"
        printf "The installer could not resolve ${CYAN}pixi.sh${RESET}.\n"
        printf "This is usually a DNS or network issue.\n\n"

        if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
            # Double slash because MSYS2 tries to convert "/" to Windows paths
            printf "Try: ${CYAN}ipconfig //flushdns${RESET}\n"
        elif [[ "$OSTYPE" == "win32"  ]]; then
            printf "Try: ${CYAN}ipconfig /flushdns${RESET}\n"
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            printf "Try: ${CYAN}sudo dscacheutil -flushcache && sudo killall -HUP mDNSResponder${RESET}\n"
        else
            printf "Try restarting your network connection or clearing your DNS cache.\n"
        fi

        printf "\n"
        printf "Then rerun: ${CYAN}./tools/scripts/install.sh${RESET}\n\n"

        exit 1
    fi
}


# Start of script

printf "%b\n"                                                               \
"${BLUE}═══════════════════════════════════════════════════════${RESET}"    \
"${BLUE}               Trident Development Setup               ${RESET}"    \
"${BLUE}═══════════════════════════════════════════════════════${RESET}\n"  \

step "Checking Pixi installation"

if command -v pixi >/dev/null 2>&1; then
    success "Pixi already installed ($(pixi --version))"
else
    rm -rf "$HOME/.pixi"
    install_pixi
    export PATH="$HOME/.pixi/bin:$PATH"
fi

step "Installing project dependencies"

pixi install
success "Dependencies installed"

printf "%b\n"                                                               \
"${GREEN}═══════════════════════════════════════════════════════${RESET}"   \
"${GREEN}                    Setup complete!                    ${RESET}"   \
"${GREEN}═══════════════════════════════════════════════════════${RESET}\n" \

warn "${YELLOW}MANUAL ACTION REQUIRED${RESET}"
warn "Open a new terminal to activate the environment. ${CYAN}(Ctrl + Shit + \`)${RESET}"
