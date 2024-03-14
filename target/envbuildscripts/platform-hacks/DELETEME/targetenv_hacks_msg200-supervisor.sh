#!/bin/bash
echo "* Deploying to Debian 11 with ambifier.com certs"

#todo - ADD THE SUPERVISOR APP HERE.
export EHS_DEFAULT_APP=../apps/customer-apps/Moodsonic/MSG200-supervisor-v1/	
#export EHS_DEFAULT_APP="NONE"

./target/envbuildscripts/targetenv_hacks_ambifier2.sh