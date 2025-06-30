#!/bin/bash
set +e
set +o pipefail

apt-get -y --allow-unauthenticated update ||:
#apt-get -y install libgstreamer0.10-0 gstreamer0.10-plugins-base gstreamer0.10-plugins-ugly &>/dev/null ||:
#apt-get -y install gstreamer0.10-plugins-good &> /dev/null ||:
apt-get -y --allow-unauthenticated install gstreamer0.10 libgtk2.0-0||:

apt-get install -y --allow-unauthenticated libidn11
#apt-get install -y --allow-unauthenticated libxcomposite1
#apt-get install -y --allow-unauthenticated libpangocairo-1.0-0
apt-get install -y --allow-unauthenticated libgtk2.0-0
apt-get install -y --allow-unauthenticated libgstreamer0.10-0
#apt-get install -y --allow-unauthenticated gstreamer0.10
apt-get install -y --allow-unauthenticated libgstreamer-plugins-base0.10-0

