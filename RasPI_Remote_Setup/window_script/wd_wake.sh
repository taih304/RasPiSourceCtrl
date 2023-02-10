#!/bin/bash

MAC_ADDR=$(cat wd_ssh.conf | jq -r .MAC_ADDR)
echo a | sudo -s etherwake -i eth0 $MAC_ADDR
