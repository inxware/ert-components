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
# This submakefile can be included by other makefiles to add some useful variables and aliases
# related to outputting with colour and text styles on the console.

# MAKE_TERMOUT/MAKE_TERMERR are set by GNU Make at parse time.
ifeq ($(NO_COLOR),)
    ifdef MAKE_TERMOUT
        __CAN_COLOUR := yes
    else
        __CAN_COLOUR := no
    endif
else
    __CAN_COLOUR := no
endif

# Compute ESC once. Everything else is pure Make string concatenation.
ESC := $(shell printf '\033')
CSI := $(ESC)[

# Helper to build SGR: $(call sgr,92) -> ESC[92m
sgr = $(CSI)$1m

# Default to empty strings
TXT_RESET :=
TXT_BOLD :=
TXT_DIM :=
TXT_UNDERLINE :=
TXT_FG_BLACK :=
TXT_BG_BLACK :=
TXT_FG_RED :=
TXT_BG_RED :=
TXT_FG_GREEN :=
TXT_BG_GREEN :=
TXT_FG_YELLOW :=
TXT_BG_YELLOW :=
TXT_FG_BLUE :=
TXT_BG_BLUE :=
TXT_FG_MAGENTA :=
TXT_BG_MAGENTA :=
TXT_FG_CYAN :=
TXT_BG_CYAN :=
TXT_FG_WHITE :=
TXT_BG_WHITE :=
TXT_FG_GREY :=
TXT_BG_GREY :=
TXT_FG_BRIGHT_RED :=
TXT_BG_BRIGHT_RED :=
TXT_FG_BRIGHT_GREEN :=
TXT_BG_BRIGHT_GREEN :=
TXT_FG_BRIGHT_YELLOW :=
TXT_BG_BRIGHT_YELLOW :=
TXT_FG_BRIGHT_BLUE :=
TXT_BG_BRIGHT_BLUE :=
TXT_FG_BRIGHT_MAGENTA :=
TXT_BG_BRIGHT_MAGENTA :=
TXT_FG_BRIGHT_CYAN :=
TXT_BG_BRIGHT_CYAN :=
TXT_FG_BRIGHT_WHITE :=
TXT_BG_BRIGHT_WHITE :=

TXT_COLOUR_MODE := none

# If colour is allowed, prefer tput where available; otherwise use ANSI 16.
ifeq ($(__CAN_COLOUR),yes)
    __HAVE_TPUT := $(shell command -v tput >/dev/null 2>&1 && echo yes || echo no)
    ifeq ($(__HAVE_TPUT),yes)
        __TPUT_COLORS := $(shell tput colors 2>/dev/null || echo 0)
        ifneq ($(__TPUT_COLORS),0)
            TXT_COLOUR_MODE := tput8

            # Attributes via tput
            TXT_BOLD      := $(shell tput bold 2>/dev/null || printf '')
            TXT_DIM       := $(shell tput dim 2>/dev/null || printf '')
            TXT_UNDERLINE := $(shell tput smul 2>/dev/null || printf '')
            TXT_RESET     := $(shell tput sgr0 2>/dev/null || printf '')

            # Core 8 via tput
            TXT_FG_BLACK   := $(shell tput setaf 0)
            TXT_BG_BLACK   := $(shell tput setab 0)
            TXT_FG_RED     := $(shell tput setaf 1)
            TXT_BG_RED     := $(shell tput setab 1)
            TXT_FG_GREEN   := $(shell tput setaf 2)
            TXT_BG_GREEN   := $(shell tput setab 2)
            TXT_FG_YELLOW  := $(shell tput setaf 3)
            TXT_BG_YELLOW  := $(shell tput setab 3)
            TXT_FG_BLUE    := $(shell tput setaf 4)
            TXT_BG_BLUE    := $(shell tput setab 4)
            TXT_FG_MAGENTA := $(shell tput setaf 5)
            TXT_BG_MAGENTA := $(shell tput setab 5)
            TXT_FG_CYAN    := $(shell tput setaf 6)
            TXT_BG_CYAN    := $(shell tput setab 6)
            TXT_FG_WHITE   := $(shell tput setaf 7)
            TXT_BG_WHITE   := $(shell tput setab 7)

            # Probe whether bright indices work fall back to ANSI if not
            __HAS_TPUT16 := $(shell tput setaf 9 >/dev/null 2>&1 && echo yes || echo no)
            ifeq ($(__HAS_TPUT16),yes)
                TXT_FG_GREY           := $(shell tput setaf 8)
                TXT_BG_GREY           := $(shell tput setab 8)
                TXT_FG_BRIGHT_RED     := $(shell tput setaf 9)
                TXT_BG_BRIGHT_RED     := $(shell tput setab 9)
                TXT_FG_BRIGHT_GREEN   := $(shell tput setaf 10)
                TXT_BG_BRIGHT_GREEN   := $(shell tput setab 10)
                TXT_FG_BRIGHT_YELLOW  := $(shell tput setaf 11)
                TXT_BG_BRIGHT_YELLOW  := $(shell tput setab 11)
                TXT_FG_BRIGHT_BLUE    := $(shell tput setaf 12)
                TXT_BG_BRIGHT_BLUE    := $(shell tput setab 12)
                TXT_FG_BRIGHT_MAGENTA := $(shell tput setaf 13)
                TXT_BG_BRIGHT_MAGENTA := $(shell tput setab 13)
                TXT_FG_BRIGHT_CYAN    := $(shell tput setaf 14)
                TXT_BG_BRIGHT_CYAN    := $(shell tput setab 14)
                TXT_FG_BRIGHT_WHITE   := $(shell tput setaf 15)
                TXT_BG_BRIGHT_WHITE   := $(shell tput setab 15)

                TXT_COLOUR_MODE := tput16
            else
                # Use ANSI bright codes without more $(shell) calls
                TXT_FG_GREY           := $(call sgr,90)
                TXT_BG_GREY           := $(call sgr,100)
                TXT_FG_BRIGHT_RED     := $(call sgr,91)
                TXT_BG_BRIGHT_RED     := $(call sgr,101)
                TXT_FG_BRIGHT_GREEN   := $(call sgr,92)
                TXT_BG_BRIGHT_GREEN   := $(call sgr,102)
                TXT_FG_BRIGHT_YELLOW  := $(call sgr,93)
                TXT_BG_BRIGHT_YELLOW  := $(call sgr,103)
                TXT_FG_BRIGHT_BLUE    := $(call sgr,94)
                TXT_BG_BRIGHT_BLUE    := $(call sgr,104)
                TXT_FG_BRIGHT_MAGENTA := $(call sgr,95)
                TXT_BG_BRIGHT_MAGENTA := $(call sgr,105)
                TXT_FG_BRIGHT_CYAN    := $(call sgr,96)
                TXT_BG_BRIGHT_CYAN    := $(call sgr,106)
                TXT_FG_BRIGHT_WHITE   := $(call sgr,97)
                TXT_BG_BRIGHT_WHITE   := $(call sgr,107)

                TXT_COLOUR_MODE := tput8+ansi16
            endif
        endif
    endif

    # If tput was not usable, fall back to ANSI 16 using Make-only strings
    ifeq ($(TXT_COLOUR_MODE),none)
        TXT_RESET     := $(call sgr,0)
        TXT_BOLD      := $(call sgr,1)
        TXT_DIM       := $(call sgr,2)
        TXT_UNDERLINE := $(call sgr,4)

        TXT_FG_BLACK   := $(call sgr,30)
        TXT_BG_BLACK   := $(call sgr,40)
        TXT_FG_RED     := $(call sgr,31)
        TXT_BG_RED     := $(call sgr,41)
        TXT_FG_GREEN   := $(call sgr,32)
        TXT_BG_GREEN   := $(call sgr,42)
        TXT_FG_YELLOW  := $(call sgr,33)
        TXT_BG_YELLOW  := $(call sgr,43)
        TXT_FG_BLUE    := $(call sgr,34)
        TXT_BG_BLUE    := $(call sgr,44)
        TXT_FG_MAGENTA := $(call sgr,35)
        TXT_BG_MAGENTA := $(call sgr,45)
        TXT_FG_CYAN    := $(call sgr,36)
        TXT_BG_CYAN    := $(call sgr,46)
        TXT_FG_WHITE   := $(call sgr,37)
        TXT_BG_WHITE   := $(call sgr,47)
        TXT_FG_GREY    := $(call sgr,90)
        TXT_BG_GREY    := $(call sgr,100)

        TXT_FG_BRIGHT_RED     := $(call sgr,91)
        TXT_BG_BRIGHT_RED     := $(call sgr,101)
        TXT_FG_BRIGHT_GREEN   := $(call sgr,92)
        TXT_BG_BRIGHT_GREEN   := $(call sgr,102)
        TXT_FG_BRIGHT_YELLOW  := $(call sgr,93)
        TXT_BG_BRIGHT_YELLOW  := $(call sgr,103)
        TXT_FG_BRIGHT_BLUE    := $(call sgr,94)
        TXT_BG_BRIGHT_BLUE    := $(call sgr,104)
        TXT_FG_BRIGHT_MAGENTA := $(call sgr,95)
        TXT_BG_BRIGHT_MAGENTA := $(call sgr,105)
        TXT_FG_BRIGHT_CYAN    := $(call sgr,96)
        TXT_BG_BRIGHT_CYAN    := $(call sgr,106)
        TXT_FG_BRIGHT_WHITE   := $(call sgr,97)
        TXT_BG_BRIGHT_WHITE   := $(call sgr,107)

        TXT_COLOUR_MODE := ansi16
    endif
endif

# For the US spelling
TXT_FG_GRAY := $(TXT_FG_GREY)
TXT_BG_GRAY := $(TXT_BG_GREY)

# Our 'default' text foreground colour
TXT_FG = $(TXT_FG_GREY)

# $(warning [inx] __CAN_COLOUR=$(__CAN_COLOUR))
# $(warning [inx] __HAVE_TPUT=$(__HAVE_TPUT))
# $(warning [inx] __TPUT_COLORS=$(__TPUT_COLORS))
# $(warning [inx] __HAS_TPUT16=$(__HAS_TPUT16))
# $(warning [inx] TXT_COLOUR_MODE=$(TXT_COLOUR_MODE))

# Replace the standard 'echo' with an alias that allows colourful output
ECHO = printf '$(TXT_FG)%s$(TXT_RESET)\n'

# Equivalent to 'echo -n' that 
ECHO_N = printf '$(TXT_FG)%s'

# Output a heading (bold and bright white)
HEADING = printf '$(TXT_FG_BRIGHT_WHITE)$(TXT_BOLD)%s$(TXT_RESET)\n'

# As above but for a warning
WARN = printf '$(TXT_FG_BRIGHT_YELLOW)WARNING: $(TXT_FG_YELLOW)%s$(TXT_RESET)\n'

# As above but for an error
ERR = printf '$(TXT_FG_BRIGHT_RED)ERROR: $(TXT_FG_RED)%s$(TXT_RESET)\n'
