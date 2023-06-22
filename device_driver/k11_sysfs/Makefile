
obj-m += main.o
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR)M=$(shell pwd) clean

file := main

start:
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10

end:
	sudo rmmod $(file)
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)
 
show:
	sudo chmod 777 /sys/kernel/etx_sysfs/etx_value
	cat /sys/kernel/etx_sysfs/etx_value
 
write:
	echo 123 > /sys/kernel/etx_sysfs/etx_value
 
clean:
	python3 clean.py