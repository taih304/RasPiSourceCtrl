#!/bin/bash

set -e

gcc start_test.c kmess_cli_handler.c -o ut
set +e
./ut
rm -f ut
