
obj-m += main.o
KDIR = /lib/modules/$(shell uname -r)/build

module:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	sudo rm -rf /boot/overlays/pwm_led.dtbo

dt: pwm_led.dts
	dtc -@ -I dts -O dtb -o pwm_led.dtbo pwm_led.dts
	sudo cp pwm_led.dtbo /boot/overlays

file := main

start: module dt
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	sudo dmesg | tail -n 10

end: clean
	sudo rmmod $(file)
	sudo dmesg | tail -n 5

log:
	sudo dmesg | tail -n $(len)

read:
	sudo chmod 777 /dev/etx_device
	gcc -Wall read.c -o read.exe
	./read.exe
 
gui:
	python3 gui.py
