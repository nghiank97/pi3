
#ifndef ST7735_TEXT_H
#define ST7735_TEXT_H

#include <string.h>
#include "binary.h"
#include "st7735.h"
#include "font5x7.h"

void ST7735_PutChar5x7(u16 X, u16 Y, u08 chr, u16 color);
void ST7735_PutStr5x7(u08 X, u08 Y, char *str, u16 color);

#endif


