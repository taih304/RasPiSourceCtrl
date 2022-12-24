#!/bin/bash


# sleep 20 second to make sure everything is up
sleep 20
HOSTNAME_FILE_LOCATION="/home/pi/workspace/init_script/bootup/e2c_host_name.txt"
KEY_PAIR_LOCATION="/home/pi/workspace/init_script/bootup/vm_keypair.pem"

while true
do
    # echo "$HOSTNAME_FILE_LOCATION" >> /home/pi/workspace/init_script/test.txt
    E2C_HOSTNAME=$(cat $HOSTNAME_FILE_LOCATION)
    ssh -i $KEY_PAIR_LOCATION -tt -o StrictHostKeyChecking=no -R 2222:localhost:22 ubuntu@$E2C_HOSTNAME
    sleep 5
done