#!/bin/bash
set -e
# https://stackoverflow.com/a/67610397/13406850 - CC BY-SA 4.0
git init
git remote add origin $(cat remote_url)
git fetch
git branch master origin/master
git reset HEAD -- .
