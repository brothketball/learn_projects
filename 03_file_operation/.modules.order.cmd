cmd_/home/pi/Gits/learn_projects/file_operation/modules.order := {   echo /home/pi/Gits/learn_projects/file_operation/file_operation.ko; :; } | awk '!x[$$0]++' - > /home/pi/Gits/learn_projects/file_operation/modules.order