#!/bin/bash

PORT=$1
TEMP_LOG_SPACE=/home/$USER/workspace/my_script/remote_idf_flash/temp.txt
PI_PASSWD=a

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
    FLASH_COMMAND=$(echo "$FLASH_COMMAND" | sed "s|(PORT)|$PORT|g")

    # get the list of all the bin files and compress it, also replace the dir in flash command
    mkdir -p build_files
    for element in $FLASH_COMMAND
    do
        if [[ $element == build* ]] && [[ $element == *\.bin ]]; then
            cp $element build_files/
            ELEMENT_NAME="${element##*/}"
            FLASH_COMMAND=$(echo $FLASH_COMMAND | sed "s|$element|/tmp/build_files/$ELEMENT_NAME|g")
        fi
    done
    tar -czvf build_files.tar.gz build_files

    sshpass -p $PI_PASSWD scp -P 2222 build_files.tar.gz pi@localhost:/tmp
    sshpass -p $PI_PASSWD ssh -p 2222 -o ConnectTimeout=10 -tt -o StrictHostKeyChecking=no pi@localhost "cd /tmp && . /home/pi/esp/esp-idf/export.sh && tar -xvf build_files.tar.gz && esptool.py $FLASH_COMMAND && rm -f build_files.tar.gz && rm -fr build_files"

    rm -fr build_files
    rm -f build_files.tar.gz

fi


