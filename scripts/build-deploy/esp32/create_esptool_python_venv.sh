#!/bin/bash

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

REPO_BASE=$SCRIPT_DIR/../../../


cd /opt #mkdir if not
sudo python3.10 -m venv python_env || echo "You need to install python2-venv (e.g. apt install python3-venv) for this to work" || exit 1

sudo chmod -R a+rw python_env

# You need to run as real root not sudo user:
#sudo -i
source ./python_env/bin/activate
pip install -r  ${REPO_BASE}/../ert-contrib-middleware/inx_build_scripts/source-scripts/python-pip-requirements_inx-xbuilder-source-me-espidf.txt
