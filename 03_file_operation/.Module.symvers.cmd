cmd_/home/pi/Gits/learn_projects/file_operation/Module.symvers := sed 's/ko$$/o/' /home/pi/Gits/learn_projects/file_operation/modules.order | scripts/mod/modpost -m -a   -o /home/pi/Gits/learn_projects/file_operation/Module.symvers -e -i Module.symvers   -T -