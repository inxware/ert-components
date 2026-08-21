#!/bin/bash
#
# stack_report.sh - report per-function stack frame sizes.
#
# Compiles the given sources with -fstack-usage and prints the frames largest
# first. Function blocks run on task stacks that are a few kB on MCU targets, so
# a run function holding a buffer sized from a string-table constant can be the
# whole budget on its own.
#
#   scripts/software-utilities/stack_report.sh Common/Components/core/stringfn.c
#   scripts/software-utilities/stack_report.sh --threshold 4096 Common/Components/core/*.c
#   scripts/software-utilities/stack_report.sh --platform linux_x86_64_clang_gtk_debian10 <src>
#   scripts/software-utilities/stack_report.sh --prebuilt        # read .su files already in the tree
#
# --threshold N exits non-zero if any frame is >= N bytes, so it drops into CI.
#
# Frame sizes are ABI-dependent: this compiles for the host to give a quick
# comparative figure. For the number that matters on a device, build the real
# target and use --prebuilt to read the .su files the cross-compiler left.

set -u

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PLATFORM="linux_x86_64_clang_gtk_debian10"
THRESHOLD=0
PREBUILT=0
OPT="-O2"
SRCS=()

while [ $# -gt 0 ]; do
    case "$1" in
        --platform)  PLATFORM="$2"; shift 2 ;;
        --threshold) THRESHOLD="$2"; shift 2 ;;
        --opt)       OPT="$2"; shift 2 ;;
        --prebuilt)  PREBUILT=1; shift ;;
        -h|--help)   sed -n '2,20p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit 0 ;;
        *)           SRCS+=("$1"); shift ;;
    esac
done

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

collect_prebuilt() {
    find "$ROOT" -name '*.su' -print0 | xargs -0 cat 2>/dev/null
}

compile_and_collect() {
    local inc=(
        -I"$ROOT/target/platform/$PLATFORM"
        -I"$ROOT/target/os-arch/linux_ALL"
        -I"$ROOT/target/os-arch/gnu_ALL"
        -I"$ROOT/target/os-arch/base_full"
        -I"$ROOT/Common/KAPI"
        -I"$ROOT/Common/HAL/include"
        -I"$ROOT/Common/Ehs"
        -I"$ROOT/Common/Components/core"
    )
    local src
    for src in "${SRCS[@]}"; do
        [ -f "$src" ] || { echo "skip (not a file): $src" >&2; continue; }
        ( cd "$WORK" && \
          ${CC:-cc} -c -std=c99 $OPT -fstack-usage -w \
                    -DEHS_TARGET_FP_SUPPORT "${inc[@]}" \
                    -o /dev/null "$(cd "$(dirname "$OLDPWD/$src")" >/dev/null 2>&1 && pwd)/$(basename "$src")" \
                    2>/dev/null ) || echo "warn: $src did not compile for $PLATFORM" >&2
    done
    cat "$WORK"/*.su 2>/dev/null
}

if [ "$PREBUILT" = "1" ]; then
    RAW="$(collect_prebuilt)"
    SOURCE_DESC="pre-built .su files under $ROOT"
else
    if [ ${#SRCS[@]} -eq 0 ]; then
        echo "error: give at least one source file, or use --prebuilt" >&2
        exit 2
    fi
    RAW="$(compile_and_collect)"
    SOURCE_DESC="host build for $PLATFORM ($OPT)"
fi

if [ -z "$RAW" ]; then
    echo "no stack-usage data produced" >&2
    exit 1
fi

echo "Stack frames - $SOURCE_DESC"
echo "-------------------------------------------------------------------------"
printf "%10s  %-8s %s\n" "BYTES" "TYPE" "FUNCTION"

printf '%s\n' "$RAW" \
  | awk -F'\t' 'NF>=2 {
        split($1, a, ":");
        name = a[length(a)];
        printf "%10d  %-8s %s\n", $2, $3, name;
    }' \
  | sort -rn \
  | head -60

if [ "$THRESHOLD" -gt 0 ]; then
    OVER="$(printf '%s\n' "$RAW" | awk -F'\t' -v t="$THRESHOLD" 'NF>=2 && $2+0 >= t {n++} END {print n+0}')"
    echo "-------------------------------------------------------------------------"
    if [ "$OVER" -gt 0 ]; then
        echo "FAIL: $OVER function(s) at or above the $THRESHOLD byte threshold"
        exit 1
    fi
    echo "OK: no function reaches the $THRESHOLD byte threshold"
fi

exit 0
