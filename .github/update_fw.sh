#!/bin/bash
cp ../../../ESP_PROJECT/ESP32/hello_world1/build/hello_world1.bin ./images/${1}.bin
git add .
git commit -m "update test FOTA"
git push origin master
ls
