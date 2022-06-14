#!/bin/bash

cp ../../../ESP_PROJECT/ESP32/FOTA_test/build/FOTA_test.bin ./images/FOTA.bin
git add .
git commit -m "update test FOTA"
git push origin master
