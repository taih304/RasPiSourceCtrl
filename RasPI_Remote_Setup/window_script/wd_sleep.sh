#!/bin/bash

SLEEP_DELAY=$1

if [[ -z $SLEEP_DELAY ]]; then
    SLEEP_DELAY=5
else
    if [[ $SLEEP_DELAY ]] && [ $SLEEP_DELAY -eq $SLEEP_DELAY 2>/dev/null ]; then
        echo "Commencing sleep in $SLEEP_DELAY seconds" 
    else
        echo "$SLEEP_DELAY is not an integer or not defined"
        exit 1
    fi
fi

WD_IP=$(cat wd_ssh.conf | jq -r .WD_IP)
MACHINE_NAME=$(cat wd_ssh.conf | jq -r .MACHINE_NAME)
USER_NAME=$(cat wd_ssh.conf | jq -r .USER_NAME)
PASSWD=$(cat wd_ssh.conf | jq -r .PASSWD)

sshpass -p $PASSWD ssh -tt -o StrictHostKeyChecking=no $MACHINE_NAME/$USER_NAME@$WD_IP "C:\Windows\System32\psshutdown\psshutdown.exe -d -t $SLEEP_DELAY"
