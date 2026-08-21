#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# generate_feature_matrix.sh — Generate the feature compliance matrix without
# running a full regression build.
#
# Reads the current target/platform/ tree and CDF files directly; no build
# is required.  Output goes to SystemTests/CI/results/feature_matrix.<fmt>
# (or results/matrix.{csv,md,html} for --all).
#
# Usage:
#   ./SystemTests/CI/generate_feature_matrix.sh [md|csv|html|all]
#
# Arguments:
#   md   (default)  Markdown table  → results/feature_matrix.md
#   csv             CSV              → results/feature_matrix.csv
#   html            HTML page        → results/feature_matrix.html
#   all             All three formats → results/matrix.{csv,md,html}
#
# See scripts/software-utilities/README.md for full fb_platform_matrix.py
# option reference.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIB_DIR="${SCRIPT_DIR}/../../scripts/build-function-library"

# shellcheck source=../../scripts/build-function-library/ci_functions.sh
source "${LIB_DIR}/ci_functions.sh"

ci_setup_paths

# ── Format argument ───────────────────────────────────────────────────────────
FMT="${1:-md}"
if [[ ! "$FMT" =~ ^(md|csv|html|all)$ ]]; then
    printf 'Usage: %s [md|csv|html|all]\n' "$(basename "$0")" >&2
    exit 1
fi

# ── Ensure results directory exists ──────────────────────────────────────────
mkdir -p "${CI_RESULT_DIR}"

# ── Generate ──────────────────────────────────────────────────────────────────
cd "${ROOT_DIR}/../ert-components" || { err "Cannot cd to ert-components"; exit 1; }

ci_generate_matrix "$FMT"
