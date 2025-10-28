# ESP32 flashing and debugging tools

Debugging tools are required for building flashing directly from a linux PC (i.e. has USB/tty)
This environment is created in /opt/python venv.

For WSL machines, where you want to flash directly from the PC you can either use the web tool from espressif or enable uor USB in in WSL (see below)

## Dependencies
These tools usually run using a Docker Environment called inxware/esp32s3_ubuntu22.04-build-essential, however for falshing native dependency supp
### Installation Dependencies
- Python Envvironment (This venv by Espressif - installed with pip) ../ert-contrib-middleware/contrib/esp-idf/esp-idf-5.1/

## Creating the venv on local Linux machine

Note: You need to install `python2-venv` (e.g. `apt install python3-venv`) for this to work

### Create the esp32 venv directory:
# NOTE: THIS METHOD MUST BE DONE USING PYTHON3.10 - IT DOES NOT WORK WITH PYTHON3.12 -SEE BELOW FOR INSTALL PYTHON3.10
```bash
cd /opt #mkdir if not
sudo python3 -m venv python_env
sudo chmod -R a+rw python_env

# You need to run as real root not sudo user:
#sudo -i
source ./python_env/bin/activate
pip install -r  <TOUR REPO PATH>/ert-contrib-middleware/inx_build_scripts/source-scripts/python-pip-requirements_inx-xbuilder-source-me-espidf.txt

# For flashing there are some missing packages that should be installed manually in the venv prompt:
# You may have missed some pacjages so use things like this in the penv environemt:

pip install esptool
pip install littlefs-python>=0.9.1

# To leave the python venv and go back to your prompt use
deactivate
```

This script can also be run from using
```bash
./scripts/build-deploy/wsp32/create_esptool_python_venv.sh
```

# Maintianing a live pythen venv packages
```bash
source /opt/python_env/bin/activate
```
# Installing Python 3.10
```bash
sudo apt update
sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt install python3.10 
./scripts/build-deploy/esp32/create_esptool_python_venv.sh 
```
# WSL USB Support


 
  echo "https://github.com/dorssel/usbipd-win"

  In Power shell run the following
  
```powershell
  usbipd list
  usbipd bind --busid=2-2
  usbipd attach --wsl --busid=2-2
```

  In WSL you will need to type the following to attach to the new USB device, each time the device re-connects.
```bash
 usbipd.exe attach --wsl --busid=2-2
```

# ESP32 consoles

WARNING- Currently the ESP32 console does not support buffering commands on the device and consoles with "line editing" capabilities are required for configuring devices (e.g. wifi config)
A putty session config profile is provided for this, but you can also use the web tool https://espressif.github.io/esptool-js/
