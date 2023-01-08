#!/bin/bash

ACTION=$1

if [[ $ACTION == "enable" ]]; then
    mosquitto_pub -p 1883 -h broker.emqx.io -t "OOyhCjXUpjH9LE" -i RTTqxGIe0xkKAd -m 'Jd3RK1VllBZatc1'
elif [[ $ACTION == "disable" ]]; then
    mosquitto_pub -p 1883 -h broker.emqx.io -t "OOyhCjXUpjH9LE" -i RTTqxGIe0xkKAd -m 'Jd3RK1VllBZatc0'
else
    echo "Unknown action: $ACTION"
fi

