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
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>

#include "binary.h"
#include "can_spi.h"
#include "mcp2515.h"

#endif
