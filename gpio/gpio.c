#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <asm/io.h>

#define GPIO_BASE      	0x3F200000

#define BLOCK_SIZE 	0xB0
#define GPFSEL0    	0x00
#define GPFSEL1    	0x04
#define GPFSEL2    	0x08
#define GPSET0     	0x1C
#define GPSET1		0x20
#define GPCLR0     	0x28
#define GPCLR1		0x2C

#define GPPUD		0x94
#define GPPUDCLK0	0x98
#define GPPUDCLK1	0x9C


volatile uint32_t *gpio;

static int __init module_start(void){
	pr_info(": start\n");
	gpio = ioremap(GPIO_BASE, 0xB0);
	if (gpio == NULL) {
		pr_info("io remap failed\n");
		iounmap(gpio);
		return -1;
	}
	// config gpio 6
	gpio[GPFSEL0/4] &=~ (7 << 18);
	gpio[GPFSEL0/4] |= (1 << 18);

	// config pull up
	// gpio[GPPUD/4] &=~ (3<<12);
	// gpio[GPPUD/4] |= (2<<12);
	// gpio[GPPUDCLK0/4] |= (1<<6);
	for(int i = 0; i < 5; i++){
		gpio[GPSET0/4] |= (1 << 6);
		pr_info("LED ON\n");
		mdelay(1000);

		gpio[GPCLR0/4] |= (1 << 6);
		pr_info("LED OFF\n");
		mdelay(1000);
	}
	return 0;
}

static void __exit module_end(void){
	iounmap(gpio);
	pr_info(": end\n");
}

module_init(module_start);
module_exit(module_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("0:0.2");
