#!/bin/bash
set -e
if [[ -n $IDF_PATH ]]; then

COM_PORT=$1

if [[ -z $COM_PORT ]]; then
    echo "Please specify flashing port"
    exit 1
fi

# Configure session
SCP_PORT=22
PI_PASSWD=a
PI_USER=pi
FLASH_DV_IP=192.168.1.13
FLASH_ID=$(echo $RANDOM)
TEMP_LOG_SPACE=/tmp/remote_flash_temp_$FLASH_ID.txt

# start the program
idf.py build | tee $TEMP_LOG_SPACE

cat $TEMP_LOG_SPACE | grep -q -m 1 "To flash, run this command"
FLASH_RESULT=$?

if [[ $FLASH_RESULT -ne 0 ]]; then
    echo ======== Build fail ========
else

    MAIN_BLOCK=$(cat $TEMP_LOG_SPACE | grep -m 1 -A 10 "To flash, run this command")

    # process the flash command, remove first and last line
    FLASH_COMMAND=$(echo "$MAIN_BLOCK" | tail -n +2 | head -n -1)

    # unify all the line into one command
    FLASH_COMMAND=$(echo "$FLASH_COMMAND" | tr '\n' '*' | tr -d '*')

    # get only the parameter
    FLASH_COMMAND=$(echo "$FLASH_COMMAND" | grep -oP "(?<=\/esptool\.py).*")

    # replace the port
    FLASH_COMMAND=$(echo "$FLASH_COMMAND" | sed "s|(PORT)|$COM_PORT|g")

    # get the list of all the bin files and compress it, also replace the dir in flash command
    mkdir -p build_files_$FLASH_ID
    for element in $FLASH_COMMAND
    do
        if [[ $element == build* ]] && [[ $element == *\.bin ]]; then
            cp $element build_files_$FLASH_ID/
            ELEMENT_NAME="${element##*/}"
            FLASH_COMMAND=$(echo $FLASH_COMMAND | sed "s|$element|/tmp/build_files_$FLASH_ID/$ELEMENT_NAME|g")
        fi
    done

    cat > build_files_$FLASH_ID/check_flash_env.sh <<- EOF
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
    cat build_files_$FLASH_ID/check_flash_env.sh

    tar -czvf build_files_$FLASH_ID.tar.gz build_files_$FLASH_ID

    sshpass -p $PI_PASSWD scp -P $SCP_PORT build_files_$FLASH_ID.tar.gz $PI_USER@$FLASH_DV_IP:/tmp
    echo "Start flashing on $COM_PORT"
    sshpass -p $PI_PASSWD ssh -p $SCP_PORT -o ConnectTimeout=10 -tt -o StrictHostKeyChecking=no $PI_USER@$FLASH_DV_IP "cd /tmp && tar -xvf build_files_$FLASH_ID.tar.gz && chmod +x /tmp/build_files_$FLASH_ID/check_flash_env.sh && /tmp/build_files_$FLASH_ID/check_flash_env.sh && tmux send-keys -t $COM_PORT.0 \"esptool.py $FLASH_COMMAND\" ENTER \"tmux wait -S $COM_PORT\" ENTER && tmux wait $COM_PORT && rm -f build_files_$FLASH_ID.tar.gz && rm -fr build_files_$FLASH_ID && tmux send-keys -t $COM_PORT.0 \"minicom -b 115200 -D $COM_PORT\" ENTER"

    # minicom -b 115200 -D $COM_PORT

    rm -fr build_files_$FLASH_ID
    rm -f build_files_$FLASH_ID.tar.gz
    rm -f /tmp/remote_flash_temp_$FLASH_ID.txt

fi

else

echo "Unset IDF environment, please run the get_idf command"

fi


