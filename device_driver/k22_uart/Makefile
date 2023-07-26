obj-m += serdev_echo.o

all: module dt
	echo Builded Device Tree Overlay and kernel module

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
dt: serdev_overlay.dts
	dtc -@ -I dts -O dtb -o serdev_overlay.dtbo serdev_overlay.dts
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf serdev_overlay.dtbo
	sudo rm -rf /boot/overlays/serdev_overlay.dtbo
	
start:module dt
	sudo cp serdev_overlay.dtbo /boot/overlays
	sudo dtoverlay serdev_overlay.dtbo
	sudo insmod serdev_echo.ko

end: clean
	sudo rmmod serdev_echo.ko


log:
	dmesg | tail -n $(len)
