
obj-m += timer2.o
KDIR = /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

log:
	dmesg | tail -n $(len)

start:
	sudo insmod timer.ko
	dmesg | tail -n 5

end:
	sudo rmmod timer
	dmesg | tail -n 5
