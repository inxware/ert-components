#This is the original server running without any connection on Etlisat Network)
# It has 50 + 25 devices as of August 2023.
# This server is using self signed inx certificates

DEVMAN_SERVER_DOMAIN=adnocdevman.inx-systems.net
DEVMAN_SERVER_PROTOCOL=https

#Server config & credentials for uplading OTA updates
DEVMAN_UNAME=vagrant
#This is what the funny uploader scripts call the apps
#todo this should be a specific app 
EHS_PRODUCT_NAME=player
DEVMAN_SERVER_NAME=adnoc-green
SSHPORT=8822

EHS_DEVMAN_SUPERVISOR_REQUIRED=yes