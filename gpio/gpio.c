#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");

#define LED 6

static int __init ModuleInit(void) {
	printk("Hello, Kernel!\n");

	/* GPIO 4 init */
	if(gpio_request(LED, "rpi-gpio-6")) {
		printk("Can not allocate GPIO 6\n");
		goto Gpio4Error;
	}
	/* Set GPIO 4 direction */
	if(gpio_direction_output(LED, 0)) {
		printk("Can not set GPIO 6 to output!\n");
		goto Gpio4Error;
	}
	for (int i=0;i<5;i++){
		gpio_set_value(LED, 0);
		printk("LED ON\n");
		mdelay(1000);
		gpio_set_value(LED, 1);
		printk("LED OFF\n");
		mdelay(1000);
	}
	
	
	return 0;
Gpio4Error:
	gpio_free(4);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	gpio_set_value(4, 0);
	gpio_free(4);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
