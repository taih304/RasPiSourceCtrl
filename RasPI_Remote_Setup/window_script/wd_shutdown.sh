#!/bin/bash

SHUTDOWN_DELAY=$1

if [[ -z $SHUTDOWN_DELAY ]]; then
    SHUTDOWN_DELAY=5
else
    if [[ $SHUTDOWN_DELAY ]] && [ $SHUTDOWN_DELAY -eq $SHUTDOWN_DELAY 2>/dev/null ]; then
        echo "Commencing shutdown in $SHUTDOWN_DELAY seconds" 
    else
        echo "$SHUTDOWN_DELAY is not an integer or not defined"
        exit 1
    fi
fi

WD_IP=$(cat wd_ssh.conf | jq -r .WD_IP)
MACHINE_NAME=$(cat wd_ssh.conf | jq -r .MACHINE_NAME)
USER_NAME=$(cat wd_ssh.conf | jq -r .USER_NAME)
PASSWD=$(cat wd_ssh.conf | jq -r .PASSWD)

sshpass -p $PASSWD ssh $MACHINE_NAME/$USER_NAME@$WD_IP "shutdown /s /t $SHUTDOWN_DELAY"
