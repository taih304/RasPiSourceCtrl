#!/bin/bash

set -e

target=$1
module_name=$(echo ${target} | cut -d '.' -f 1)

#perform rename
new_makefile=$()