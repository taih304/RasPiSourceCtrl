cmd_/home/pi/myspace/device_file/platform_driver/modules.order := {   echo /home/pi/myspace/device_file/platform_driver/pl_driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/myspace/device_file/platform_driver/modules.order