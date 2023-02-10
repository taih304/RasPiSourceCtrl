#!/bin/bash

CONF_FILE_LOCATION="/etc/my_service/.autossh.conf"

#if [[ -h "$CONF_FILE_LOCATION" ]]; then
if true; then

KEY_PAIR_LOCATION=$(cat $CONF_FILE_LOCATION | jq -r .KEY_PAIR_LOCATION)
E2C_HOSTNAME=$(cat $CONF_FILE_LOCATION | jq -r .E2C_HOSTNAME)
SSH_PORT=$(cat $CONF_FILE_LOCATION | jq -r .PORT)

ssh -i $KEY_PAIR_LOCATION -tt -o ConnectTimeout=10 -o StrictHostKeyChecking=no -R $SSH_PORT:localhost:22 ubuntu@$E2C_HOSTNAME

else
	echo "No conf file exist"
fi
