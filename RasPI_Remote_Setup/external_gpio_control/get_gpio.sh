#!/bin/bash

gpio_state_file="/home/$USER/.gpio_state"

if ! [[ -e $gpio_state_file ]]; then
	echo -n 255 > $gpio_state_file
fi

gpio_state=$(cat $gpio_state_file)

echo    "Pin number:  ---- 1 ---- 2 ---- 3 ---- 4 ---- 5 ---- 6 ---- 7 ---- 8 ----"
echo -n "Pin level:  "

count=0
while [[ $count != 8 ]]
do
	pin_level=$(( gpio_state >> count & 1  ))
	echo -n "      $pin_level"
	count=$((count+1))
done

echo ""

