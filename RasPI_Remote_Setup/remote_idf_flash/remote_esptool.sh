#!/bin/bash

set -e
PARAMS=$@

COM_PORT=$(echo "$PARAMS"| grep -oP "\/dev\/ttyUSB\d*")
FILE_BIN=$(echo "$PARAMS"| grep -oP "\w+\.\w+$" | cut -d "." -f1)

# echo $COM_PORT
# exit 0
SCP_PORT=22
PI_PASSWD=a
PI_USER=pi
FLASH_DV_IP=192.168.1.13
FLASH_ID=$(echo $RANDOM)
TEMP_LOG_SPACE=/tmp/remote_esp_temp_$FLASH_ID

mkdir -p $TEMP_LOG_SPACE
cp $FILE_BIN.bin $TEMP_LOG_SPACE
FLASH_COMMAND=$(echo "$PARAMS" | sed "s|$FILE_BIN\.bin|/tmp/remote_esp_temp_$FLASH_ID/$FILE_BIN.bin|g")

cat > $TEMP_LOG_SPACE/check_flash_env.sh <<- EOF
#!/bin/bash
MINICOM_SESSION_PID="\$(ps -ef | grep minicom | grep $COM_PORT | grep -v ENTER | tr -s " " | cut -d " " -f2)"
if [[ -n "\$MINICOM_SESSION_PID" ]]; then
    kill -9 "\$MINICOM_SESSION_PID"
fi
TMUX_LS="\$(tmux ls | grep $COM_PORT)"
if [[ -z "\$TMUX_LS" ]]; then
    tmux new -d -s $COM_PORT
    tmux send-keys -t $COM_PORT.0 "get_idf" ENTER "tmux wait -S $COM_PORT" ENTER
tmux wait $COM_PORT
else
tmux send-keys -t $COM_PORT.0 'if [[ -z "\$IDF_PATH" ]]; then get_idf;fi' ENTER "tmux wait -S $COM_PORT" ENTER
tmux wait $COM_PORT
fi
EOF

cat $TEMP_LOG_SPACE/check_flash_env.sh
sshpass -p $PI_PASSWD scp -r -P $SCP_PORT /tmp/remote_esp_temp_$FLASH_ID $PI_USER@$FLASH_DV_IP:/tmp/

sshpass -p $PI_PASSWD ssh -p $SCP_PORT -o ConnectTimeout=10 -tt -o StrictHostKeyChecking=no $PI_USER@$FLASH_DV_IP "chmod +x /tmp/remote_esp_temp_$FLASH_ID/check_flash_env.sh && /tmp/remote_esp_temp_$FLASH_ID/check_flash_env.sh && tmux send-keys -t $COM_PORT.0 \"esptool.py $FLASH_COMMAND\" ENTER \"tmux wait -S $COM_PORT\" ENTER && tmux wait $COM_PORT && rm -fr /tmp/remote_esp_temp_$FLASH_ID"

rm -fr /tmp/remote_esp_temp_$FLASH_ID


