#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# run_regression.sh — Unified regression entry point for ert-components.
#
# Discovers platforms from the filesystem or from a named platform list,
# builds each one, optionally runs smoke tests, and optionally generates a
# per-platform SBOM and/or an overall feature compliance matrix.
#
# Usage:
#   ./SystemTests/CI/run_regression.sh [MODE] [OPTIONS]
#
# Modes (mutually exclusive; interactive menu shown if omitted):
#   --discover                 Enumerate every directory under target/platform/
#   --platform-list <name>     Use a named list from SystemTests/CI/platform-lists/
#                              (e.g. "community", "published") or a direct file path
#
# Build options:
#   --build-only               Compile only — run make all_docker/make -j8, skip targetenv,
#                              APK, Unity, and exe-presence checks.  Fastest correctness gate.
#   --compile-only             Build + targetenv — skip APK, Unity, and exe-presence checks
#   --include-slow             Append slow targets (linux_android_arm64_unity-lib, etc.)
#   --no-preflight             Skip the CDF hash pre-flight check
#
# Test options:
#   --run-apps                 Run smoke tests on suitable Linux targets (default: off)
#   --local-apps               Build against the ../apps working tree as-is
#                              (EHS_SKIP_REPO_PULL=1). Without this, targetenv
#                              switches ../apps to RELEASE-PRODUCTION and refuses
#                              to run if that repo has uncommitted changes -- so a
#                              colleague editing an app in Lucid fails the run for
#                              reasons unrelated to the code under test. Use for
#                              build gates; omit when validating a real release.
#   --package                  Also run `make targetenv_package` per platform, which
#                              proxies to the packager EHS_PACKAGER_TYPE names
#                              (deb/apk/nsis/esp32/xmos/arduino). Off by default
#                              because packaging is slower than a build.
#
# Reporting options:
#   --generate-sbom            Generate a per-platform SBOM (Software Bill of Materials)
#                              after each platform builds.  Output goes to
#                              ../CI_RESULTS/<platform>/sbom/ — outside the source
#                              tree; does NOT write to Releases/ and does NOT
#                              update DEPENDENTS.md.  Suitable for routine/nightly runs.
#                              Skipped if configure failed for a platform.
#   --release-report           Like --generate-sbom but writes to Releases/SBOM/<version>/
#                              and updates DEPENDENTS.md in sibling dependency repos.
#                              Intended for use by update_release_report.sh or when
#                              explicitly cutting a versioned release snapshot.
#   --generate-matrix [fmt]    Generate feature compliance matrix after the build run.
#                              fmt = csv | md | html | all  (default: md)
#   --no-summary               Suppress the colourised pass/fail listing at the end
#
# Claude-assisted failure options:
#   --claude-autofix           Invoke "claude --print" to diagnose and fix on failure
#   --claude-interactive       Open interactive Claude session on failure
#
# Platform list format (SystemTests/CI/platform-lists/*.txt):
#   One platform directory name per line.  Blank lines and # comments are ignored.
#
# Build logs and pass/fail flags go to ../CI_RESULTS/<platform>/ (outside the
# source tree, rotated to ../CI_RESULTS.prev each run). Durable summaries --
# feature matrix, timings -- go to SystemTests/reports/ and are committed.
# See SystemTests/CI/README.md for the layout.

set -uo pipefail

# ── Resolve script and library locations ─────────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIB_DIR="${SCRIPT_DIR}/../../scripts/build-function-library"
PLATFORM_LISTS_DIR="${SCRIPT_DIR}/platform-lists"

# shellcheck source=../../scripts/build-function-library/ci_functions.sh
source "${LIB_DIR}/ci_functions.sh"

ci_setup_paths

# ── Flag defaults ─────────────────────────────────────────────────────────────
MODE=""
PLATFORM_LIST_ARG=""
COMPILE_ONLY=""
BUILD_ONLY=""
INCLUDE_SLOW=0
RUN_PREFLIGHT=1
RUN_APPS=0
RUN_PACKAGE=0
GENERATE_SBOM=0
SBOM_RELEASE_MODE=0
GENERATE_MATRIX=0
MATRIX_FMT="md"
SHOW_SUMMARY=1
CLAUDE_AUTOFIX=0
CLAUDE_INTERACTIVE=0

# Export flags consumed by ci_functions.sh library functions
export BUILD_ONLY COMPILE_ONLY CLAUDE_AUTOFIX CLAUDE_INTERACTIVE SBOM_RELEASE_MODE

# ── Argument parsing ──────────────────────────────────────────────────────────
while [[ $# -gt 0 ]]; do
    case "$1" in
        --discover)
            MODE=discover
            ;;
        --platform-list)
            shift
            [[ $# -eq 0 ]] && { err "--platform-list requires an argument"; exit 1; }
            MODE=list
            PLATFORM_LIST_ARG="$1"
            ;;
        --build-only)
            BUILD_ONLY=yes
            ;;
        --compile-only)
            COMPILE_ONLY=yes
            ;;
        --include-slow)
            INCLUDE_SLOW=1
            ;;
        --no-preflight)
            RUN_PREFLIGHT=0
            ;;
        --local-apps)
            export EHS_SKIP_REPO_PULL=1
            ;;
        --package)
            RUN_PACKAGE=1
            ;;
        --run-apps)
            RUN_APPS=1
            ;;
        --generate-sbom)
            GENERATE_SBOM=1
            ;;
        --release-report)
            GENERATE_SBOM=1
            SBOM_RELEASE_MODE=1
            ;;
        --generate-matrix)
            GENERATE_MATRIX=1
            # Optional next argument: csv | md | html | all
            if [[ ${2:-} =~ ^(csv|md|html|all)$ ]]; then
                MATRIX_FMT="$2"
                shift
            fi
            ;;
        --no-summary)
            SHOW_SUMMARY=0
            ;;
        --claude-autofix)
            CLAUDE_AUTOFIX=1
            heading "--claude-autofix enabled — Claude will attempt automatic fixes on failure"
            ;;
        --claude-interactive)
            CLAUDE_INTERACTIVE=1
            heading "--claude-interactive enabled — Claude interactive session will open on failure"
            ;;
        -h|--help)
            # Print the header comment block as help text
            sed -n '/^# Usage:/,/^[^#]/p' "${BASH_SOURCE[0]}" \
                | grep '^#' | sed 's/^# \?//'
            exit 0
            ;;
        *)
            err "Unknown option: '$1'"
            printf 'Run with --help for usage.\n' >&2
            exit 1
            ;;
    esac
    shift
done

# ── Interactive mode selection (only when stdin is a terminal) ────────────────
if [ -z "$MODE" ]; then
    if [ ! -t 0 ]; then
        warn "No mode specified and stdin is not a terminal — defaulting to --discover"
        MODE=discover
    else
        # Build the list of available platform list files
        declare -a _plist_names=()
        declare -a _plist_files=()
        if [ -d "$PLATFORM_LISTS_DIR" ]; then
            for _f in "${PLATFORM_LISTS_DIR}"/*.txt; do
                [ -f "$_f" ] || continue
                _plist_names+=( "$(basename "$_f" .txt)" )
                _plist_files+=( "$_f" )
            done
        fi

        echo
        echo "Select regression mode:"
        echo "  1) Discover all platforms  (current target/platform/ tree)"
        _menu_idx=2
        for (( _i=0; _i<${#_plist_names[@]}; _i++ )); do
            printf '  %d) Platform list: %-20s  [%s]\n' \
                "$_menu_idx" "${_plist_names[$_i]}" "${_plist_files[$_i]}"
            (( _menu_idx++ ))
        done
        echo "  c) Custom platform list file  (enter path)"
        echo "  q) Quit"
        echo
        read -rp "Choice: " _choice

        if [ "$_choice" = "1" ]; then
            MODE=discover
        elif [[ "$_choice" =~ ^[0-9]+$ ]] \
             && (( _choice >= 2 && _choice < _menu_idx )); then
            MODE=list
            PLATFORM_LIST_ARG="${_plist_files[$(( _choice - 2 ))]}"
        elif [ "${_choice,,}" = "c" ]; then
            MODE=list
            read -rp "Path to platform list file: " PLATFORM_LIST_ARG
        elif [ "${_choice,,}" = "q" ]; then
            echo "Aborted."
            exit 0
        else
            err "Invalid choice '${_choice}'"
            exit 1
        fi
    fi
fi

# ── Resolve the platform list file path ──────────────────────────────────────
PLATFORM_LIST_FILE=""
if [ "$MODE" = "list" ]; then
    if [ -f "$PLATFORM_LIST_ARG" ]; then
        PLATFORM_LIST_FILE="$PLATFORM_LIST_ARG"
    else
        # Accept a bare name (e.g. "community") → look up in platform-lists/
        _candidate="${PLATFORM_LISTS_DIR}/${PLATFORM_LIST_ARG}.txt"
        if [ -f "$_candidate" ]; then
            PLATFORM_LIST_FILE="$_candidate"
        else
            err "Platform list not found: '${PLATFORM_LIST_ARG}'"
            err "(also tried: ${_candidate})"
            exit 1
        fi
    fi
fi

# ── Move to the ert-components root ──────────────────────────────────────────
cd "${ROOT_DIR}/../ert-components" || { err "Cannot cd to ert-components"; exit 1; }

# ── Build the platform array ──────────────────────────────────────────────────
declare -a platforms=()

if [ "$MODE" = "discover" ]; then
    while IFS= read -r -d '' _d; do
        platforms+=( "$(basename "$_d")" )
    done < <(find ./target/platform -maxdepth 1 -mindepth 1 -type d -print0 | sort -z)
else
    while IFS= read -r _line; do
        _line="${_line%%#*}"       # strip inline comments
        _line="${_line//[[:space:]]/}"  # strip whitespace
        [ -n "$_line" ] && platforms+=( "$_line" )
    done < "$PLATFORM_LIST_FILE"
fi

if [ "$INCLUDE_SLOW" -eq 1 ]; then
    platforms+=( 'linux_android_arm64_unity-lib' )
fi

echo
printf 'Platforms to process (%d):\n' "${#platforms[@]}"
for _p in "${platforms[@]}"; do printf '  - %s\n' "$_p"; done
echo

# ── Run lock ──────────────────────────────────────────────────────────────────
# TARGET.cfg is global mutable state: the loop below runs ./configure for every
# platform in turn. A second regression run — or a stray ./configure in another
# shell — silently hijacks this one's target, and the results are then attributed
# to the wrong platform with no error anywhere. Refuse to start if a run is
# already in progress.
CI_LOCK_FILE="${SCRIPT_DIR}/.regression.lock"
if [ -e "$CI_LOCK_FILE" ]; then
    _lock_pid="$(cat "$CI_LOCK_FILE" 2>/dev/null || true)"
    if [ -n "$_lock_pid" ] && kill -0 "$_lock_pid" 2>/dev/null; then
        err "A regression run is already in progress (pid ${_lock_pid})."
        err "Lock: ${CI_LOCK_FILE}"
        err "Do not run ./configure or make while one is active — it will corrupt both runs."
        exit 1
    fi
    echo "  Removing stale lock (pid ${_lock_pid:-unknown} is not running)"
    rm -f "$CI_LOCK_FILE"
fi
echo $$ > "$CI_LOCK_FILE"

# ── Save TARGET.cfg so the developer's active target survives the run ─────────
CI_TARGET_CFG="${ROOT_DIR}/TARGET.cfg"
CI_TARGET_CFG_SAVED=""
if [ -f "$CI_TARGET_CFG" ]; then
    CI_TARGET_CFG_SAVED="$(cat "$CI_TARGET_CFG")"
    echo "  Saved TARGET.cfg (${CI_TARGET_CFG_SAVED}) — will be restored on exit"
fi

# Released on any exit path, including Ctrl-C. Note the exit status is preserved:
# bash restores $? after an EXIT trap unless the trap itself calls exit.
_ci_cleanup() {
    if [ -n "${CI_TARGET_CFG_SAVED}" ]; then
        printf '%s\n' "${CI_TARGET_CFG_SAVED}" > "${CI_TARGET_CFG}"
    fi
    rm -f "${CI_LOCK_FILE}"
}
# INT/TERM must *exit*, not just clean up: a signal handler that returns lets the
# script carry on to the next platform, which re-runs ./configure and overwrites
# the TARGET.cfg the handler just restored. Route both signals through exit so
# the single EXIT trap does the cleanup exactly once.
trap _ci_cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

# ── Initialise results directory (rotating the previous run to .prev) ─────────
if [ -d "${CI_RESULT_DIR}" ] && [ -n "$(ls -A "${CI_RESULT_DIR}" 2>/dev/null)" ]; then
    rm -rf "${CI_RESULT_DIR%/}.prev"
    mv "${CI_RESULT_DIR%/}" "${CI_RESULT_DIR%/}.prev"
    echo "  Previous results rotated to $(basename "${CI_RESULT_DIR%/}").prev"
fi
mkdir -p "${CI_RESULT_DIR}"

# ── Pre-flight check ──────────────────────────────────────────────────────────
if [ "$RUN_PREFLIGHT" -eq 1 ]; then
    if ! ci_run_preflight "Common/Components"; then
        err "Pre-flight failed — aborting regression run"
        exit 1
    fi
fi

# ── Once-per-run preparation ──────────────────────────────────────────────────
# Host tools and the dependency-repo fetch are target-independent, so they run
# once here rather than once per platform. The loop still calls prepdeps with
# SKIP_REPOS=1 for the per-target checks.
ci_prepare_run || {
    err "Run preparation failed — not starting the platform sweep."
    exit 1
}

# ── Main per-platform loop ────────────────────────────────────────────────────
for platform in "${platforms[@]}"; do
    echo
    echo "=============================== ${platform} =============================="

    if ci_skip_platform "$platform"; then
        warn "Ignoring directory '${platform}'"
        continue
    fi

    echo "Building and testing ${platform}"
    _plat_start=$SECONDS

    ci_setup_platform_dir
    ci_remove_build

    # Return to the build root in case ci_build_target changed the directory
    cd "${ROOT_DIR}/../ert-components" || { err "Cannot cd to ert-components"; exit 1; }

    if ci_build_target; then
        if [ "${COMPILE_ONLY:-}" != "yes" ] && [ "${BUILD_ONLY:-}" != "yes" ]; then
            ci_test_build
        fi
    fi

    # ── SBOM — run while the platform is still configured (opt-in) ───────────
    # Must happen before the next iteration's make clean / ./configure.
    if [ "$GENERATE_SBOM" -eq 1 ]; then
        cd "${ROOT_DIR}/../ert-components" || { err "Cannot cd to ert-components"; exit 1; }
        ci_generate_sbom
    fi

    # ── Smoke tests on Linux targets (opt-in via --run-apps) ─────────────────
    # Runs the unit suite through the single app-test engine; see
    # SystemTests/apps/README.md. Flag files keep the previous names so the
    # results layout in SystemTests/CI/README.md still holds.
    if [ "$RUN_APPS" -eq 1 ]; then
        if [[ "${platform}" =~ ^linux_x86 ]] \
           || [[ "${platform}" == "${TOOL_TEST_EHS_VARIANT}" ]]; then
            if "${ROOT_DIR}/../ert-components/SystemTests/CI/run_lucid_apps.sh" \
                    --target "${platform}" --suite unit \
                    >> "${CI_RESULT_DIR}/${platform}/build.log" 2>&1; then
                touch "${CI_RESULT_DIR}/${platform}/exe-host-run-app.pass"
            else
                touch "${CI_RESULT_DIR}/${platform}/exe-host-run-app.fail"
            fi
        fi
    fi

    # Per-platform wall clock. Recorded so a representative sub-10-minute smoke
    # list can be chosen from measurement rather than guesswork -- there was no
    # timing data at all before this.
    _plat_secs=$((SECONDS - _plat_start))
    printf '%s\t%s\n' "${platform}" "${_plat_secs}" >> "${CI_REPORT_DIR}/timings.tsv"
    echo "  ${platform} took ${_plat_secs}s"

    ci_teardown_platform_dir

    # Return to the build root for the next iteration
    cd "${ROOT_DIR}/../ert-components" || { err "Cannot cd to ert-components"; exit 1; }
done

# ── Feature compliance matrix ─────────────────────────────────────────────────
if [ "$GENERATE_MATRIX" -eq 1 ]; then
    cd "${ROOT_DIR}/../ert-components" || true
    ci_generate_matrix "$MATRIX_FMT"
fi

# ── Summary and exit ──────────────────────────────────────────────────────────
[ "$SHOW_SUMMARY" -eq 1 ] && ci_show_summary

ci_exit_code
