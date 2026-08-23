#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# regression_test-published-only.sh — Regression runner: builds from a
# curated platform list rather than discovering all platforms.
#
# This is a thin wrapper around run_regression.sh --platform-list.
#
# Usage:
#   ./regression_test-published-only.sh [OPTIONS]
#
# List selection options (choose one; defaults to "deployed"):
#   --community-only     Use the community platform list
#   --platform-list <n>  Use a named list from platform-lists/ (overrides default)
#
# All other run_regression.sh flags are passed through unchanged:
#   --compile-only, --include-slow, --run-apps, --generate-matrix,
#   --claude-autofix, --claude-interactive, --no-preflight, --no-summary, etc.
#
# See run_regression.sh --help for the full option list.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

LIST="deployed"
passthrough_args=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --community-only)
            LIST="community"
            ;;
        --platform-list)
            # Allow the caller to override the list entirely
            shift
            LIST="$1"
            ;;
        *)
            passthrough_args+=("$1")
            ;;
    esac
    shift
done

exec "${SCRIPT_DIR}/run_regression.sh" --platform-list "$LIST" "${passthrough_args[@]+"${passthrough_args[@]}"}"
