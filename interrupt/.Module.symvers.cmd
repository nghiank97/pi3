cmd_/home/knghia/dev/driver/interrupt/Module.symvers :=  sed 's/ko$$/o/'  /home/knghia/dev/driver/interrupt/modules.order | scripts/mod/modpost -m -a    -o /home/knghia/dev/driver/interrupt/Module.symvers -e -i Module.symvers -T - 