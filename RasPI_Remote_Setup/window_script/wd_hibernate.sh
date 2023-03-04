#!/bin/bash

HIBERNATE_DELAY=$1

if [[ -z $HIBERNATE_DELAY ]]; then
    HIBERNATE_DELAY=5
else
    if [[ $HIBERNATE_DELAY ]] && [ $HIBERNATE_DELAY -eq $HIBERNATE_DELAY 2>/dev/null ]; then
        echo "Commencing hibernation in $HIBERNATE_DELAY seconds" 
    else
        echo "$HIBERNATE_DELAY is not an integer or not defined"
        exit 1
    fi
fi

WD_IP=$(cat wd_ssh.conf | jq -r .WD_IP)
MACHINE_NAME=$(cat wd_ssh.conf | jq -r .MACHINE_NAME)
USER_NAME=$(cat wd_ssh.conf | jq -r .USER_NAME)
PASSWD=$(cat wd_ssh.conf | jq -r .PASSWD)

sshpass -p $PASSWD ssh -tt -o StrictHostKeyChecking=no $MACHINE_NAME/$USER_NAME@$WD_IP "C:\Windows\System32\psshutdown\psshutdown.exe -h -t $HIBERNATE_DELAY"
