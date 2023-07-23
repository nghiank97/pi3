
file := main
obj-m += $(file).o

module:
	make -C /lib/modul
	es/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

start: module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	sudo chmod 777 /sys/kernel/etx_sysfs/etx_value

end: clean
	sudo rmmod $(file).ko

log: 
	dmesg | tail -n $(len)

write:
	sudo echo 123 > /sys/kernel/etx_sysfs/etx_value
read:
	cat /sys/kernel/etx_sysfs/etx_value
 

test:
	gcc -o read.o read.c
	./read.o