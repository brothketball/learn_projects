cmd_/home/pi/Gits/learn_projects/misc/modules.order := {   echo /home/pi/Gits/learn_projects/misc/mymisc.ko; :; } | awk '!x[$$0]++' - > /home/pi/Gits/learn_projects/misc/modules.order
