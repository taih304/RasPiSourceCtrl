cmd_/home/pi/myspace/kernel_module/interrupt/modules.order := {   echo /home/pi/myspace/kernel_module/interrupt/it_module.ko; :; } | awk '!x[$$0]++' - > /home/pi/myspace/kernel_module/interrupt/modules.order
