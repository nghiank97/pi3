#include "main.h"

#define printd(msg) printk("\t %s: %s:%d \n", msg, __func__, __LINE__);

int gpio_init(void);
void tft18_cmd(u08 cmd);
void tft18_data(u08 data);
void tft18_write(u08 data);
int tft18_init(void);
void tft18_orientation(ScrOrientation_TypeDef orientation);
void tff_add_set(u08 XS, u08 YS, u08 XE, u08 YE);
void tft18_clear(u16 color);
void tft18_pixel(u16 X, u16 Y, u16 color);
void tft18_put_char5x7(u16 X, u16 Y, u08 chr, u16 color);
void tft18_put_str5x7(u08 X, u08 Y, char *str, u16 color);
static int __init ModuleInit(void);
static void __exit ModuleExit(void);

#define MY_BUS_NUM 0
static struct spi_device *tft_18_dev;
u16 scr_width;
u16 scr_height;

int gpio_init(void){
//   if(gpio_is_valid(CS0_pin) == false){
//     printd("ERROR");
//     return -1;
//   }
//   if(gpio_request(CS0_pin,"CS0_pin") < 0){
//     printd("ERROR");
//     return -1;
//   }
//   gpio_direction_output(CS0_pin, 0);

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

void tft18_write(u08 data) {
	spi_write(tft_18_dev, &data, 1);
}

void tft18_cmd(u08 cmd) {
	A0_L();
    tft18_write(cmd);
}

void tft18_data(u08 data) {
	A0_H();
	tft18_write(data);
}

int tft18_init(void) {
	// Reset display
	CS_H();
	RST_H();
	mdelay(5);
	RST_L();
	mdelay(5);
	RST_H();
	CS_H();
	mdelay(5);
	CS_L();

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

	CS_H();

	tft18_orientation(scr_normal);
	return 0;
}

void tft18_orientation(ScrOrientation_TypeDef orientation) {
	CS_L();
	tft18_cmd(0x36); // Memory data access control:
	switch(orientation) {
	case scr_CW:
		scr_width  = scr_h;
		scr_height = scr_w;
		tft18_data(0xA0); // X-Y Exchange,Y-Mirror
		break;
	case scr_CCW:
		scr_width  = scr_h;
		scr_height = scr_w;
		tft18_data(0x60); // X-Y Exchange,X-Mirror
		break;
	case scr_180:
		scr_width  = scr_w;
		scr_height = scr_h;
		tft18_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		break;
	default:
		scr_width  = scr_w;
		scr_height = scr_h;
		tft18_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		break;
	}
	CS_H();
}

void tff_add_set(u08 XS, u08 YS, u08 XE, u08 YE) {
	tft18_cmd(0x2a); // Column address set
	A0_H();
	tft18_write(XS >> 8);
	tft18_write(XS);
	tft18_write(XE >> 8);
	tft18_write(XE);

	tft18_cmd(0x2b); // Row address set
	A0_H();
	tft18_write(YS >> 8);
	tft18_write(YS);
	tft18_write(YE >> 8);
	tft18_write(YE);

	tft18_cmd(0x2c); // Memory write
}

void tft18_clear(u16 color) {
	u16 i;
	CS_L();
	tff_add_set(0,0,scr_width - 1,scr_height - 1);
	A0_H();
	for (i = 0; i < scr_width * scr_height; i++) {
		tft18_write(0x00);
		tft18_write(0x00);
		tft18_write(0x00);
	}
	CS_H();
}

void tft18_pixel(u16 X, u16 Y, u16 color) {
    CS_L();
    tff_add_set(X,Y,X,Y);
    A0_H();
    tft18_write(color >> 8);
    tft18_write((u08)color);
    CS_H();
}

void tft18_put_char5x7(u16 X, u16 Y, u08 chr, u16 color) {
	u16 i,j;
	u08 buffer[5];
	u08 CH = color >> 8;
	u08 CL = (u08)color;
	memcpy(buffer,&Font5x7[(chr - 32) * 5],5);
	CS_L();
	tff_add_set(X,Y,X + 4,Y + 6);
	A0_H();
	for (j = 0; j < 7; j++) {
        for (i = 0; i < 5; i++) {
    		if ((buffer[i] >> j) & 0x01) {
    			tft18_write(CH);
    			tft18_write(CL);
    		} else {
    			tft18_write(0x00);
    			tft18_write(0x00);
    		}
    	}
    }
	CS_H();
}

void tft18_put_str5x7(u08 X, u08 Y, char *str, u16 color) {
    while (*str) {
        tft18_put_char5x7(X,Y,*str++,color);
        if (X < scr_width - 6) { X += 6; } else if (Y < scr_height - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
    };
}

static int __init ModuleInit(void) {
	struct spi_master *master;
	/* Parameters for SPI device */
	struct spi_board_info spi_device_info = {
		.modalias = "tft_18_in",
		.max_speed_hz = 4200000,
		.bus_num = MY_BUS_NUM,
		.chip_select = 0,
		.mode = 1,
	};

	if (gpio_init() < 0){
		printd("ERROR");
		gpio_free(A0_pin);
		gpio_free(RST_pin);
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
	tft_18_dev -> bits_per_word = 8;
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
/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	tft18_clear(0x0000);
	gpio_free(A0_pin);
	gpio_free(RST_pin);
	if(tft_18_dev){
		spi_unregister_device(tft_18_dev);
    }
	printd("EXIT");
}

module_init(ModuleInit);
module_exit(ModuleExit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple tft-spi device");
