cmd_/home/pi/myspace/device_file/gpio/modules.order := {   echo /home/pi/myspace/device_file/gpio/gpio.ko; :; } | awk '!x[$$0]++' - > /home/pi/myspace/device_file/gpio/modules.order
