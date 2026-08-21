#!/bin/bash
#
# SCOPE — throws away local work and makes this clone match origin/master.
#
# Input : nothing (no arguments, no prompt)
# Output: the working tree, the index and the master branch are all reset to
#         origin/master, and master is checked out. Uncommitted changes and any
#         local commits on master are DESTROYED and are not recoverable through
#         git's normal routes. Stash or push first.

git fetch origin
git reset --hard origin/master
git checkout master
