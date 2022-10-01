#!/bin/bash

# pre displayed
# dmesg | tail

current=$(dmesg | tail | tac)
sleep 0.4

while true
do
    new_block_of_lines=""
    last_line_of_current=$(head -1 <<< "$current")
    next=$(dmesg | tail | tac)
    while read -r line
    do
        if [[ "$line" != "$last_line_of_current" ]]; then
            new_block_of_lines="$line"$'\n'"$new_block_of_lines"
        else
            break
        fi
    done <<< "$next"
    if [[ ! -z "$new_block_of_lines" ]]; then
        echo "$(head -c-1 <<< "$new_block_of_lines")"
    fi
    current="$next"
    sleep 0.4
done