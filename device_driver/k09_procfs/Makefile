
file := main
obj-m += $(file).o

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

start: module
	sudo chmod 777 $(file).ko
	sudo insmod $(file).ko
	sudo chmod 777 /proc/my_value/value
	gcc -o read.o read.c

end: clean
	sudo rmmod $(file).ko

log: 
	dmesg | tail -n $(len)

write:
	sudo echo $(data) > /proc/my_value/value

read:
	sudo ./read.o