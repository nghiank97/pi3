obj-m += main.o
KDIR = /lib/modules/$(shell uname -r)/build

module:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean

file := main

start: module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10

end: clean
	sudo rmmod $(file)
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)

write:
	ls -l /proc/my_value
	echo led_on > /proc/my_value

read:
	ls -l /proc/my_value
	cat /proc/my_value

