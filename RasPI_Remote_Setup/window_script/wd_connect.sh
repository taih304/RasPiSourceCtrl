#!/bin/bash

WD_IP=$(cat wd_ssh.conf | jq -r .WD_IP)
MACHINE_NAME=$(cat wd_ssh.conf | jq -r .MACHINE_NAME)
USER_NAME=$(cat wd_ssh.conf | jq -r .USER_NAME)
PASSWD=$(cat wd_ssh.conf | jq -r .PASSWD)

sshpass -p $PASSWD ssh -tt -o StrictHostKeyChecking=no $MACHINE_NAME/$USER_NAME@$WD_IP
