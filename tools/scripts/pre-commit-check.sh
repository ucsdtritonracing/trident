#!/usr/bin/env bash
set -uo pipefail

if pixi run just format-check; then
    exit 0
else
    status=$?
    printf '\nFormatting or lint checks failed.\n'
    printf 'Fix the issues by running:\n'
    printf '  pixi run just format\n'
    exit "$status"
fi