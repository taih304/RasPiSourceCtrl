#!/bin/bash

autossh_status_line=$(systemctl --type=service | grep autossh.service)

LOAD=$(echo "$autossh_status_line" | tr -s ' ' | cut -d ' ' -f3)
ACTIVE=$(echo "$autossh_status_line" | tr -s ' ' | cut -d ' ' -f4)
SUB=$(echo "$autossh_status_line" | tr -s ' ' | cut -d ' ' -f5)
JSON_STR=$(jq --null-input --arg load $LOAD --arg active $ACTIVE --arg sub $SUB '{"load": $load, "active": $active, "sub": $sub}')

echo "$JSON_STR"