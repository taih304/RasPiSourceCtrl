cmd_/home/pi/myspace/kernel_module/i2c_stub/modules.order := {   echo /home/pi/myspace/kernel_module/i2c_stub/i2c_stub.ko; :; } | awk '!x[$$0]++' - > /home/pi/myspace/kernel_module/i2c_stub/modules.order
