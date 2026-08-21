#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
#
# SCOPE — deletes a branch, locally and on the remote.
#
# Input : $INX_BRANCH_NAME (environment variable; refuses to run without it)
# Output: the branch is gone from this clone AND from origin.
#         `git branch -d` is the safe form — it refuses to delete a branch that
#         is not merged — but `git push origin :<branch>` is not, and deletes the
#         remote branch regardless. There is no confirmation prompt.

if [ -z "${INX_BRANCH_NAME}" ]; then
	echo "Please set environment variable INX_BRANCH_NAME"
	exit
fi
git branch -d ${INX_BRANCH_NAME}
git push origin :${INX_BRANCH_NAME}
