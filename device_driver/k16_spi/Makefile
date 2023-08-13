
obj-m += main.o
KDIR = /lib/modules/$(shell uname -r)/build

module:
	make -C $(KDIR) M=$(shell pwd) modules
dt:
	dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts
	sudo dtoverlay testoverlay.dtbo
	sudo modprobe industrialio
	sudo dmesg | tail -n 10

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

# ls -l /boot/overlays/tft_1_8.dtbo