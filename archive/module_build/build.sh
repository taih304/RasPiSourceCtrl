#!/bin/bash

set -e

target=$1
module_name=$(echo ${target} | cut -d '.' -f 1)

#perform rename
sed "1 s/module/${module_name}/" /home/pi/myspace/module_build/Makefile > Makefile

make all
