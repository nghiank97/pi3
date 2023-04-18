
obj-m += sysfs.o
KDIR = /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

start:
	sudo insmod sysfs.ko
	dmesg | tail -n 10

end:
	sudo rmmod sysfs
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)

list_sysfs:
	ls -l /sys/kernel/
