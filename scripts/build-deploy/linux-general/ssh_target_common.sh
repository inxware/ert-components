#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
#
# ssh_target_common.sh - shared SSH destination parsing for the linux-general
# deploy scripts.  Sourced by deployViaSsh.sh and upload_local_deb_install.sh so
# the two accept exactly the same syntax and cannot drift apart.
#
# Both accepted forms, in both scripts:
#
#     user@host          one argument, user embedded in the address
#     user host          two arguments, user FIRST (the historic deb-script form)
#     host               one argument, no '@' - only valid when the calling
#                        script sets EHS_SSH_DEFAULT_USER (deployViaSsh.sh
#                        sets it to root; the deb script does not, so it
#                        requires an explicit user).
#
# NOT sourced from anywhere else - keep it dependency-free (no TARGET.cfg, no
# repo-root assumption) so either script can be run from wherever.

# ehs_resolve_ssh_target <arg1> <arg2>
#
# Returns 0 on success and sets:
#     EHS_SSH_USER   user part
#     EHS_SSH_HOST   host part
#     EHS_SSH_DEST   "user@host", ready to hand to ssh / scp / rsync
#     EHS_SSH_SHIFT  how many of the caller's arguments were consumed (1 or 2),
#                    so the caller can `shift ${EHS_SSH_SHIFT}` and carry on
#                    reading its own trailing arguments.
#
# Returns 1 on failure with EHS_SSH_ERROR set to a human-readable reason; the
# caller decides how to report it (each script shows its own usage text).
ehs_resolve_ssh_target() {
	local a1="$1"
	local a2="$2"

	EHS_SSH_USER=""
	EHS_SSH_HOST=""
	EHS_SSH_DEST=""
	EHS_SSH_SHIFT=0
	EHS_SSH_ERROR=""

	if [ -z "${a1}" ]; then
		EHS_SSH_ERROR="no target given"
		return 1
	fi

	case "${a1}" in
		@*)
			EHS_SSH_ERROR="malformed address '${a1}' - no user before the '@'"
			return 1
			;;
		*@)
			EHS_SSH_ERROR="malformed address '${a1}' - no host after the '@'"
			return 1
			;;
		*@*)
			# user@host.  Split on the LAST '@', which is what ssh itself does.
			EHS_SSH_USER="${a1%@*}"
			EHS_SSH_HOST="${a1##*@}"
			EHS_SSH_SHIFT=1
			;;
		*)
			# No '@'.  Either "user host" across two arguments, or a bare host
			# that falls back to the calling script's default user.
			if [ -n "${a2}" ]; then
				EHS_SSH_USER="${a1}"
				EHS_SSH_HOST="${a2}"
				EHS_SSH_SHIFT=2
			elif [ -n "${EHS_SSH_DEFAULT_USER}" ]; then
				EHS_SSH_USER="${EHS_SSH_DEFAULT_USER}"
				EHS_SSH_HOST="${a1}"
				EHS_SSH_SHIFT=1
			else
				EHS_SSH_ERROR="no ssh user given - use 'user@${a1}' or '<user> ${a1}'"
				return 1
			fi
			;;
	esac

	EHS_SSH_DEST="${EHS_SSH_USER}@${EHS_SSH_HOST}"
	return 0
}
