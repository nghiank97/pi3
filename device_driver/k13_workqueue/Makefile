
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
	sudo dmesg | tail -n 10

end: clean
	sudo rmmod $(file)
	sudo dmesg | tail -n 5

log:
	sudo dmesg | tail -n $(len)

write:
	sudo chmod 777 /dev/etx_device
	sudo echo 1 > /dev/etx_device
	sudo dmesg | tail -n 5

read:
	sudo chmod 777 /dev/etx_device
	sudo cat /dev/etx_device
	sudo dmesg | tail -n 5