#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# ci_functions.sh — Shared build, test, and reporting functions for the
# ert-components CI regression framework.
#
# Source this file from any CI entry-point script:
#
#   source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../../scripts/build-function-library/ci_functions.sh"
#
# Call ci_setup_paths immediately after sourcing to initialise the standard
# path variables (ROOT_DIR, CI_RESULT_DIR, CI_REPORT_DIR, TARGET_TREE_DIR).
#
# Globals consumed by the per-platform functions (set by the entry-point):
#   platform          — current platform name  (e.g. linux_x86_64_gtk_gst_debian11)
#   CI_RESULT_DIR     — build logs / flags, ../CI_RESULTS (set by ci_setup_paths)
#   CI_REPORT_DIR     — committed summaries, SystemTests/reports (ditto)
#   TARGET_TREE_DIR   — path to TARGET_TREES (set by ci_setup_paths)
#   COMPILE_ONLY      — "yes" to skip APK/Unity/exe-presence steps
#   BUILD_ONLY        — "yes" to run only the compile step (make all_docker/make -j8);
#                       skips targetenv, APK, Unity.  Writes build-target.skip.
#   CLAUDE_AUTOFIX    — 1 to run claude --print on failure
#   CLAUDE_INTERACTIVE — 1 to open interactive claude session on failure
#   SBOM_RELEASE_MODE — 1 to write SBOM to Releases/SBOM/ and update DEPENDENTS.md
#                       (default 0: write to CI_RESULT_DIR/<platform>/sbom/, skip DEPENDENTS.md)

# ── Guard against double-sourcing ────────────────────────────────────────────
[ -n "${_CI_FUNCTIONS_LOADED:-}" ] && return 0
_CI_FUNCTIONS_LOADED=1

# ── Pull in colour helpers ────────────────────────────────────────────────────
_CI_FUNC_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=colour.sh
source "${_CI_FUNC_DIR}/colour.sh"

# ── Path setup ────────────────────────────────────────────────────────────────
# Sets the five canonical path variables used by every CI function.
# Must be called once from the entry-point after sourcing this library.
function ci_setup_paths {
    if [ -d ../CI ]; then
        ROOT_DIR="$(cd "$PWD/../../" && pwd)"
    else
        ROOT_DIR="$PWD"
    fi
    # Two homes, split on whether the artefact is worth reading next month.
    #
    # CI_RESULT_DIR — build logs and pass/fail flags for one run. Debris: a
    # single platform's build.log runs to tens of MB. Lives outside the repo
    # next to TARGET_TREES, the same place the rest of the build output goes,
    # so the source tree cannot accumulate it and no .gitignore is needed.
    #
    # CI_REPORT_DIR — small durable summaries that are committed for history
    # (feature matrix, timings, app-test reports).
    CI_RESULT_DIR="${ROOT_DIR}/../CI_RESULTS"
    CI_REPORT_DIR="${ROOT_DIR}/SystemTests/reports"
    TARGET_TREE_DIR="${ROOT_DIR}/../TARGET_TREES"
    TOOL_TEST_EHS_VARIANT="linux_x86_gtk"
    export ROOT_DIR CI_RESULT_DIR CI_REPORT_DIR TARGET_TREE_DIR TOOL_TEST_EHS_VARIANT
}

# ── Platform filtering ────────────────────────────────────────────────────────
# Returns 0 (true = skip this platform) for housekeeping names that should
# never be treated as build targets.
function ci_skip_platform {
    local p="$1"
    [[ "$p" == ".svn" || "$p" == "depricated" || "$p" =~ ^base ]]
}

# ── Per-platform result directory lifecycle ───────────────────────────────────
# Creates a fresh per-platform results directory and places the in-progress
# marker.  Call ci_teardown_platform_dir when the platform is done.
function ci_setup_platform_dir {
    mkdir -p "${CI_RESULT_DIR}/${platform}"
    rm -rf   "${CI_RESULT_DIR}/${platform:?}/"*
    touch    "${CI_RESULT_DIR}/${platform}/building.flag"
}

function ci_teardown_platform_dir {
    rm -f "${CI_RESULT_DIR}/${platform}/building.flag"
}

# ── Binary cleanup ────────────────────────────────────────────────────────────
# Removes both ehs.exe and any .so files from the target tree so that a stale
# artefact from a previous run cannot mask a link failure.
function ci_remove_build {
    rm -f "${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe" 2>/dev/null ||:
    rm -f "${TARGET_TREE_DIR}/ehs_env-${platform}/bin/"*.so   2>/dev/null ||:
}

# ── Binary presence check ─────────────────────────────────────────────────────
# Accepts either ehs.exe (Linux/Windows/embedded) or ehs_${platform}.so
# (Android library targets).
function ci_test_build {
    local exe="${TARGET_TREE_DIR}/ehs_env-${platform}/bin/ehs.exe"
    local so="ehs_${platform}.so"
    if test -e "$exe"; then
        heading "'ehs.exe' present for ${platform}"
        touch "${CI_RESULT_DIR}/${platform}/exe-built.pass"
    elif [ -f "$so" ]; then
        heading "'${so}' built successfully"
        touch "${CI_RESULT_DIR}/${platform}/exe-built.pass"
    else
        err "'ehs.exe' / '${so}' not found for ${platform}"
        touch "${CI_RESULT_DIR}/${platform}/exe-built.fail"
    fi
}

# ── Log tail display on failure ───────────────────────────────────────────────
# Prints the last 20 lines of a build log to the console — useful when
# watching a live terminal session.
function ci_display_log_tail {
    local log="$1"
    printf '%s\n' ">>>>>> START LOG TAIL (${log}) >>>>>>"
    tail -n 20 "$log"
    printf '%s\n' "<<<<<< END LOG TAIL <<<<<<"
}

# ── Claude-assisted failure hook ──────────────────────────────────────────────
# Arguments: $1 = path to build log,  $2 = human-readable stage name.
# Reads CLAUDE_AUTOFIX and CLAUDE_INTERACTIVE globals; uses $platform.
function ci_on_build_failure {
    local log="$1"
    local stage="$2"

    [ "${CLAUDE_AUTOFIX:-0}"     -eq 0 ] && \
    [ "${CLAUDE_INTERACTIVE:-0}" -eq 0 ] && return

    if ! command -v claude &>/dev/null; then
        warn "'claude' not found in PATH — skipping Claude assistance for ${platform} / ${stage}"
        return
    fi

    local prompt
    prompt="$(printf \
'Regression build failure detected.
  Platform : %s
  Stage    : %s
  Log file : %s

Please read the build log at the path above, diagnose the root cause of the
failure, and apply source-level fixes to the repository files.
Do not run any build or make commands.' \
        "${platform}" "${stage}" "${log}")"

    if [ "${CLAUDE_AUTOFIX:-0}" -eq 1 ]; then
        echo "--- claude --autofix: ${platform} / ${stage} ---"
        claude --print "$prompt"
    else
        echo "--- claude --interactive: ${platform} / ${stage} ---"
        echo "    Exit the Claude session (Ctrl-D or /exit) to continue the regression run."
        claude "$prompt"
    fi
}

# Once-per-run preparation: host tools and the dependency repos.
#
# Both are target-independent. configure_host.sh installs machine-level build
# tools; the prepdeps repo fetch clones/pulls ert-build-support,
# ert-contrib-middleware and EHS-kernel whole, to fixed paths. Running either
# per platform meant a 37-platform sweep repeated them 37 times for no benefit.
#
# The per-platform loop still calls 'SKIP_REPOS=1 make prepdeps' for the checks.
function ci_prepare_run {
    local log="${CI_RESULT_DIR}/prepare-run.log"
    mkdir -p "${CI_RESULT_DIR}"
    : > "$log"

    # INX_NONINTERACTIVE=1: never sudo, never prompt. If the machine is missing a
    # build tool the script lists what to install and exits non-zero, instead of
    # blocking an unattended sweep on a password or a keypress.
    INX_NONINTERACTIVE=1 make configure-host >>"$log" 2>&1 || {
        err "configure-host failed — this machine is missing build tools."
        tail -n 20 "$log"
        return 1
    }

    # Needs a configured target for chkconfig; whichever platform is current will
    # do, since the fetch itself does not depend on it.
    make prepdeps >>"$log" 2>&1 \
        || warn "dependency repo fetch reported a problem — see ${log}"
}

# ── Unified build function ────────────────────────────────────────────────────
# Globals read:
#   platform  CI_RESULT_DIR  ROOT_DIR
#   BUILD_ONLY  COMPILE_ONLY  CLAUDE_AUTOFIX  CLAUDE_INTERACTIVE
#
# Build strategy:
#   • If target/platform/${platform}/Dockerimagename exists → make all_docker
#   • Otherwise                                             → make -j 8 (host build)
# After a successful compile + targetenv:
#   • If ehs_${platform}.so exists and COMPILE_ONLY != yes → make targetenv_apk
#   • If platform name contains '_unity' and not unity-lib  → make targetenv_unity_export
#
# Returns 0 on full success, 1 on any failure.
function ci_build_target {
    if [ ! -d ../ert-components ]; then
        err "Not in expected directory layout — ${PWD} has no ../ert-components sibling"
        return 1
    fi
    cd "${ROOT_DIR}/../ert-components" || return 1

    local build_log="${CI_RESULT_DIR}/${platform}/build.log"
    local fail=0

    # Truncate / create the log for this platform
    > "$build_log"

    make clean                >>"$build_log" 2>&1 ||:
    rm -f ./*.o ./*.obj
    ./configure "${platform}" >>"$build_log" 2>&1 || {
        err "Platform '${platform}' not found — skipping"
        return 1
    }
    make targetenv_cleanall >>"$build_log" 2>&1 ||:
    # SKIP_REPOS=1: the dependency-repo fetch is target-independent (whole repos to
    # fixed paths) and the runner does it once before the loop - see ci_prepare_run.
    # What is still wanted per platform are the checks at the end of prepdeps, which
    # need the make-exported environment (EHS_ARCH, TARGET) that ./configure has not
    # got. Without SKIP_REPOS this re-ran 'git pull' + a full 'git lfs pull' on two
    # large LFS repos once per platform to fetch nothing.
    #
    # The result is no longer discarded with '||:'. A failure here used to be silent
    # and the build then died further down with a confusing missing-header error
    # instead of the real cause. Warn rather than abort, so a platform that was
    # building fine does not start failing on a check.
    SKIP_REPOS=1 make prepdeps >>"$build_log" 2>&1 \
        || warn "prepdeps checks failed for ${platform} — see ${build_log}"

    touch "${CI_RESULT_DIR}/${platform}/build"

    # ── Zephyr: three-step CMake-master build ────────────────────────────────
    # Zephyr targets reject 'make all_docker' at parse time, so without this they
    # fail on the build command rather than on anything real. The SDK fetch is a
    # no-op once the shared ../TARGET_SRC_STAGING tree for that (manifest,
    # version) exists, so only the first Zephyr target in a sweep pays for it.
    if [ -f "target/platform/${platform}/zephyr" ] \
       || case "$platform" in zephyr_*) true ;; *) false ;; esac; then
        if make zephyr_cmake_gen    >>"$build_log" 2>&1 \
        && make zephyr_prepdeps     >>"$build_log" 2>&1 \
        && make zephyr_build_docker >>"$build_log" 2>&1; then
            heading "${platform} Zephyr build success"
            touch "${CI_RESULT_DIR}/${platform}/build.pass"
        else
            err "${platform} Zephyr build failed — targetenv not run"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/build.fail"
            touch "${CI_RESULT_DIR}/${platform}/build-target.notrun"
            ci_on_build_failure "$build_log" "zephyr build"
            return 1
        fi
    # ── Docker or host compile ────────────────────────────────────────────────
    elif [ -f "target/platform/${platform}/Dockerimagename" ]; then
        if make all_docker >>"$build_log" 2>&1; then
            heading "${platform} Docker build success"
            touch "${CI_RESULT_DIR}/${platform}/build.pass"
        else
            err "${platform} Docker build failed — targetenv not run"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/build.fail"
            touch "${CI_RESULT_DIR}/${platform}/build-target.notrun"
            ci_on_build_failure "$build_log" "docker build"
            return 1
        fi
    else
        if make -j 8 >>"$build_log" 2>&1; then
            heading "${platform} host build success"
            touch "${CI_RESULT_DIR}/${platform}/build.pass"
        else
            err "${platform} host build failed — targetenv not run"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/build.fail"
            touch "${CI_RESULT_DIR}/${platform}/build-target.notrun"
            ci_on_build_failure "$build_log" "host build"
            return 1
        fi
    fi

    # ── BUILD_ONLY early exit ────────────────────────────────────────────────
    if [ "${BUILD_ONLY:-}" = "yes" ]; then
        touch "${CI_RESULT_DIR}/${platform}/build-target.skip"
        return 0
    fi

    # ── targetenv ─────────────────────────────────────────────────────────────
    # Zephyr has no targetenv stage: zephyr_build_docker already places
    # zephyr.hex/.elf/.bin in ehs_env-<target>/bin/, which is what targetenv
    # produces for other targets. Running it would fail on a stage that does not
    # exist for this os-arch.
    if case "$platform" in zephyr_*) true ;; *) false ;; esac; then
        touch "${CI_RESULT_DIR}/${platform}/build-target.skip"
        return 0
    fi

    if make targetenv >>"$build_log" 2>&1; then
        heading "${platform} targetenv success"
        touch "${CI_RESULT_DIR}/${platform}/build-target.pass"
    else
        err "${platform} targetenv build failed"
        ci_display_log_tail "$build_log"
        touch "${CI_RESULT_DIR}/${platform}/build-target.fail"
        ci_on_build_failure "$build_log" "targetenv"
        return 1
    fi

    [ "${COMPILE_ONLY:-}" = "yes" ] && return 0

    # ── Packaging (opt-in via --package) ──────────────────────────────────────
    # `make targetenv_package` is a proxy: it depends on whichever packaging goal
    # EHS_PACKAGER_TYPE names (deb/apk/nsis/esp32/xmos/arduino), defaulted per
    # os-arch. CI needs no knowledge of which packager a platform uses, and
    # adding a new one needs no change here.
    if [ "${RUN_PACKAGE:-0}" -eq 1 ]; then
        if make targetenv_package >>"$build_log" 2>&1; then
            heading "${platform} package success"
            touch "${CI_RESULT_DIR}/${platform}/package.pass"
        else
            err "${platform} packaging failed"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/package.fail"
            ci_on_build_failure "$build_log" "targetenv_package"
        fi
    fi

    # ── Android APK (only when .so was produced) ──────────────────────────────
    if [ -f "ehs_${platform}.so" ]; then
        echo "Found ehs_${platform}.so — building APK"
        if make targetenv_apk >>"$build_log" 2>&1; then
            heading "${platform} APK build success"
            touch "${CI_RESULT_DIR}/${platform}/build-apk.pass"
        else
            err "${platform} APK build failed"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/build-apk.fail"
            ci_on_build_failure "$build_log" "targetenv_apk"
            fail=1
        fi
    fi

    # ── Unity export ──────────────────────────────────────────────────────────
    if [[ "${platform}" == *"_unity"* && "${platform}" != "linux_android_arm64_unity-lib" ]]; then
        echo "Building Unity export for ${platform}"
        if make targetenv_unity_export >>"$build_log" 2>&1; then
            heading "${platform} Unity export success"
            touch "${CI_RESULT_DIR}/${platform}/build-unity.pass"
        else
            err "${platform} Unity export failed"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/build-unity.fail"
            ci_on_build_failure "$build_log" "targetenv_unity_export"
            fail=1
        fi
    fi

    return $fail
}

# ── SBOM generation ──────────────────────────────────────────────────────────
# Runs 'make sbom' for the current platform.
# Must be called while the build tree is still configured for $platform
# (i.e. after ci_build_target has run ./configure for this platform).
#
# Output location depends on SBOM_RELEASE_MODE:
#
#   SBOM_RELEASE_MODE=0 (default — CI / routine regression):
#     SBOM files  → CI_RESULT_DIR/<platform>/sbom/SBOM.spdx + SBOM_SUMMARY.md
#     DEPENDENTS.md   NOT written (SBOM_SKIP_DEPENDENTS=1 passed to make)
#     Use this for routine builds; output is local to the CI results tree and
#     will not pollute Releases/ or the sibling dependency repos.
#
#   SBOM_RELEASE_MODE=1 (release report — set by --release-report):
#     SBOM files  → Releases/SBOM/<version>/<platform>/SBOM.spdx + SBOM_SUMMARY.md
#     DEPENDENTS.md   written to each dependency root (ert-contrib-middleware etc.)
#     Use this only via update_release_report.sh or run_regression.sh --release-report.
#
# Flag files in CI_RESULT_DIR/<platform>/:
#   sbom.pass   — make sbom succeeded
#   sbom.fail   — make sbom failed
#   sbom.skip   — skipped because TARGET.cfg does not match $platform
#                 (configure must have failed earlier)
function ci_generate_sbom {
    # Safety guard: only generate SBOM when the build tree is actually configured
    # for this platform.  If ./configure failed inside ci_build_target, TARGET.cfg
    # might still show the previous platform — running make sbom then would produce
    # an SBOM for the wrong platform.
    local cfg_target
    cfg_target="$(grep -m1 '^TARGET=' TARGET.cfg 2>/dev/null | cut -d= -f2)"
    if [ "$cfg_target" != "$platform" ]; then
        warn "SBOM skipped for ${platform}: TARGET.cfg='${cfg_target}' (configure may have failed)"
        touch "${CI_RESULT_DIR}/${platform}/sbom.skip"
        return 0
    fi

    local build_log="${CI_RESULT_DIR}/${platform}/build.log"

    if [ "${SBOM_RELEASE_MODE:-0}" -eq 1 ]; then
        # Release mode — write to Releases/SBOM/<version>/<platform>/, update DEPENDENTS.md
        echo "Generating SBOM (release) for ${platform}..."
        if make sbom >>"$build_log" 2>&1; then
            heading "SBOM generated → Releases/SBOM/…/${platform}/"
            touch "${CI_RESULT_DIR}/${platform}/sbom.pass"
        else
            err "SBOM generation failed for ${platform}"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/sbom.fail"
            return 1
        fi
    else
        # CI mode — redirect to CI results dir, skip DEPENDENTS.md
        local sbom_ci_dir="${CI_RESULT_DIR}/${platform}/sbom"
        echo "Generating SBOM (CI) for ${platform} → ${sbom_ci_dir}/"
        if make sbom \
               SBOM_OUT_DIR="${sbom_ci_dir}" \
               SBOM_SKIP_DEPENDENTS=1 \
               >>"$build_log" 2>&1; then
            heading "SBOM generated → results/${platform}/sbom/"
            touch "${CI_RESULT_DIR}/${platform}/sbom.pass"
        else
            err "SBOM generation failed for ${platform}"
            ci_display_log_tail "$build_log"
            touch "${CI_RESULT_DIR}/${platform}/sbom.fail"
            return 1
        fi
    fi
}

# ── Pre-flight: CDF hash uniqueness and CDF/header consistency ────────────────
# Must be called from ${ROOT_DIR}/../ert-components.
# Returns 0 on pass, 1 on failure.
function ci_run_preflight {
    local search_root="${1:-Common/Components}"
    echo "Pre-flight: checking function-block ID uniqueness and CDF/header consistency..."
    if python3 scripts/inxware-id-tool/check_cdf_hashes.py "$search_root"; then
        heading "CDF hash check passed"
    else
        err "CDF hash check FAILED — fix mismatches before building"
        return 1
    fi

    # The public-mirror allow-list must not reach anything private. The check is
    # instant (~0.1s, no builds) and needs no mirror checkout, so it is safe on a
    # CI runner. It catches the case a flat file list never could: a target whose
    # own name looks fine but which includes a customer devman-config.
    echo "Pre-flight: checking the GitHub mirror allow-list for leaks..."
    if python3 scripts/git-utilities/publish_set.py --check; then
        heading "Mirror allow-list check passed"
    else
        err "Mirror allow-list check FAILED — a private path is reachable from the allow-list"
        err "Fix scripts/git-utilities/publish_set.txt before building"
        return 1
    fi

    # Every target's resolved kernel key must have a directory. A target whose
    # key is missing does not fail loudly at build time — it links a stale
    # archive or silently falls back — so it is asserted here instead. Only NEW
    # breakage fails; the already-broken set lives in
    # SystemTests/CI/known-broken-keys.txt.
    #
    # Rewrites TARGET.cfg as it goes, so it must run inside run_regression.sh's
    # save/restore region (it also saves and restores on its own behalf, so it
    # is safe to run by hand).
    # Producer/consumer key agreement. Reported, not enforced: there is a known
    # backlog (10 keys nothing builds, 2 nothing reads) that A3 has to clear
    # first. One line, so it stays readable rather than becoming noise to skip.
    # Make it fatal once that backlog is empty.
    python3 scripts/software-utilities/check_key_agreement.py --summary || true

    echo "Pre-flight: checking every target's dependency keys resolve..."
    if ./scripts/software-utilities/check_target_keys.sh; then
        heading "Target key check passed"
        return 0
    else
        err "Target key check FAILED — a target's kernel key no longer resolves"
        err "If the breakage is intended, re-baseline with:"
        err "  ./scripts/software-utilities/check_target_keys.sh --update-baseline"
        return 1
    fi
}

# ── Feature compliance matrix ─────────────────────────────────────────────────
# Calls fb_platform_matrix.py and writes output into CI_RESULT_DIR.
# Must be called from ${ROOT_DIR}/../ert-components.
#
# $1 = output format: csv | md | html | all   (default: md)
#
# Output location:
#   csv/md/html  → ${CI_RESULT_DIR}/feature_matrix.<fmt>   (via stdout redirect)
#   all          → ${CI_RESULT_DIR}/matrix.{csv,md,html}   (fb_platform_matrix.py
#                  writes files itself when multiple formats are requested; we run
#                  it from CI_RESULT_DIR so the files land in the right place)
function ci_generate_matrix {
    local fmt="${1:-md}"

    if [ "$fmt" = "all" ]; then
        echo "Generating feature compliance matrix (all formats) → ${CI_REPORT_DIR}/matrix.{csv,md,html}"
        # fb_platform_matrix.py writes matrix.csv/md/html to cwd when given 'all'
        if ( cd "${CI_REPORT_DIR}" && \
             python3 "${OLDPWD}/scripts/software-utilities/fb_platform_matrix.py" \
                     --format all ); then
            heading "Feature matrices written to ${CI_REPORT_DIR}/matrix.{csv,md,html}"
        else
            err "Feature matrix generation failed"
            return 1
        fi
    else
        local out="${CI_REPORT_DIR}/feature_matrix.${fmt}"
        echo "Generating feature compliance matrix (${fmt}) → ${out}"
        # Single-format output goes to stdout; redirect it to the results dir.
        if python3 scripts/software-utilities/fb_platform_matrix.py \
                   --format "$fmt" > "$out"; then
            heading "Feature matrix written to ${out}"
        else
            err "Feature matrix generation failed"
            return 1
        fi
    fi
}

# ── Colourised results summary ────────────────────────────────────────────────
function ci_show_summary {
    echo
    echo "=============================== RESULTS =============================="
    LS_COLORS="${LS_COLORS:-}*.fail=00;31:*.pass=00;32:*.skip=00;33:" \
        ls -l --color=always "${CI_RESULT_DIR}"/*/  2>/dev/null || true
}

# ── Overall exit code ─────────────────────────────────────────────────────────
# Returns 1 if any .fail flag file exists anywhere in the results tree.
function ci_exit_code {
    ls -- "${CI_RESULT_DIR%/}"/*/*.fail >/dev/null 2>&1 && return 1 || return 0
}
