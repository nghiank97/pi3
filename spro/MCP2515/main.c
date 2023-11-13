#include "main.h"

#define printd(msg) printk("\t %s: %s:%d \n", msg, __func__, __LINE__);

#define CS0_pin 	8
#define CS_H()      (gpio_set_value(CS0_pin,1))
#define CS_L()      (gpio_set_value(CS0_pin,0))

static int tft_probe(struct platform_device *pdev) {
	struct device *dev = &pdev->dev;
	struct spi_master *master;
	struct spi_board_info spi_device_info = {
		.modalias = "tft_18_in",
		.max_speed_hz = 4200000,
		.bus_num = MY_BUS_NUM,
		.chip_select = 0,
		.mode = 1,
	};
	A0_pin = gpiod_get(dev, "tft_a0", GPIOD_OUT_HIGH);
	if(IS_ERR(A0_pin)) {
		printd("ERROR");
		return -1;
	}
	RST_pin = gpiod_get(dev, "tft_rst", GPIOD_OUT_HIGH);
	if(IS_ERR(RST_pin)) {
		printd("ERROR");
		return -1;
	}
	/* Get access to spi bus */
	master = spi_busnum_to_master(MY_BUS_NUM);
	/* Check if we could get the master */
	if(!master) {
		printd("ERROR");
		return -1;
	}
	/* Create new SPI device */
	tft_18_dev = spi_new_device(master, &spi_device_info);
	if(!tft_18_dev) {
		printd("ERROR");
		return -1;
	}
	tft_18_dev->bits_per_word = 8;
	/* Setup the bus for device's parameters */
	if(spi_setup(tft_18_dev) != 0){
		printd("ERROR");
		spi_unregister_device(tft_18_dev);
		return -1;
	}
    if (tft18_init() < 0){
        return -1;
    }
	tft18_clear(0xFF00);
	tft18_put_str5x7(128/2,160/2, "nghia@123", 0xFA83);
	printd("INIT");
	return 0;
}

static int tft_remove(struct platform_device *pdev) {
	tft18_clear(0x0000);
	gpiod_put(A0_pin);
	gpiod_put(RST_pin);
	if(tft_18_dev){
		spi_unregister_device(tft_18_dev);
    }
	printd("EXIT");
	return 0;
}

static struct of_device_id tft_driver_ids[] = {
	{
		.compatible = "tft,gpio",
	}, { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, tft_driver_ids);

static struct platform_driver tft_driver = {
	.probe = tft_probe,
	.remove = tft_remove,
	.driver = {
		.name = "tft_driver",
		.of_match_table = of_match_ptr(tft_driver_ids),
	},
};

module_platform_driver(tft_driver);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple tft-spi device");
