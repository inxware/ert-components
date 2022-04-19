#!/bin/bash
#cp ./QL-500.ppd /etc/cups/ppd/
apt-get install xvfb cups lpr printer-driver-ptouch wkhtmltopdf -y
lpadmin -p QL-500 -E -v usb://Brother/QL-500?serial=F8G111700 -P /etc/cups/ppd/QL-500.ppd
#cho you prolly also ant to copy ov th QL-500.ppd ppd file to /etc/cups/ppd/
