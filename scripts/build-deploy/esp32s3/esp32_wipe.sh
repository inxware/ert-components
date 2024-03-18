#!/bin/bash
set -e



echo "HAVE YOU RUN make targetenv_esp32 yet???????"

$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.4/components/esptool_py/esptool/esptool.py erase_fash
