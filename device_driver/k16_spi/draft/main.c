
#include "main.h"

#define printd(msg) printk("\t %s: %s:%d \n", msg, __func__, __LINE__);

struct spi_device_id temp_spi[] = {
	{"spidev",0},
	{}
};

static struct spidev_data *spi_data;
static struct class *tft18_class;
static struct spi_message sm;

struct spi_transfer spi_tr = {
	.tx_buf = 0,
	.rx_buf = 0,
	.len = 1,
	.bits_per_word = 8,
	.speed_hz = 10000000,
	.cs_change = 1,
};

struct spidev_data {
	dev_t  devt;
	struct spi_device *spi;
};

u16 scr_width;
u16 scr_height;

u08 tft18_write(u08 data){
	int ret_val = 0;
	CS_L();	// gpio cs low
	spi_tr.tx_buf = &data;
	spi_tr.len = 1;
	spi_message_init(&sm);
	spi_message_add_tail(&spi_tr,&sm);
	ret_val=spi_sync(spi_data->spi, &sm);
	CS_H();	// gpio cs high
	return ret_val;
}

void tft18_cmd(uint8_t cmd) {
	A0_L();
	tft18_write(cmd);
}

void tft18_data(uint8_t data) {
	A0_H();
	tft18_write(data);
}

int gpio_init(void){
  if(gpio_is_valid(A0_pin) == false){
    printd("ERROR");
    return -1;
  }
  if(gpio_request(A0_pin,"A0_pin") < 0){
    printd("ERROR");
    return -1;
  }
  gpio_direction_output(A0_pin, 0);

  if(gpio_is_valid(RST_pin) == false){
    printd("ERROR");
    return -1;
  }
  if(gpio_request(RST_pin,"RST_pin") < 0){
    printd("ERROR");
    return -1;
  }
  gpio_direction_output(RST_pin, 0);

  A0_H();
  RST_H();
  return 0;
}

int tft18_init(void) {
	if (gpio_init() < 0){
		printd("ERROR");
		return -1;
	}
	// Reset display
	CS_H();
	RST_H();
	mdelay(5);
	RST_L();
	mdelay(5);
	RST_H();
	CS_H();
	mdelay(5);

	tft18_cmd(ST7735_SLPOUT);	// Sleep out & booster on
	mdelay(120);	
	tft18_cmd(ST7735_FRMCTR1);	// In normal mode (full colors):
	A0_H();
	tft18_write(0x05);	// RTNA set 1-line period: RTNA2, RTNA0
	tft18_write(0x3c);	// Front porch: FPA5,FPA4,FPA3,FPA2
	tft18_write(0x3c);	// Back porch: BPA5,BPA4,BPA3,BPA2

	tft18_cmd(0xb2);	// In idle mode (8-colors):
	A0_H();
	tft18_write(0x05); 	// RTNB set 1-line period: RTNAB, RTNB0
	tft18_write(0x3c); 	// Front porch: FPB5,FPB4,FPB3,FPB2
	tft18_write(0x3c); 	// Back porch: BPB5,BPB4,BPB3,BPB2

	tft18_cmd(0xb3);   	// In partial mode + full colors:
	A0_H();
	tft18_write(0x05); 	// RTNC set 1-line period: RTNC2, RTNC0
	tft18_write(0x3c); 	// Front porch: FPC5,FPC4,FPC3,FPC2
	tft18_write(0x3c); 	// Back porch: BPC5,BPC4,BPC3,BPC2
	tft18_write(0x05); 	// RTND set 1-line period: RTND2, RTND0
	tft18_write(0x3c); 	// Front porch: FPD5,FPD4,FPD3,FPD2
	tft18_write(0x3c); 	// Back porch: BPD5,BPD4,BPD3,BPD2

	tft18_cmd(0xB4);   	// Display dot inversion control:
	tft18_data(0x03);  	// NLB,NLC

	tft18_cmd(0x3a);   	// Interface pixel format
	tft18_data(0x05);  	// 16-bit/pixel RGB 5-6-5 (65k colors)
	tft18_cmd(0x20);   	// Display inversion off

	tft18_cmd(0x13);   	// Partial mode off

	tft18_cmd(0x26);   	// Gamma curve set:
	tft18_data(0x01);  	// Gamma curve 1 (G2.2) or (G1.0)
	tft18_cmd(0x29);   	// Display on
	return 0;
}

static int tft18_f_open(struct inode *inode, struct file *filp) {
	return 0;
}

static int tft18_f_release(struct inode *i,struct file *filp){
	return 0;
}

static ssize_t tft18_f_write(struct file *f,const char *tft18, size_t sz, loff_t *loffptr){
	return sz; 
}

static const struct file_operations tft18_fops = {
	.open = tft18_f_open,
	.release = tft18_f_release,
	.write	= tft18_f_write,
};

static int tft18_probe(struct spi_device *spi)
{                                           
	int status = 0;
	struct device *dev;                                                      
	spi_data = kzalloc(sizeof(*spi_data), GFP_KERNEL);
	if(!spi_data){
		return -ENOMEM;
    }
	spi_data->spi = spi;
	spi_data->devt = MKDEV(MAJORNO, MINORNO);
	dev = device_create(tft18_class, &spi->dev, spi_data->devt, spi_data, DEVICENAME);
	if(IS_ERR(dev))
	{
		printd("ERROR");
		kfree(spi_data);
		return -1;
	}
	printk("\t SPI LED Driver Probed.\n");
	return status;
}

static void tft18_remove(struct spi_device *spi){
	device_destroy(tft18_class, spi_data->devt);
	kfree(spi_data);
	printd("ERROR");
}

static struct spi_driver tft18_spi_driver = {
	.id_table = temp_spi,
	.driver = {
		.name = DRIVERNAME,
		.owner= THIS_MODULE,
	},
	.probe  = tft18_probe,
	.remove = tft18_remove,
};

int __init start_module(void)
{
	int retval;                                                               
	retval = register_chrdev(MAJORNO, DEVICENAME, &tft18_fops);                  //Register the Device
	if(retval < 0){
		printd("ERROR");
		return -1;
	}
	tft18_class = class_create(THIS_MODULE, DEVICECLASS);                    //Create the class
	if(IS_ERR(tft18_class))
	{
		printd("ERROR");
        goto r_class;
	}
	retval = spi_register_driver(&tft18_spi_driver);                              //Register the Driver
	if(retval < 0)
	{
		printd("ERROR");
        goto r_spi;
    }
	if (tft18_init() < 0){
		printd("ERROR");
        goto r_tft;
	}
    printd("INIT");
	return 0;
r_tft:
	spi_unregister_driver(&tft18_spi_driver);
	class_destroy(tft18_class);
r_class:
	unregister_chrdev(MAJORNO, tft18_spi_driver.driver.name);
	return -1;
}

void __exit end_module(void){
	gpio_free(A0_pin);
	gpio_free(RST_pin);
	spi_unregister_driver(&tft18_spi_driver);
	class_destroy(tft18_class);
	unregister_chrdev(MAJORNO,tft18_spi_driver.driver.name);
    printd("EXIT");
	
}

module_init(start_module);
module_exit(end_module);

MODULE_LICENSE("GPL");
