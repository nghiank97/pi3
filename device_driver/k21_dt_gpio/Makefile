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
	sudo ls /proc/device-tree/my_device/green-led-gpio
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10

end:
	sudo rmmod $(file)
	dmesg | tail -n 5

log:
	dmesg | tail -n $(len)


on_led:
	echo 1 > /proc/my-led
 
off_led:
	echo 0 > /proc/my-led