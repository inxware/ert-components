# shellcheck shell=sh
#---------------------------------------------------------------
# Copyright (C) 2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# This script can be included by other scripts to add some useful variables and aliases
# related to outputting with colour and text styles on the console.

# Idempotency... Re-sourcing is harmless; we just recompute.
: "${TXT_COLOUR_MODE:=none}"

# Policy toggles...
# TXT_FORCE_COLOR=1      # force colours even if stdout is not a TTY
# TXT_FORCE_ANSI=1       # skip tput/terminfo and use ANSI 16
# TXT_FORCE_TPUT16=1     # assume tput 16-colour works (setaf 8..15)

# Can we colour?
can_colour=no
if [ -z "${NO_COLOR:-}" ]; then
    if [ -n "${TXT_FORCE_COLOR:-}" ] || [ -t 1 ]; then
        can_colour=yes
    fi
fi

# Initialise defaults to empty...
TXT_RESET=""
TXT_BOLD=""
TXT_DIM=""
TXT_UNDERLINE=""

TXT_FG_BLACK=""
TXT_BG_BLACK=""
TXT_FG_RED=""
TXT_BG_RED=""
TXT_FG_GREEN=""
TXT_BG_GREEN=""
TXT_FG_YELLOW=""
TXT_BG_YELLOW=""
TXT_FG_BLUE=""
TXT_BG_BLUE=""
TXT_FG_MAGENTA=""
TXT_BG_MAGENTA=""
TXT_FG_CYAN=""
TXT_BG_CYAN=""
TXT_FG_WHITE=""
TXT_BG_WHITE=""
TXT_FG_GREY=""
TXT_BG_GREY=""

TXT_FG_BRIGHT_RED=""
TXT_BG_BRIGHT_RED=""
TXT_FG_BRIGHT_GREEN=""
TXT_BG_BRIGHT_GREEN=""
TXT_FG_BRIGHT_YELLOW=""
TXT_BG_BRIGHT_YELLOW=""
TXT_FG_BRIGHT_BLUE=""
TXT_BG_BRIGHT_BLUE=""
TXT_FG_BRIGHT_MAGENTA=""
TXT_BG_BRIGHT_MAGENTA=""
TXT_FG_BRIGHT_CYAN=""
TXT_BG_BRIGHT_CYAN=""
TXT_FG_BRIGHT_WHITE=""
TXT_BG_BRIGHT_WHITE=""
TXT_COLOUR_MODE="none"

# Detection...
if [ "$can_colour" = yes ]; then
    if [ -z "${TXT_FORCE_ANSI:-}" ] && command -v tput >/dev/null 2>&1 && [ -n "${TERM:-}" ]; then
        ncol="$(tput colors 2>/dev/null || printf 0)"
        if [ "$ncol" -ge 8 ] || [ -n "${TXT_FORCE_TPUT16:-}" ]; then
            # Attributes via terminfo
            TXT_BOLD="$(tput bold 2>/dev/null || printf '')"
            TXT_DIM="$(tput dim 2>/dev/null || printf '')"
            TXT_UNDERLINE="$(tput smul 2>/dev/null || printf '')"
            TXT_RESET="$(tput sgr0 2>/dev/null || printf '')"

            # Core 8 via terminfo
            TXT_FG_BLACK="$(tput setaf 0)"
            TXT_BG_BLACK="$(tput setab 0)"
            TXT_FG_RED="$(tput setaf 1)"
            TXT_BG_RED="$(tput setab 1)"
            TXT_FG_GREEN="$(tput setaf 2)"
            TXT_BG_GREEN="$(tput setab 2)"
            TXT_FG_YELLOW="$(tput setaf 3)"
            TXT_BG_YELLOW="$(tput setab 3)"
            TXT_FG_BLUE="$(tput setaf 4)"
            TXT_BG_BLUE="$(tput setab 4)"
            TXT_FG_MAGENTA="$(tput setaf 5)"
            TXT_BG_MAGENTA="$(tput setab 5)"
            TXT_FG_CYAN="$(tput setaf 6)"
            TXT_BG_CYAN="$(tput setab 6)"
            TXT_FG_WHITE="$(tput setaf 7)"
            TXT_BG_WHITE="$(tput setab 7)"
            TXT_COLOUR_MODE="tput8"

            # Bright via terminfo if supported; otherwise layer ANSI bright sequences
            has_tput16=no
            if [ -n "${TXT_FORCE_TPUT16:-}" ] || [ "$ncol" -ge 16 ]; then
                # Probe setaf 9 works
                if printf %s "$(tput setaf 9 2>/dev/null)" | grep -q .; then
                    has_tput16=yes
                fi
            fi

            if [ "$has_tput16" = yes ]; then
                TXT_FG_GREY="$(tput setaf 8)"
                TXT_BG_GREY="$(tput setab 8)"
                TXT_FG_BRIGHT_RED="$(tput setaf 9)"
                TXT_BG_BRIGHT_RED="$(tput setab 9)"
                TXT_FG_BRIGHT_GREEN="$(tput setaf 10)"
                TXT_BG_BRIGHT_GREEN="$(tput setab 10)"
                TXT_FG_BRIGHT_YELLOW="$(tput setaf 11)"
                TXT_BG_BRIGHT_YELLOW="$(tput setab 11)"
                TXT_FG_BRIGHT_BLUE="$(tput setaf 12)"
                TXT_BG_BRIGHT_BLUE="$(tput setab 12)"
                TXT_FG_BRIGHT_MAGENTA="$(tput setaf 13)"
                TXT_BG_BRIGHT_MAGENTA="$(tput setab 13)"
                TXT_FG_BRIGHT_CYAN="$(tput setaf 14)"
                TXT_BG_BRIGHT_CYAN="$(tput setab 14)"
                TXT_FG_BRIGHT_WHITE="$(tput setaf 15)"
                TXT_BG_BRIGHT_WHITE="$(tput setab 15)"
                TXT_COLOUR_MODE="tput16"
            else
                # ANSI bright sequences
                TXT_FG_GREY=$'\033[90m'
                TXT_BG_GREY=$'\033[100m'
                TXT_FG_BRIGHT_RED=$'\033[91m'
                TXT_BG_BRIGHT_RED=$'\033[101m'
                TXT_FG_BRIGHT_GREEN=$'\033[92m'
                TXT_BG_BRIGHT_GREEN=$'\033[102m'
                TXT_FG_BRIGHT_YELLOW=$'\033[93m'
                TXT_BG_BRIGHT_YELLOW=$'\033[103m'
                TXT_FG_BRIGHT_BLUE=$'\033[94m'
                TXT_BG_BRIGHT_BLUE=$'\033[104m'
                TXT_FG_BRIGHT_MAGENTA=$'\033[95m'
                TXT_BG_BRIGHT_MAGENTA=$'\033[105m'
                TXT_FG_BRIGHT_CYAN=$'\033[96m'
                TXT_BG_BRIGHT_CYAN=$'\033[106m'
                TXT_FG_BRIGHT_WHITE=$'\033[97m'
                TXT_BG_BRIGHT_WHITE=$'\033[107m'
                TXT_COLOUR_MODE="tput8+ansi16"
            fi
        fi
    fi

    # Pure ANSI-16 fallback (or forced)
    if [ "$TXT_COLOUR_MODE" = none ]; then
        TXT_RESET=$'\033[0m'
        TXT_BOLD=$'\033[1m'
        TXT_DIM=$'\033[2m'
        TXT_UNDERLINE=$'\033[4m'

        TXT_FG_BLACK=$'\033[30m'
        TXT_BG_BLACK=$'\033[40m'
        TXT_FG_RED=$'\033[31m'
        TXT_BG_RED=$'\033[41m'
        TXT_FG_GREEN=$'\033[32m'
        TXT_BG_GREEN=$'\033[42m'
        TXT_FG_YELLOW=$'\033[33m'
        TXT_BG_YELLOW=$'\033[43m'
        TXT_FG_BLUE=$'\033[34m'
        TXT_BG_BLUE=$'\033[44m'
        TXT_FG_MAGENTA=$'\033[35m'
        TXT_BG_MAGENTA=$'\033[45m'
        TXT_FG_CYAN=$'\033[36m'
        TXT_BG_CYAN=$'\033[46m'
        TXT_FG_WHITE=$'\033[37m'
        TXT_BG_WHITE=$'\033[47m'

        TXT_FG_GREY=$'\033[90m'
        TXT_BG_GREY=$'\033[100m'
        TXT_FG_BRIGHT_RED=$'\033[91m'
        TXT_BG_BRIGHT_RED=$'\033[101m'
        TXT_FG_BRIGHT_GREEN=$'\033[92m'
        TXT_BG_BRIGHT_GREEN=$'\033[102m'
        TXT_FG_BRIGHT_YELLOW=$'\033[93m'
        TXT_BG_BRIGHT_YELLOW=$'\033[103m'
        TXT_FG_BRIGHT_BLUE=$'\033[94m'
        TXT_BG_BRIGHT_BLUE=$'\033[104m'
        TXT_FG_BRIGHT_MAGENTA=$'\033[95m'
        TXT_BG_BRIGHT_MAGENTA=$'\033[105m'
        TXT_FG_BRIGHT_CYAN=$'\033[96m'
        TXT_BG_BRIGHT_CYAN=$'\033[106m'
        TXT_FG_BRIGHT_WHITE=$'\033[97m'
        TXT_BG_BRIGHT_WHITE=$'\033[107m'

        TXT_COLOUR_MODE="ansi16"
    fi
fi

# For US spelling
TXT_FG_GRAY=${TXT_FG_GREY}
TXT_BG_GRAY=${TXT_BG_GREY}

# A sensible default foreground (if not already set)
: "${TXT_FG:=${TXT_FG_GREY}}"

# Helper functions (auto-reset the text style and colour)
echo() {
    # Don't add color codes in places where we shouldn't (e.g. when redirecting output)
    local pre="" post=""

    if [ -t 1 ] && [ -z "${NO_COLOR:-}" ]; then
        pre=${TXT_FG-}
        post=${TXT_RESET-}
    fi

    case $1 in
        -n*)
            shift
            printf '%s%s%s' "${pre}" "$*" "${post}"
            ;;
        -e*|-E*)
            # Defend against unsupported CLI switches for echo. If someone uses these and needs them, they
            # should instead call "command echo".
            command echo "Encountered an 'echo' with switches..."
            command echo "  echo '$*'"
            exit 1
            ;;
        *)
            printf '%s%s%s\n' "${pre}" "$*" "${post}"
            ;;
    esac
}

heading() { printf '%s%s%s\n' "${TXT_BOLD}${TXT_FG_BRIGHT_WHITE}" "$*" "${TXT_RESET}"; }
print()   { printf '%s%s'     "${TXT_FG}" "$*"; printf '%s' "${TXT_RESET}"; }
warn()    { printf '%sWARNING:%s %s%s\n' "${TXT_FG_BRIGHT_YELLOW}" "${TXT_FG_YELLOW}" "$*" "${TXT_RESET}"; }
err()     { printf '%sERROR:%s %s%s\n'   "${TXT_FG_BRIGHT_RED}"    "${TXT_FG_RED}"    "$*" "${TXT_RESET}" >&2; }
