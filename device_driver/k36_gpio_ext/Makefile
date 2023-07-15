
file := main

obj-m += $(file).o
KDIR = /lib/modules/$(shell uname -r)/build

module:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

start: module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10
	cat /proc/devices
	

end:
	sudo rmmod $(file)
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)
 
led_on:
	sudo chmod 666 /dev/my_dev
	echo 1 > /dev/my_dev
  
led_off:
	sudo chmod 666 /dev/my_dev
	echo 0 > /dev/my_dev