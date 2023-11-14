#ifndef MAIN_H
#define MAIN_H

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/spi/spi.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/param.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "binary.h"
#include "can_spi.h"
#include "mcp2515.h"

#define MCP2515_SPI_BUS     0
#define MCP2515_INT_Pin     25
#define MCP2515_CS_Pin      8
#define MCP2515_CS_HIGH()   (gpio_set_value(MCP2515_CS_Pin, 1))
#define MCP2515_CS_LOW()    (gpio_set_value(MCP2515_CS_Pin, 0))

extern u16 SPI_Tx(u08 data);
extern u16 SPI_TxBuffer(u08 *buffer, u16 length);
extern u08 SPI_Rx(void);
extern u16 SPI_RxBuffer(u08 *buffer, u16 length);

#endif
