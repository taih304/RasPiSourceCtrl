#!/bin/bash

IS_HELP=$1

if [[ $IS_HELP == "-h" ]] || [[ $IS_HELP == "--help" ]]; then
    echo "Usage: ./shell_action [NETFN] [ACTION]"
    echo ""
    echo "NETFN:"
    echo "      mc:     esp32 bmc"
    echo "      shell:  Host shell control"
    echo ""
    echo "ACTION:"
    echo "      mc:"
    echo "          on:     Turn on relay"
    echo "          off:    Turn off relay"
    echo "          get:    Get relay status"
    echo "      shell:"
    echo "          enable:  Enable shell control"
    echo "          disable: Disable shell control"
    exit 0
fi

if [[ -n $IS_HELP ]]; then
    echo "Unknown option $IS_HELP"
    exit 1
fi

mosquitto_sub -h broker.emqx.io -t "FL4LYZOfQCVEpo" &

while true
do
    read CMD_ARG
    NETFN=$(echo "$CMD_ARG" | cut -d " " -f1)
    ACTION=$(echo "$CMD_ARG" | cut -d " " -f2)
    if [[ $NETFN == "mc" ]]; then
        if [[ $ACTION == "on" ]]; then
            mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m '1'
        elif [[ $ACTION == "off" ]]; then
            mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m '0'
        elif [[ $ACTION == "get" ]]; then
            mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m '2'
        elif [[ $ACTION == "pin" ]]; then
	    OPTION=$(echo "$CMD_ARG" | cut -d " " -f3)
	    if [[ $OPTION == "set" ]]; then
		P_MODE=$(echo "$CMD_ARG" | cut -d " " -f4)
		PIN_NUM=$(echo "$CMD_ARG" | cut -d " " -f5)
		if [[ $P_MODE == "output" ]]; then
		    mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m "3MO$PIN_NUM"
		elif [[ $P_MODE == "input" ]]; then
		    echo "something"
		fi
	    elif [[ $OPTION == "level" ]]; then
		PIN_STATE=$(echo "$CMD_ARG" | cut -d " " -f4)
                PIN_NUM=$(echo "$CMD_ARG" | cut -d " " -f5)
		if [[ $PIN_STATE == "high" ]]; then
		    mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m "3SE$PIN_NUM"
		elif [[ $PIN_STATE == "low" ]]; then
		    mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m "3SD$PIN_NUM"
		elif [[ $PIN_STATE == "pushl" ]]; then
		    mosquitto_pub -h broker.emqx.io -t "Klcgd1cSk7cvkQ" -m "3SP$PIN_NUM"
	    	else
		    echo "Unknown pin state: $PIN_STATE"
		fi
	    else
		echo "Unknown option: $OPTION"
	    fi
    	else
            echo "Unknown action: $ACTION"
        fi
    elif [[ $NETFN == "shell" ]]; then
        if [[ $ACTION == "enable" ]]; then
            mosquitto_pub -p 1883 -h broker.emqx.io -t "OOyhCjXUpjH9LE" -i RTTqxGIe0xkKAd -m 'Jd3RK1VllBZatc1'
        elif [[ $ACTION == "disable" ]]; then
            mosquitto_pub -p 1883 -h broker.emqx.io -t "OOyhCjXUpjH9LE" -i RTTqxGIe0xkKAd -m 'Jd3RK1VllBZatc0'
        else
            echo "Unknown state: $ACTION"
        fi
    elif [[ $NETFN == "exit" ]]; then
	SUB_PID=$(ps -ef | grep mosquitto_sub | grep -v "grep" | tr -s " " | cut -d " " -f2)
	kill -9 $SUB_PID
	exit 0
    else
        echo "Unkown netFN: $NETFN"
    fi
done




