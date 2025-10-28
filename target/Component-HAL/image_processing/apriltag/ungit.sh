#!/bin/bash
set -e

if [ ! -d ".git" ]; then
	echo "\".git\" folder does not exist. Does Nothing. Exiting..."
	exit 1
fi

if [[ $1 = "init" ]]; then
	git rev-parse > current_commit
	git config --get remote.origin.url > remote_url
	rm -rf .git
	exit 0
elif [ -z "$1" ] && [ -f remote_url ] && [ $(git config --get remote.origin.url) = $(cat remote_url) ]; then
	git rev-parse > current_commit
	git config --get remote.origin.url > remote_url
	rm -rf .git
	exit 0
fi

echo "Does Nothing. Exiting..."
