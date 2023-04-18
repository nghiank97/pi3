
file := interrupt

obj-m += $(file).o
KDIR = /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

start:
	sudo insmod $(file).ko
	dmesg | tail -n 10

end:
	sudo rmmod $(file)
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)

