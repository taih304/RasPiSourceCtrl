#!/bin/bash

# sleep 20 second to make sure everything is up
HOSTNAME_FILE_LOCATION="/home/pi/workspace/init_script/bootup/e2c_host_name.txt"
KEY_PAIR_LOCATION="/home/pi/workspace/init_script/bootup/sing-key.pem"
E2C_HOSTNAME=$(cat $HOSTNAME_FILE_LOCATION)

ssh -i $KEY_PAIR_LOCATION -tt -o ConnectTimeout=10 -o StrictHostKeyChecking=no -R 2222:localhost:22 ubuntu@$E2C_HOSTNAME