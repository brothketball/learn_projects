cmd_/home/pi/Gits/learn_projects/09_platform_driver/device/Module.symvers := sed 's/ko$$/o/' /home/pi/Gits/learn_projects/09_platform_driver/device/modules.order | scripts/mod/modpost -m -a   -o /home/pi/Gits/learn_projects/09_platform_driver/device/Module.symvers -e -i Module.symvers   -T -