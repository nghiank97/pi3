
#ifndef _ADS1220_H_
#define _ADS1220_H_
#include "binary.h"
#include "spi.h"

//ADS1220 SPI commands
#define SPI_MASTER_DUMMY    0xFF
#define RESET   			0x06   		//Send the RESET command (06h) to make sure the ADS1220 is properly reset after power-up
#define START   			0x08    	//Send the START/SYNC command (08h) to start converting in continuous conversion mode
#define STOP   				0x02
#define WREG  				0x40
#define RREG  				0x20

//Config registers
#define CONFIG_REG0_ADDRESS 0x00
#define CONFIG_REG1_ADDRESS 0x01
#define CONFIG_REG2_ADDRESS 0x02
#define CONFIG_REG3_ADDRESS 0x03

void ads1220_cmd(u08 data_in);
void ads1220_write_reg(u08 add, u08 value);
u08 ads1220_read_reg(u08 add);

void ads1220_begin(void);
void ads1220_start_conv(void);
void ads1220_stop_conv(void);
void ads1220_reset(void);

u08 ads1220_read_drdy();
i32 ads1220_read_continuous();

#endif /* _ADS1220_H_ */