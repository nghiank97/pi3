#include "main.h"

#define printd(msg) printk("\t %s: %s:%d \n", msg, __func__, __LINE__);


static struct spi_device *mcp2515_dev;
u16 MCP2515_INT_Num = 0;
u32 count = 0;

//Interrupt handler for GPIO 25. This will be called whenever there is a raising edge detected. 
static irqreturn_t mcp2515_irq_handler(int irq,void *dev_id) {
  count+=1;
  pr_info("Interrupt Occurred : %d \n",count);
  return IRQ_HANDLED;
}

extern u16 SPI_Tx(u08 data){
	spi_write(mcp2515_dev, &data, 1);
 return 1;
}
extern u16 SPI_TxBuffer(u08 *buffer, u16 length){
  spi_write(mcp2515_dev, buffer, length);
  return length;
}
extern u08 SPI_Rx(void){
  u08 data[1] = {0};
  spi_read(mcp2515_dev, data, 1);
  return data[0];
}
extern u16 SPI_RxBuffer(u08 *buffer, u16 length){
  spi_read(mcp2515_dev, buffer, length);
  return length;
}

static int __init mcp2515_probe(void) {

	struct spi_controller master = {
    .bus_num = MCP2515_SPI_BUS,
    .num_chipselect = 0,
 };
	struct spi_board_info spi_device_info = {
		.modalias = "mcp2515_can",
		.max_speed_hz = 4200000,
		.bus_num = MCP2515_SPI_BUS,
		.chip_select = 0,
		.mode = 1,
	};
 
	/* Create new SPI device */
	mcp2515_dev = spi_new_device(&master, &spi_device_info);
	if(!mcp2515_dev) {
		printd("ERROR");
		return -1;
	}
	mcp2515_dev -> bits_per_word = 8;
	/* Setup the bus for device's parameters */
	if(spi_setup(mcp2515_dev) != 0){
		printd("ERROR");
		spi_unregister_device(mcp2515_dev);
		return -1;
	}
 
  //  Input GPIO configuratioin
  //  Checking the GPIO is valid or not
  if(gpio_is_valid(MCP2515_INT_Pin) == false){
    pr_err("GPIO %d is not valid\n", MCP2515_INT_Pin);
    goto r_gpio_in;
  }
  
  //Requesting the GPIO
  if(gpio_request(MCP2515_INT_Pin,"MCP2515_INT_Pin") < 0){
    pr_err("ERROR: GPIO %d request\n", MCP2515_INT_Pin);
    goto r_gpio_in;
  }
  
  //configure the GPIO as input
  gpio_direction_input(MCP2515_INT_Pin);
  //Get the IRQ number for our GPIO
  MCP2515_INT_Num = gpio_to_irq(MCP2515_INT_Pin);
  pr_info("GPIO_irqNumber = %d\n", MCP2515_INT_Num);
  
  if (request_irq(MCP2515_INT_Num,             //IRQ number
                  (void *)mcp2515_irq_handler,   //IRQ handler
                  IRQF_TRIGGER_RISING,        //Handler will be called in raising edge
                  NULL,               //used to identify the device name using this IRQ
                  NULL)) {                    //device id for shared IRQ
    pr_err("my_device: cannot register IRQ ");
    goto r_gpio_in;
  }
 
	printd("------  INIT");
	return 0;
r_gpio_in:
  gpio_free(MCP2515_INT_Pin);
   return -1;
 
}

static void __exit mcp2515_remove(void) {
  gpio_free(MCP2515_INT_Pin);
	if(mcp2515_dev){
		spi_unregister_device(mcp2515_dev);
  }
	printd("------  EXIT");
}

module_init(mcp2515_probe);
module_exit(mcp2515_remove);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple mcp2515 spi");




