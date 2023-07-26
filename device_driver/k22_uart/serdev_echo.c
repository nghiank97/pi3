#include <linux/module.h>
#include <linux/init.h>
#include <linux/serdev.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/gpio.h>

static int serdev_echo_probe(struct serdev_device *serdev);
static void serdev_echo_remove(struct serdev_device *serdev);

static struct of_device_id serdev_echo_ids[] = {
	{
		.compatible = "brightlight,echodev",
	}, { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, serdev_echo_ids);

#define LED_PIN 21
#define LED_ON()	(gpio_set_value(LED_PIN,1))
#define LED_OFF()	(gpio_set_value(LED_PIN,0))

uint8_t rx_data[256];
uint16_t rx_len = 0;


static bool comp_arr(char* src, char* dest, uint16_t len){
	for(uint16_t i=0;i<len;i++){
		if (src[i]  != dest[i]){
			return false;
		}
	}
	return true;
}

static int serdev_echo_recv(struct serdev_device *serdev, const unsigned char *buffer, size_t size) {
	uint8_t c = buffer[0];
	if (c == 10 || c == 13){
		if (comp_arr(rx_data,"led_on",6)){
			LED_ON();
		}
		else if (comp_arr(rx_data,"led_off",7)){
			LED_OFF();
		}
		else{
			serdev_device_write_buf(serdev, "incorrect form\n", 16);
		}
		rx_len = 0;
	}
	else{
		rx_data[rx_len] = c;
		rx_len+=1;
	}
    return serdev_device_write_buf(serdev, buffer, size);;
}

static const struct serdev_device_ops serdev_echo_ops = {
	.receive_buf = serdev_echo_recv,
};

static int serdev_echo_probe(struct serdev_device *serdev) {
	int status;
	serdev_device_set_client_ops(serdev, &serdev_echo_ops);
	status = serdev_device_open(serdev);
	if(status) {
		printk("	info! Can't open serial port!\n");
		return -status;
	}

	serdev_device_set_baudrate(serdev, 115200);
	serdev_device_set_flow_control(serdev, false);
	serdev_device_set_parity(serdev, SERDEV_PARITY_NONE);
	serdev_device_write_buf(serdev, "	welcome! \n", 12);
	
	//Output GPIO configuration
	//Checking the GPIO is valid or not
	if(gpio_is_valid(LED_PIN) == false){
		pr_err("    GPIO %d is not valid\n", LED_PIN);
		return -1;
	}
	if(gpio_request(LED_PIN,"LED_PIN") < 0){
		pr_err("    ERROR: GPIO %d request\n", LED_PIN);
		gpio_free(LED_PIN);
		return -1;
	}
	gpio_direction_output(LED_PIN,0);

	printk("	info! done\n");
	return 0;
}

static void serdev_echo_remove(struct serdev_device *serdev) {
	printk("	info! remove!\n");
	serdev_device_close(serdev);
}

static struct serdev_device_driver serdev_echo_driver = {
	.probe = serdev_echo_probe,
	.remove = serdev_echo_remove,
	.driver = {
		.name = "serdev-echo",
		.of_match_table = of_match_ptr(serdev_echo_ids),
	},
};

static int __init init_func(void) {
	if(serdev_device_driver_register(&serdev_echo_driver)) {
		printk("	error! could not load driver\n");
		return -1;
	}
	printk("	info! loading the driver\n");
	return 0;
}

static void __exit exit_func(void) {
	printk("	info! unload driver");
	serdev_device_driver_unregister(&serdev_echo_driver);
}
module_init(init_func);
module_exit(exit_func);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple uart port");
