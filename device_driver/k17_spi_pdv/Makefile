
obj-m += main.o
KDIR = /lib/modules/$(shell uname -r)/build

module:
	make -C $(KDIR) M=$(shell pwd) modules
dt:
	dtc -@ -I dts -O dtb -o spidev_disabler.dtbo spidev_disabler.dts
	sudo dtoverlay spidev_disabler.dtbo
	sudo modprobe industrialio
	sudo dmesg | tail -n 10
	dtc -@ -I dts -O dtb -o gpio_overlay.dtbo gpio_overlay.dts
	sudo dtoverlay gpio_overlay.dtbo

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -rf spidev_disabler.dtbo gpio_overlay.dtbo

file := main

start: dt module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	sudo dmesg | tail -n 10

end: clean
	sudo rmmod $(file)
	sudo dmesg | tail -n 5

log:
	sudo dmesg | tail -n $(len)
