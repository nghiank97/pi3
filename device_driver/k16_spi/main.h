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

#include "font5x7.h"
#include "garmin-digits.h"

#define u08 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define i08 int8_t
#define i16 int16_t
#define i32 int32_t

#define f32 float

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define A0_pin	25
#define CS0_pin 8
#define RST_pin 7

#define CS_H()       		(gpio_set_value(CS0_pin,1))
#define CS_L()      		(gpio_set_value(CS0_pin,0))

#define A0_H()       		(gpio_set_value(A0_pin,1))
#define A0_L()      		(gpio_set_value(A0_pin,0))

#define RST_H()       		(gpio_set_value(RST_pin,1))
#define RST_L()      		(gpio_set_value(RST_pin,0))


typedef enum {
	scr_normal = 0,
	scr_CW     = 1,
	scr_CCW    = 2,
	scr_180    = 3
} ScrOrientation_TypeDef;

#define scr_w       	128
#define scr_h       	160

#endif
