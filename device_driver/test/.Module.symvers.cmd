cmd_/home/knghia/dev/pi3/device_driver/test/Module.symvers :=  sed 's/ko$$/o/'  /home/knghia/dev/pi3/device_driver/test/modules.order | scripts/mod/modpost -m -a    -o /home/knghia/dev/pi3/device_driver/test/Module.symvers -e -i Module.symvers -T - 
