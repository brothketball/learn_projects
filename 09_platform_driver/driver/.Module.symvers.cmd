cmd_/home/pi/Gits/learn_projects/09_platform_driver/driver/Module.symvers := sed 's/ko$$/o/' /home/pi/Gits/learn_projects/09_platform_driver/driver/modules.order | scripts/mod/modpost -m -a   -o /home/pi/Gits/learn_projects/09_platform_driver/driver/Module.symvers -e -i Module.symvers   -T -
