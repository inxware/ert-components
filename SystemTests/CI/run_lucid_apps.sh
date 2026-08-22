#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# run_lucid_apps.sh — run every Lucid application in a tree against a host eRT
#                     build and classify what happened to each one.
#
# SCOPE — two questions. Every app: does it load and stay up. Apps authored as
# tests: does it produce the expected result, by diffing what it wrote against
# the test_result.txt checked in beside it. The second needs --assert, which is
# on by default for the unit and system suites and off for external.
#
# Input : a directory tree containing .lpj projects with exported SODL, and a
#         built host target tree under ../TARGET_TREES/ehs_env-<target>.
# Output: SystemTests/reports/<suite>/<UTC-timestamp>/ containing summary.md,
#         results.csv, junit.xml and logs/<app>.log — one timestamped run per
#         invocation, so the directory accumulates history.
#
#   ./SystemTests/CI/run_lucid_apps.sh --docker              # ../apps
#   ./SystemTests/CI/run_lucid_apps.sh --suite unit          # component tests
#   ./SystemTests/CI/run_lucid_apps.sh --filter tutorials    # subset
#
# --docker runs ehs.exe inside the target's build image instead of on the host.
# Needed whenever the build links system libraries the host does not have: the
# debian13 targets want OpenCV 4.10, which an Ubuntu host does not carry. The
# pre-flight resolves missing libraries once and says which are unsatisfied, so
# a wrong image shows up immediately rather than as 165 spurious failures.
#
# SDL_VIDEODRIVER defaults to 'offscreen', so LVGL/SDL targets need no display
# and do not open 165 windows. Pass --sdl-driver x11 to watch them.
#
# STAGING
#
# The env tree is copied to ../TARGET_TREES/ehs-env-test-<target> and the app
# under test is written into that copy's appdata/default. The real
# ehs_env-<target> is never touched, so a default app installed there survives.
#
# CLASSIFICATION
#
#   RUNS       parsed, still alive when the timeout expired — the good case
#   EXITED     parsed, then exited by itself before the timeout
#   PARSE-FAIL kernel rejected the SODL; the error lines are in the report
#   CRASH      killed by SIGSEGV/SIGABRT/SIGBUS/SIGFPE/SIGILL
#   NO-RUN     started but never reported a completed parse, or exited non-zero
#   NO-ERT1    no SODL v1 export present — skipped, not a failure
#
# Order matters: a parse failure leaves the kernel alive and waiting for a new
# app, so it exits via the timeout exactly like a healthy app. The console
# markers are therefore checked before the exit status, never after.
#
# WHY THE CONSOLE MARKERS
#
# EHS-kernel/Common/Kernel/messages.h emits fixed strings for parse outcomes
# ('**Info: ...Parsed OK', '**Error: Parsing FAILED!') and notes that Lucid
# Tools already keys off them, so they are a de-facto interface rather than
# incidental log text. They reach stdout only when EHS_RUNTIME_LOGGER_ENABLED
# is set for the target. If no app emits a '**' record the run says the results
# are unreliable rather than reporting a screen of false NO-RUNs.
#
# ehs.exe is launched directly rather than through bin/run_ehs.sh, which runs
# './ehs.exe || echo ...' and so discards the exit status CRASH detection needs.
# run_ehs.sh's INXWAREROOT/DEVMANCOREDIR/SYSDATA are not exported here because
# ehs.exe never reads them — they are for the env tree's own shell scripts
# (runOsInit.sh, stop_ehs.sh, devman/core/*). Setting them would be inert noise.
# The runtime finds sysdata/ from its install path, not from $SYSDATA.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

TARGET="linux_x86_64_clang_lvgl_tflite_debian13"
SUITE="external"
TIMEOUT=8
FILTER=""
APPS_DIR=""
DOCKER=0
IMAGE="inxware/inx-debian13-clang-lvgl-axelera:latest"
SDL_DRV="offscreen"
EXTRA_LIBS=""
ASSERT=-1          # -1 = decide from the suite; unit and system assert by default

while [ $# -gt 0 ]; do
    case "$1" in
        --target)   TARGET="${2:-}"; shift 2 ;;
        --suite)    SUITE="${2:-}"; shift 2 ;;
        --timeout)  TIMEOUT="${2:-}"; shift 2 ;;
        --filter)   FILTER="${2:-}"; shift 2 ;;
        --apps-dir) APPS_DIR="${2:-}"; shift 2 ;;
        --docker)   DOCKER=1; shift ;;
        --image)    IMAGE="${2:-}"; DOCKER=1; shift 2 ;;
        --sdl-driver) SDL_DRV="${2:-}"; shift 2 ;;
        --libs)     EXTRA_LIBS="${2:-}"; shift 2 ;;
        --assert)   ASSERT=1; shift ;;
        --no-assert) ASSERT=0; shift ;;
        -h|--help)  sed -n '6,70p' "$0"; exit 0 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
done

source "${ROOT}/scripts/build-function-library/colour.sh"
note() { printf '   %s\n' "$*"; }
bad()  { printf '   %s%s%s\n' "${TXT_FG_BRIGHT_RED}"    "$*" "${TXT_RESET}"; }
warnl(){ printf '   %s%s%s\n' "${TXT_FG_BRIGHT_YELLOW}" "$*" "${TXT_RESET}"; }
good() { printf '   %s%s%s\n' "${TXT_FG_GREEN}"         "$*" "${TXT_RESET}"; }
die()  { bad "$*"; exit 1; }

# ── Suite selects the app tree and the report directory ──────────────────────
# unit discovers tests wherever they sit under the component tree, so a test
# added beside its function block is picked up with no list to maintain. Both
# 'tests' and 'test' are matched: 34 components use the former, 8 the latter.
PATH_FILTER=""
case "$SUITE" in
    external) DEFAULT_APPS="${ROOT}/../apps";          REPORT_SUB="external-apps" ;;
    system)   DEFAULT_APPS="${ROOT}/SystemTests/apps"; REPORT_SUB="system-apps" ;;
    unit)     DEFAULT_APPS="${ROOT}/Common/Components"; REPORT_SUB="unit-tests"
              PATH_FILTER="/tests?/" ;;
    *) die "unknown --suite '${SUITE}' (external|system|unit)" ;;
esac
# unit and system apps carry an expected result; ../apps generally do not.
if [ "$ASSERT" -eq -1 ]; then
    case "$SUITE" in unit|system) ASSERT=1 ;; *) ASSERT=0 ;; esac
fi
[ -n "$APPS_DIR" ] || APPS_DIR="$DEFAULT_APPS"
[ -d "$APPS_DIR" ] || die "app tree not found: ${APPS_DIR}"
APPS_DIR="$(cd "$APPS_DIR" && pwd)"

ENV_SRC="${ROOT}/../TARGET_TREES/ehs_env-${TARGET}"
STAGE="${ROOT}/../TARGET_TREES/ehs-env-test-${TARGET}"
[ -x "${ENV_SRC}/bin/ehs.exe" ] || die "no built env: ${ENV_SRC}/bin/ehs.exe
   Build it first:  ./configure ${TARGET} && make && make targetenv"

STAMP="$(date -u +%Y%m%dT%H%M%SZ)"
OUT="${ROOT}/SystemTests/reports/${REPORT_SUB}/${STAMP}"
mkdir -p "${OUT}/logs" || die "cannot create ${OUT}"

printf '\n%s── run_lucid_apps%s\n' "${TXT_BOLD}${TXT_FG_BRIGHT_WHITE}" "${TXT_RESET}"
note "target : ${TARGET}"
note "apps   : ${APPS_DIR}"
note "stage  : ${STAGE}"
note "report : ${OUT}"
note "timeout: ${TIMEOUT}s per app"


# ── Stage a private copy of the env tree ────────────────────────────────────
note "staging env copy..."
rm -rf "$STAGE"
cp -r "$ENV_SRC" "$STAGE" || die "could not stage ${STAGE}"
STAGE="$(cd "$STAGE" && pwd)"
# Not every target's targetenv produces the full tree; the debian13 ones ship
# only bin/ and sysdata/. appdata/ is where the app under test goes.
mkdir -p "${STAGE}/appdata" "${STAGE}/userdata" "${STAGE}/devman/core"

MOUNT="$(cd "${ROOT}/.." && pwd)"

# ── Resolve shared libraries once, in the environment that will run ─────────
# Anything still 'not found' after this produces a load failure for every app,
# so it is worth one check up front rather than 165 identical logs.
_ldd_missing() {
    if [ "$DOCKER" -eq 1 ]; then
        docker run --rm --user "$(id -u):$(id -g)" -v "${MOUNT}:${MOUNT}" \
            -w "${STAGE}/bin" -e LD_LIBRARY_PATH="$1" "$IMAGE" \
            ldd ./ehs.exe 2>/dev/null | awk '/not found/{print $1}'
    else
        ( cd "${STAGE}/bin" && LD_LIBRARY_PATH="$1" ldd ./ehs.exe 2>/dev/null \
          | awk '/not found/{print $1}' )
    fi
}

LIBPATH="${STAGE}/bin/cslib"
[ -n "$EXTRA_LIBS" ] && LIBPATH="${LIBPATH}:${EXTRA_LIBS}"
CONTRIB="${ROOT}/../ert-contrib-middleware/target_libs"
_missing="$(_ldd_missing "$LIBPATH")"
# contrib holds the same library name for every architecture, so a candidate is
# only usable if its ELF e_machine matches the binary's. Taking the first match
# by name picks an arm64 .so for an x86-64 build.
_elf_machine() { od -An -tx1 -j18 -N2 "$1" 2>/dev/null | tr -d ' \n'; }
_EXE_MACH="$(_elf_machine "${STAGE}/bin/ehs.exe")"
if [ -n "$_missing" ]; then
    for _so in $_missing; do
        while read -r _p; do
            [ -n "$_p" ] || continue
            [ "$(_elf_machine "$_p")" = "$_EXE_MACH" ] || continue
            LIBPATH="${LIBPATH}:$(dirname "$_p")"
            break
        done < <(find "$CONTRIB" -name "$_so" 2>/dev/null)
    done
    _missing="$(_ldd_missing "$LIBPATH")"
fi
if [ -n "$_missing" ]; then
    bad "unresolved shared libraries — every app will fail to load:"
    printf '%s\n' "$_missing" | sed 's/^/        /'
    if [ "$DOCKER" -eq 0 ]; then
        bad "The host is $(grep -oP '(?<=^PRETTY_NAME=").*(?=")' /etc/os-release 2>/dev/null)."
        bad "This target expects its build image's system libraries. Retry with --docker."
    else
        bad "Image ${IMAGE} does not provide them. Pick another with --image."
    fi
    exit 1
fi
good "shared libraries resolved"

if [ "$DOCKER" -eq 1 ]; then
    note "running in ${IMAGE}"
elif [ "$SDL_DRV" != "offscreen" ] && [ -z "${DISPLAY:-}" ] && [ -z "${WAYLAND_DISPLAY:-}" ]; then
    warnl "--sdl-driver ${SDL_DRV} but no DISPLAY; GUI targets may report CRASH spuriously."
fi

# ── Where the app's own output goes ─────────────────────────────────────────
# EhsHUpdateFilePathEnvironment (Common/HAL/file/hal_file.c) derives the app's
# writable directory from $HOME, so pointing HOME into the stage keeps every run
# self-contained and leaves the real home alone — the old runner did
# 'rm -rf ${HOME}/inxware/inx-tests' on every invocation.
#
# The write FB creates results/ itself and needs its parent to exist, so the
# pre-created depth must be exactly <home>/inxware/inx-tests:
#
#   inxware/ only                    -> nothing written (no parent for results/)
#   inxware/inx-tests/               -> writes         (the only correct depth)
#   inxware/inx-tests/results/       -> nothing written (results/ already exists)
#
# One directory too few or too many and the app silently produces no result,
# which reads as a test failure rather than a harness fault.
APP_HOME="${STAGE}/testhome"
APP_RESULTS="${APP_HOME}/inxware/inx-tests/results"

_reset_app_home() {
    rm -rf "$APP_HOME"
    mkdir -p "${APP_HOME}/inxware/inx-tests"
}

# ── Launch one app; $1 is the log file ──────────────────────────────────────
# Deliberately does NOT export INXWAREROOT / DEVMANCOREDIR / SYSDATA. Those are
# for bin/run_ehs.sh and devman; setting them redirects the runtime's own path
# resolution and the app then writes nothing. Only LD_LIBRARY_PATH (missing
# libraries), SDL_VIDEODRIVER (headless) and HOME (above) are set.
_run_app() {
    if [ "$DOCKER" -eq 1 ]; then
        docker run --rm --user "$(id -u):$(id -g)" -v "${MOUNT}:${MOUNT}" \
            -w "${STAGE}/bin" \
            -e LD_LIBRARY_PATH="$LIBPATH" \
            -e SDL_VIDEODRIVER="$SDL_DRV" \
            -e HOME="$APP_HOME" \
            "$IMAGE" \
            timeout --signal=TERM --kill-after=3 "$TIMEOUT" ./ehs.exe >"$1" 2>&1
    else
        (
            cd "${STAGE}/bin" || exit 127
            export LD_LIBRARY_PATH="$LIBPATH"
            export SDL_VIDEODRIVER="$SDL_DRV"
            export HOME="$APP_HOME"
            exec timeout --signal=TERM --kill-after=3 "$TIMEOUT" ./ehs.exe
        ) >"$1" 2>&1
    fi
}

# ── Collect apps ────────────────────────────────────────────────────────────
# One .lpj per project; DEP/ holds generated copies, not projects.
mapfile -t LPJS < <(find "$APPS_DIR" -name '*.lpj' -not -path '*/DEP/*' 2>/dev/null \
                    | { [ -n "$PATH_FILTER" ] && grep -E "$PATH_FILTER" || cat; } | sort)
[ "${#LPJS[@]}" -gt 0 ] || die "no .lpj found under ${APPS_DIR}${PATH_FILTER:+ matching ${PATH_FILTER}}"

# SODL v1 exports start with a 0x02 byte; the deprecated v0 text format starts
# with '#V:'. A v1 target cannot load a v0 export, so those are skipped rather
# than reported as parse failures.
_is_v1() { [ "$(od -An -tx1 -N1 "$1" 2>/dev/null | tr -d ' ')" = "02" ]; }

_find_export() {
    local app="$1" d
    for d in "export-ert1" "export"; do
        if [ -f "${app}/${d}/t.sdl" ] && _is_v1 "${app}/${d}/t.sdl"; then
            printf '%s' "${app}/${d}"; return 0
        fi
    done
    return 1
}

declare -a R_NAME R_STATE R_DETAIL R_MS
n_runs=0; n_exited=0; n_parse=0; n_crash=0; n_norun=0; n_noert1=0
n_pass=0; n_fail=0; n_noresult=0
saw_marker=0

printf '\n'
for lpj in "${LPJS[@]}"; do
    app="$(dirname "$lpj")"
    name="${app#${APPS_DIR}/}"
    [ -n "$FILTER" ] && [[ ! "$name" =~ $FILTER ]] && continue

    slug="$(printf '%s' "$name" | tr '/ ' '__')"
    log="${OUT}/logs/${slug}.log"

    if ! exp="$(_find_export "$app")"; then
        R_NAME+=("$name"); R_STATE+=("NO-ERT1")
        R_DETAIL+=("no SODL v1 export"); R_MS+=("0")
        n_noert1=$((n_noert1 + 1))
        printf '   %-58.58s %s%s%s\n' "$name" "${TXT_FG_BRIGHT_YELLOW}" "NO-ERT1" "${TXT_RESET}"
        continue
    fi

    # Fresh app slot for every run; a stale default would silently be re-tested.
    rm -rf "${STAGE}/appdata/default" "${STAGE}/appdata/temp"/*
    cp -r "$exp" "${STAGE}/appdata/default" 2>/dev/null
    _reset_app_home

    _t0=$(date +%s%N)
    _run_app "$log"
    rc=$?
    _ms=$(( ($(date +%s%N) - _t0) / 1000000 ))

    parsed_ok=0;  grep -aq 'Parsed OK' "$log" 2>/dev/null && parsed_ok=1
    parse_err=0
    grep -aqE 'Parsing FAILED|Wrong SODL format|Couldn.t open SODL file' "$log" 2>/dev/null && parse_err=1
    grep -aq '\*\*' "$log" 2>/dev/null && saw_marker=1

    detail=""
    if [ "$parse_err" -eq 1 ]; then
        state="PARSE-FAIL"
        detail="$(grep -aoE '\*\*Error:?[^\r]*' "$log" 2>/dev/null \
                  | head -2 | paste -sd';' - | cut -c1-150)"
        [ -n "$detail" ] || detail="parse rejected"
        n_parse=$((n_parse + 1))
    elif [ "$rc" -ge 128 ] && [[ " 132 134 135 136 139 " == *" $rc "* ]]; then
        state="CRASH"; detail="signal $((rc - 128))"
        n_crash=$((n_crash + 1))
    elif [ "$rc" -eq 124 ] || [ "$rc" -eq 137 ]; then
        if [ "$parsed_ok" -eq 1 ]; then
            state="RUNS"; detail="alive at ${TIMEOUT}s"
            n_runs=$((n_runs + 1))
        else
            state="NO-RUN"; detail="no completed parse before timeout"
            n_norun=$((n_norun + 1))
        fi
    elif [ "$rc" -eq 0 ] && [ "$parsed_ok" -eq 1 ]; then
        state="EXITED"; detail="exited cleanly after ${_ms}ms"
        n_exited=$((n_exited + 1))
    else
        state="NO-RUN"; detail="exit ${rc}"
        n_norun=$((n_norun + 1))
    fi

    # --assert: the app is also a test, so compare what it wrote against the
    # expected result checked in beside it. Only meaningful once it actually
    # loaded, so a parse failure or crash keeps its own state.
    if [ "$ASSERT" -eq 1 ] && [ -f "${app}/test_result.txt" ] \
       && [ "$state" != "PARSE-FAIL" ] && [ "$state" != "CRASH" ]; then
        case "$state" in RUNS) n_runs=$((n_runs - 1)) ;; EXITED) n_exited=$((n_exited - 1)) ;;
                         NO-RUN) n_norun=$((n_norun - 1)) ;; esac
        if [ ! -f "${APP_RESULTS}/test_result.txt" ]; then
            state="NO-RESULT"
            detail="app wrote no result in ${TIMEOUT}s"
            n_noresult=$((n_noresult + 1))
        elif diff -q "${APP_RESULTS}/test_result.txt" "${app}/test_result.txt" >/dev/null 2>&1; then
            state="PASS"; detail="matches expected"
            n_pass=$((n_pass + 1))
        else
            state="FAIL"
            detail="got [$(head -c 60 "${APP_RESULTS}/test_result.txt" | tr -d '\n')] want [$(head -c 60 "${app}/test_result.txt" | tr -d '\n')]"
            n_fail=$((n_fail + 1))
        fi
        cp -f "${APP_RESULTS}/test_result.txt" "${OUT}/logs/${slug}.result" 2>/dev/null
    fi

    R_NAME+=("$name"); R_STATE+=("$state")
    R_DETAIL+=("$detail"); R_MS+=("$_ms")

    case "$state" in
        PASS|RUNS|EXITED)      _c="${TXT_FG_GREEN}" ;;
        PARSE-FAIL|NO-RESULT)  _c="${TXT_FG_BRIGHT_YELLOW}" ;;
        *)                     _c="${TXT_FG_BRIGHT_RED}" ;;
    esac
    printf '   %-58.58s %s%s%s\n' "$name" "$_c" "$state" "${TXT_RESET}"
done

TOTAL=${#R_NAME[@]}
[ "$TOTAL" -gt 0 ] || die "no apps matched --filter '${FILTER}'"

# No '**' record from any app means the kernel's console messages never reached
# stdout — EHS_RUNTIME_LOGGER_ENABLED is off for this target. Every result is
# then meaningless rather than merely bad, so say so instead of reporting it.
if [ "$saw_marker" -eq 0 ] && [ "$((TOTAL - n_noert1))" -gt 0 ]; then
    printf '\n'
    bad "No app emitted a single '**' kernel record."
    bad "EHS_RUNTIME_LOGGER_ENABLED is off for ${TARGET}, so parse outcomes"
    bad "cannot be observed and every state below is unreliable. Use a target"
    bad "built with the runtime logger enabled."
fi

# ── Reports ─────────────────────────────────────────────────────────────────
# CSV and JUnit for tooling; markdown to read in an editor or a diff.
{
    printf 'app,state,detail,duration_ms\n'
    for i in "${!R_NAME[@]}"; do
        printf '"%s","%s","%s",%s\n' \
            "${R_NAME[$i]}" "${R_STATE[$i]}" "${R_DETAIL[$i]//\"/\'}" "${R_MS[$i]}"
    done
} > "${OUT}/results.csv"

_xml_esc() { printf '%s' "$1" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g'; }
{
    printf '<?xml version="1.0" encoding="UTF-8"?>\n'
    _jf=$((n_parse + n_fail + n_noresult)); _je=$((n_crash + n_norun))
    printf '<testsuites name="lucid-apps" tests="%d" failures="%d" errors="%d" skipped="%d">\n' \
        "$TOTAL" "$_jf" "$_je" "$n_noert1"
    printf '  <testsuite name="%s" tests="%d" failures="%d" errors="%d" skipped="%d">\n' \
        "$(_xml_esc "$REPORT_SUB")" "$TOTAL" "$_jf" "$_je" "$n_noert1"
    for i in "${!R_NAME[@]}"; do
        _n="$(_xml_esc "${R_NAME[$i]}")"; _d="$(_xml_esc "${R_DETAIL[$i]}")"
        printf '    <testcase classname="%s" name="%s" time="%s.%03d">' \
            "$(_xml_esc "$REPORT_SUB")" "$_n" "$(( ${R_MS[$i]} / 1000 ))" "$(( ${R_MS[$i]} % 1000 ))"
        case "${R_STATE[$i]}" in
            PASS|RUNS|EXITED) ;;
            NO-ERT1)          printf '\n      <skipped message="%s"/>\n    ' "$_d" ;;
            PARSE-FAIL|FAIL|NO-RESULT)
                              printf '\n      <failure type="%s" message="%s"/>\n    ' "${R_STATE[$i]}" "$_d" ;;
            *)                printf '\n      <error type="%s" message="%s"/>\n    ' "${R_STATE[$i]}" "$_d" ;;
        esac
        printf '</testcase>\n'
    done
    printf '  </testsuite>\n</testsuites>\n'
} > "${OUT}/junit.xml"

{
    printf '# Lucid app run report — %s\n\n' "$REPORT_SUB"
    printf '| Field | Value |\n|-----------|--------------------------------------|\n'
    printf '| Run (UTC) | %s |\n| Target | `%s` |\n| App tree | `%s` |\n' \
        "$STAMP" "$TARGET" "$APPS_DIR"
    printf '| Timeout | %ss per app |\n| Host | %s |\n\n' "$TIMEOUT" "$(uname -sr)"
    printf '## Totals\n\n'
    printf '| State      | Count | Meaning                                        |\n'
    printf '|------------|-------|------------------------------------------------|\n'
    if [ "$ASSERT" -eq 1 ]; then
    printf '| PASS       | %5d | result matches the expected file               |\n' "$n_pass"
    printf '| FAIL       | %5d | result differs from expected                   |\n' "$n_fail"
    printf '| NO-RESULT  | %5d | loaded but wrote no result                     |\n' "$n_noresult"
    fi
    printf '| RUNS       | %5d | parsed, still alive at the timeout             |\n' "$n_runs"
    printf '| EXITED     | %5d | parsed, then exited by itself                  |\n' "$n_exited"
    printf '| PARSE-FAIL | %5d | kernel rejected the SODL                       |\n' "$n_parse"
    printf '| CRASH      | %5d | killed by a signal                             |\n' "$n_crash"
    printf '| NO-RUN     | %5d | never reported a completed parse               |\n' "$n_norun"
    printf '| NO-ERT1    | %5d | no SODL v1 export — skipped, not a failure     |\n' "$n_noert1"
    printf '| **total**  | %5d |                                                |\n\n' "$TOTAL"
    printf '## Per app\n\n'
    printf '| App                                                | State      | Detail                             |\n'
    printf '|----------------------------------------------------|------------|------------------------------------|\n'
    for i in "${!R_NAME[@]}"; do
        printf '| %-50.50s | %-10s | %-34.34s |\n' \
            "${R_NAME[$i]}" "${R_STATE[$i]}" "${R_DETAIL[$i]}"
    done
    printf '\nPer-app console output is in `logs/`.\n'
} > "${OUT}/summary.md"

# ── Summary ─────────────────────────────────────────────────────────────────
printf '\n'
[ "$ASSERT" -eq 1 ] && good "PASS ${n_pass}"
good "RUNS ${n_runs}   EXITED ${n_exited}"
[ "$n_fail"     -gt 0 ] && bad "FAIL ${n_fail}"
[ "$n_noresult" -gt 0 ] && warnl "NO-RESULT ${n_noresult}"
[ "$n_parse"    -gt 0 ] && warnl "PARSE-FAIL ${n_parse}"
[ "$n_noert1"   -gt 0 ] && warnl "NO-ERT1 ${n_noert1} (skipped — no v1 export)"
[ "$n_crash"    -gt 0 ] && bad "CRASH ${n_crash}"
[ "$n_norun"    -gt 0 ] && bad "NO-RUN ${n_norun}"
note "of ${TOTAL} apps"
note "report: ${OUT}/summary.md"

# Non-zero only for genuine defects; a missing v1 export is not one.
[ "$((n_parse + n_crash + n_norun + n_fail + n_noresult))" -eq 0 ] && exit 0
exit 1
