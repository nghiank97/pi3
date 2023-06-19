
#ifndef ST7735_H
#define ST7735_H

#ifdef __cplusplus
extern "C"{
#endif

#include "binary.h"
#include "main.h"
#include "font5x7.h"
#include "garmin-digits.h"

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

typedef enum {
	scr_normal = 0,
	scr_CW     = 1,
	scr_CCW    = 2,
	scr_180    = 3
} ScrOrientation_TypeDef;

#define scr_h						128
#define scr_w						160

extern u16 scr_width;
extern u16 scr_height;

int ST7735_spi_init(void);
int ST7735_gpio_init(void);
void A0_L(void);
void A0_H(void);
void CS_L(void);
void CS_H(void);
u16 RGB565(u08 R,u08 G,u08 B);

extern void ST7735_reset(void);
extern void ST7735_cmd(u08 cmd);
extern void ST7735_data(u08 data);
extern void ST7735_write(u08 data);

extern void ST7735_Init(void);

void ST7735_Orientation(ScrOrientation_TypeDef orientation);
void ST7735_AddrSet(uint16_t XS, u16 YS, u16 XE, u16 YE);
void ST7735_Clear(u16 color);
void ST7735_Pixel(u16 X, u16 Y, u16 color);

#ifdef __cplusplus
}
#endif

#endif