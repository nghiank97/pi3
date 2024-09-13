#ifndef _SPI_H
#define _SPI_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "binary.h"

u08 read_drdy(void);

int spi_init(void);
void spi_cs_l(void);
void spi_cs_h(void);
void spi_spi_write(u08 data);
u08 w5500_spi_read(void);
void spi_writes(u08* pBuf, u16 len);
void spi_reads(u08* pBuf, u16 len);

#endif /* _SPI_H */
