
file := timer

obj-m += $(file).o
KDIR = /lib/modules/$(shell uname -r)/build

module:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean

start: module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	sudo dmesg | tail -n 10

end: clean
	sudo rmmod $(file)
	sudo dmesg | tail -n 5

log:
	sudo dmesg | tail -n $(len)
