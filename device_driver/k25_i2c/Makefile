
file := main
obj-m += $(file).o

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

dt: i2c_overlay.dts
	dtc -@ -I dts -O dtb -o i2c_overlay.dtbo i2c_overlay.dts

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf i2c_overlay.dtbo

start: module dt
	sudo insmod $(file).ko
	dmesg | tail -n 10
	sudo chmod 777 i2c_overlay.dtbo 
	sudo dtoverlay i2c_overlay.dtbo 

run:
	sudo insmod $(file).ko
	dmesg | tail -n 10

end: clean
	sudo rmmod $(file).ko

log:
	dmesg | tail -n $(len)

read:
	ls -l /proc/lsm303dlhc/value
	python3 read.py
