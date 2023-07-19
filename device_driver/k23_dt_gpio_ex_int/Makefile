obj-m += main.o

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
dt: testoverlay.dts
	dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf testoverlay.dtbo
	
file := main

start: module dt
	sudo dtoverlay testoverlay.dtbo
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	dmesg | tail -n 10

end: clean
	dmesg | tail -n 10
	sudo rmmod $(file).ko

log:
	dmesg | tail -n $(len)

show:
	sudo cat /proc/device-tree/foo_device/led-gpios

on_led:
	echo 1 > /proc/my-led
 
off_led:
	echo 0 > /proc/my-led
