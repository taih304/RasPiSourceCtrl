#!/bin/bash

level_state=$1
pin_arr=$2
gpio_state_file="/home/$USER/.gpio_state"

if ! [[ -e $gpio_state_file ]]; then
	echo -n 255 > $gpio_state_file
fi

gpio_state=$(cat $gpio_state_file)
for pin in $pin_arr
do
	re='^[0-9]+$'
	if [[ $pin =~ $re ]]; then
   		if [[ $pin -gt 0 ]] && [[ $pin -lt 9 ]]; then
			if [[ $level_state == "h" ]]; then
				gpio_state=$(( gpio_state | (1 << (pin-1))  ))
			elif [[ $level_state == "l" ]]; then
				gpio_state=$(( gpio_state & (~(1 << (pin-1)))  ))
			fi
		fi
	fi	
done

# echo $gpio_state
echo -n $gpio_state > $gpio_state_file
echo a | sudo -S i2cset -y 0 0x20 $gpio_state
