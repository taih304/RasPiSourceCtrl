#!/bin/bash

WD_IP=$(cat wd_ssh.conf | jq -r .WD_IP)
MACHINE_NAME=$(cat wd_ssh.conf | jq -r .MACHINE_NAME)
USER_NAME=$(cat wd_ssh.conf | jq -r .USER_NAME)
PASSWD=$(cat wd_ssh.conf | jq -r .PASSWD)

# start VM
sshpass -p $PASSWD ssh -tt -o StrictHostKeyChecking=no $MACHINE_NAME/$USER_NAME@$WD_IP "VBoxManage startvm \"Ubuntu 20.04\""

sleep 20
# ssh to VM and enable autossh service in VM
sshpass -p $PASSWD ssh -tt -o StrictHostKeyChecking=no $MACHINE_NAME/$USER_NAME@$WD_IP "ssh -tt -o StrictHostKeyChecking=no purelife@127.0.1.1 \"echo a | sudo -S systemctl restart autossh.service\""

