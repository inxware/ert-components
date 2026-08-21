#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# validate_all.sh — Regression runner: discovers every platform under
# target/platform/ and builds/tests them all.
#
# This is a thin wrapper around run_regression.sh --discover.
# All flags supported by run_regression.sh are passed through.
#
# Common flags:
#   --run-apps           Enable smoke tests on Linux x86 targets
#   --build-only         Fastest gate: make all_docker only, skip targetenv/APK/Unity
#   --compile-only       Build + targetenv, skip exe-presence and APK/Unity steps
#   --generate-matrix    Generate feature compliance matrix after the run
#   --claude-autofix     Auto-fix failures with Claude (non-interactive)
#   --claude-interactive Open interactive Claude session on failure
#   --no-preflight       Skip the CDF hash pre-flight check
#
# See run_regression.sh --help for the full option list.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "${SCRIPT_DIR}/run_regression.sh" --discover "$@"
