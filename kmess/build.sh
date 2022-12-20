#!/bin/bash

set -e

sudo gcc -g kmess.c kmess_cli_handler.c -o kmess
sudo chmod +x kmess
sudo cp kmess /usr/local/bin

echo "Done installation. type kmess to run"