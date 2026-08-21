
#!/bin/bash
# Script for simulating a bunch of devices (on the same IP address, but using unique ports).

IP_ADDRESS="192.168.0.11"

python3 ./iot_mdns.py 23-5673-1231 $IP_ADDRESS 8080 provisioned 16 &
python3 ./iot_mdns.py 23-5673-1232 $IP_ADDRESS 8081 provisioned 1 &
python3 ./iot_mdns.py 23-5673-1233 $IP_ADDRESS 8082 provisioned 1 &
python3 ./iot_mdns.py 23-5673-1234 $IP_ADDRESS 8083 provisioned 2 &
python3 ./iot_mdns.py 23-5673-1235 $IP_ADDRESS 8084 provisioned 16 &
python3 ./iot_mdns.py 23-5673-1236 $IP_ADDRESS 8085 provisioned 16 &





