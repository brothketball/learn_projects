cmd_/home/pi/Gits/learn_projects/chardev/modules.order := {   echo /home/pi/Gits/learn_projects/chardev/chardev.ko; :; } | awk '!x[$$0]++' - > /home/pi/Gits/learn_projects/chardev/modules.order
