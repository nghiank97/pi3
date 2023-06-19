
#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "st7735.h"

u16 scr_width;
u16 scr_height;

#define spi_chanel 	  	1
#define spi_speed 	  	500000
#define	spi_cs_pin		16

#define	st7735_rst_pin	23
#define	st7735_a0_pin	24

int spi_fd;

int ST7735_spi_init(void){
  /* setup gpio */
  wiringPiSetupGpio();
  pinMode(spi_cs_pin, OUTPUT);
  digitalWrite(spi_cs_pin, 1);
  /* setup spi */
  if( (wiringPiSPISetup(spi_chanel, spi_speed)) <0){
	printf("SPI Setup failed\n");
	return 0;
  }
  spi_fd = wiringPiSPIGetFd(spi_chanel);
  return 1;
}

int ST7735_gpio_init(void){
  /* setup gpio */
  wiringPiSetupGpio();
  pinMode(st7735_rst_pin, OUTPUT);
  digitalWrite(st7735_rst_pin, 1);
  pinMode(st7735_a0_pin, OUTPUT);
  digitalWrite(st7735_a0_pin, 1);
  return 1;
}

void ST7735_reset(void){
	digitalWrite(st7735_rst_pin, 1);
	delay(5);
	digitalWrite(st7735_rst_pin, 0);
	delay(5);
  digitalWrite(st7735_rst_pin, 1);
}
void A0_L(void){
	digitalWrite(st7735_a0_pin, 0);
}
void A0_H(void){
	digitalWrite(st7735_a0_pin, 1);
}
void CS_L(void){
	digitalWrite(spi_cs_pin, 0);
}
void CS_H(void){
	digitalWrite(spi_cs_pin, 1);
}
void ST7735_write(u08 data){
	write(spi_fd,&data, 1);
}
u08 ST7735_read(void){
	u08 c = 0;
	read(spi_fd,&c, 1);
	return c;
}
u16 ST7735_write_page(u08* pBuf, u16 len){
	write(spi_fd,pBuf, len);
	return len;
}
u16 ST7735_read_page(u08* pBuf, u16 len){
	read(spi_fd,pBuf, len);
	return len;
}

void ST7735_cmd(u08 cmd) {
	A0_L();
	ST7735_write(cmd);
}

void ST7735_data(u08 data) {
	A0_H();
	ST7735_write(data);
}

u16 RGB565(u08 R,u08 G,u08 B) {
	return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}

void ST7735_Init(void) {
	ST7735_spi_init();
	ST7735_gpio_init();

	// Reset display
	CS_H();
	ST7735_reset();
	CS_L();

	ST7735_cmd(ST7735_SLPOUT); 		// Sleep out & booster on
	delay(120); 								// Datasheet says what display wakes about 120ms (may be much faster actually)
	
/*
	B1H : FRMCTR1 - Frame Rate Control (In normal mode/ Full colors)
	Frame rate=fosc/((RTNA + 20) x (LINE + FPA + BPA))
	focs = 333kHz
*/
	ST7735_cmd(ST7735_FRMCTR1);   	// In normal mode (full colors):
	A0_H();
	ST7735_write(0x05); 	// RTNA set 1-line period: RTNA2, RTNA0
	ST7735_write(0x3c); 	// Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_write(0x3c); 	// Back porch: BPA5,BPA4,BPA3,BPA2

// Frame rate=fosc/((RTNA + 20) x (LINE + FPA + BPA))

	ST7735_cmd(0xb2);   	// In idle mode (8-colors):
	A0_H();
	ST7735_write(0x05); 	// RTNB set 1-line period: RTNAB, RTNB0
	ST7735_write(0x3c); 	// Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_write(0x3c); 	// Back porch: BPB5,BPB4,BPB3,BPB2

	ST7735_cmd(0xb3);   	// In partial mode + full colors:
	A0_H();
	ST7735_write(0x05); 	// RTNC set 1-line period: RTNC2, RTNC0
	ST7735_write(0x3c); 	// Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_write(0x3c); 	// Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_write(0x05); 	// RTND set 1-line period: RTND2, RTND0
	ST7735_write(0x3c); 	// Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_write(0x3c); 	// Back porch: BPD5,BPD4,BPD3,BPD2

	ST7735_cmd(0xB4);   	// Display dot inversion control:
	ST7735_data(0x03);  	// NLB,NLC

	ST7735_cmd(0x3a);   	// Interface pixel format
	ST7735_data(0x05);  	// 16-bit/pixel RGB 5-6-5 (65k colors)
	ST7735_cmd(0x20);   	// Display inversion off

	ST7735_cmd(0x13);   	// Partial mode off

	ST7735_cmd(0x26);   	// Gamma curve set:
	ST7735_data(0x01);  	// Gamma curve 1 (G2.2) or (G1.0)
	ST7735_cmd(0x29);   	// Display on

	CS_H();
}

void ST7735_Orientation(ScrOrientation_TypeDef orientation) {
	CS_L();
	ST7735_cmd(0x36); // Memory data access control:
	switch(orientation) {
	case scr_CW:
		scr_width  = scr_h;
		scr_height = scr_w;
		ST7735_data(0xA0); // X-Y Exchange,Y-Mirror
		break;
	case scr_CCW:
		scr_width  = scr_h;
		scr_height = scr_w;
		ST7735_data(0x60); // X-Y Exchange,X-Mirror
		break;
	case scr_180:
		scr_width  = scr_w;
		scr_height = scr_h;
		ST7735_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		break;
	default:
		scr_width  = scr_w;
		scr_height = scr_h;
		ST7735_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		break;
	}
	CS_H();
}

void ST7735_AddrSet(uint16_t XS, u16 YS, u16 XE, u16 YE) {
	ST7735_cmd(0x2a); // Column address set
	A0_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(0x2b); // Row address set
	A0_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(0x2c); // Memory write
}

void ST7735_Clear(u16 color) {
	u16 i;
	CS_L();
	ST7735_AddrSet(0,0,scr_width - 1,scr_height - 1);
	A0_H();
	for (i = 0; i < scr_width * scr_height; i++) {
		ST7735_write(0x00);
		ST7735_write(0x00);
	}
	CS_H();
}

void ST7735_Pixel(u16 X, u16 Y, u16 color) {
    CS_L();
    ST7735_AddrSet(X,Y,X,Y);
    A0_H();
    ST7735_write(color >> 8);
    ST7735_write((uint8_t)color);
    CS_H();
}
