#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <asm/io.h>

#define u08 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define i08 int8_t
#define i16 int16_t
#define i32 int32_t
#define f32 float

#define BCM2835_PERI_BASE      (0x3F000000)
#define GPIO_BASE              (BCM2835_PERI_BASE + 0x200000)

#define BLOCK_SIZE 		4096
#define GPFSEL0    		0x00
#define GPFSEL1    		0x04
#define GPFSEL2    		0x08
#define GPSET0     		0x1C
#define GPCLR0     		0x28

#define UART_BASE              (BCM2835_PERI_BASE + 0x7E20100)

#define FUARTCLK		64000000

#define DR    			0x00	// data register
#define RSRECR    		0x04	// receive status register//error clear
#define FR    			0x18	// flags register
#define ILPR    		0x20
#define IBRD     		0x24
#define FBRD     		0x28
#define LCRH     		0x2C	// the line control register
#define CR     			0x30	// control register

#define IFLS     		0x34
#define IMSC     		0x38
#define RIS     		0x3C
#define MIS     		0x40

#define ICR     		0x44
#define DMACR     		0x48
#define ITCR     		0x80
#define ITIP     		0x84
#define ITOP     		0x88
#define TDR     		0x8C

volatile unsigned int __iomem *gpio;
volatile unsigned int __iomem *uart;

bool gpio_init(void){
	// config the gpio
	gpio = ioremap(GPIO_BASE, 4096);
	if (gpio == NULL) {
		pr_info("io remap failed\n");
		iounmap(gpio);
		return false;
	}
	return true;
}

bool uart_init(void){
	// config the uart
	uart = ioremap(UART_BASE, 4096);
	if (uart == NULL){
		pr_err("Can't remap uart");
		iounmap(uart);
		return false;
	}
	// clean gpio 14,15
	gpio[GPFSEL1/4] &=~ (7 << 12);
	gpio[GPFSEL1/4] &=~ (7 << 15);
	// alternate function 0
	gpio[GPFSEL1/4] |= (0b001 << 12);
	gpio[GPFSEL1/4] |= (0b001 << 15);

	// data 8 bit
	uart[LCRH/4] |= (0b11<<5);
	// parity disable
	uart[LCRH/4] &=~ (1<<1);
	// set baudrate : 115200
	// = 640000000 / (16 * 115200) = 347.222222
	// the integer number
	uart[IBRD/4] = 347;
	// the float number
	uart[FBRD/4] = 4; // 16*.222222

	/* the config is followed the datasheet in 182 page */
	// disable uart
	uart[CR/4] &=~ (1<<0);
	// wait the end of transmission
	while((uart[FR/4]&(1<<7)) != 0x00);
	// wait the end of reception
	while((uart[FR/4]&(1<<4)) != 0x00);
	// disable FIFOs
	uart[LCRH/4] &=~ (0b11<<4);
	// uart enable tx,rx
	uart[CR/4] |= (1<<8)|(1<<9);
	// uart enable
	uart[CR/4] |= (1<<0);

	return true;
}

void uart_byte(u08 ch){
	uart[DR/4] &=~ 0xFF;
	uart[DR/4] |= ch;
	// wait the end of transmission
        while((uart[FR/4]&(1<<7)) != 0x00);
}

void uart_print(u08 *data, u16 len){
	for(u16 i=0;i<len;i++){
		uart_byte(data[i]);
	}
}

static int __init module_start(void){

	pr_info(": start\n");
	// config the gpio
	if(gpio_init() == false){
		pr_err("Can't map the gpio\n");
		return -1;
	}
	// config the uart
	if(uart_init() == false){
		pr_err("Can't map the uart\n");
		return -1;
	}

	pr_info("%d \n",FUARTCLK);
	for(u16 i=0;i<10;i++){
		uart_print("nghia\r\n",7);
	}
	return 0;
}

static void __exit module_end(void){
	iounmap(gpio);
	iounmap(uart);
	pr_info(": end\n");
}

module_init(module_start);
module_exit(module_end);

MODULE_LICENSE("GPL");
