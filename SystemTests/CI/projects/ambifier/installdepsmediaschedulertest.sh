#!/bin/bash

#install latest vagrant for doing builds
curl -fsSL https://apt.releases.hashicorp.com/gpg | sudo apt-key add -
sudo apt-add-repository "deb [arch=amd64] https://apt.releases.hashicorp.com $(lsb_release -cs) main"
sudo apt-get update && sudo apt-get install vagrant
su jenkins
vagrant plugin install vagrant-vbguest
exit
sudo cp ./asound.conf /etc/
su jenkins
python3 -m pip install sounddevice --user
python3 -m pip install -U matplotlib
python3 -m pip install -U scipy