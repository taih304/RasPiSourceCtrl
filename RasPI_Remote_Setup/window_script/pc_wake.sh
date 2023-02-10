#!/bin/bash

MAC_ADDR=18:C0:4D:C5:D2:81
echo a | sudo -s etherwake -i eth0 $MAC_ADDR
