obj-m += main.o

all: module dt
	echo Builded Device Tree Overlay and kernel module

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
dt: testoverlay.dts
	dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf testoverlay.dtbo
	
file := main

start:
	sudo dtoverlay testoverlay.dtbo
	ls -l /proc/device-tree/my_device/
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10

end: clean
	sudo rmmod $(file).ko
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)
