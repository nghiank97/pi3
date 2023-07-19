
obj-m += base.o

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
file := base

start: module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10

end: clean
	sudo rmmod $(file).ko
	dmesg | tail -n 10

log:
	dmesg | tail -n $(len)

read:
	sudo cat /proc/my_value
	dmesg | tail -n 2