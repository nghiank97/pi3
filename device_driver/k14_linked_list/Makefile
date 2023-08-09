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
	sudo chmod 777 /dev/etx_device
	sudo echo 1 > /dev/etx_device

read:
	sudo chmod 777 /dev/etx_device
	sudo cat /dev/etx_device

test:
	gcc -Wall test.c -o test.exe
	./test.exe