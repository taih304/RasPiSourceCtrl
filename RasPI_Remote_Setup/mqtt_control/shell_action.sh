#!/bin/bash

NETFN=$1


if [[ $NETFN == "mc" ]]; then
    ACTION=$2
    if [[ $ACTION == "on" ]]; then
        mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m '1'
    elif [[ $ACTION == "off" ]]; then
        mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m '0'
    elif [[ $ACTION == "get" ]]; then
        mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m '2'
    else
        echo "Unknown action: $ACTION"
    fi
elif [[ $NETFN == "shell" ]]; then
    ACTION=$2
    if [[ $ACTION == "enable" ]]; then
        mosquitto_pub -p 1883 -h broker.emqx.io -t "OOyhCjXUpjH9LE" -i RTTqxGIe0xkKAd -m 'Jd3RK1VllBZatc1'
    elif [[ $ACTION == "disable" ]]; then
        mosquitto_pub -p 1883 -h broker.emqx.io -t "OOyhCjXUpjH9LE" -i RTTqxGIe0xkKAd -m 'Jd3RK1VllBZatc0'
    else
        echo "Unknown state: $ACTION"
    fi
else
    echo "Unkown netFN: $NETFN"
fi



