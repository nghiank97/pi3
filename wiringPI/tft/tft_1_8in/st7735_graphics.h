
#ifndef ST7735_GRAPHICS_H
#define ST7735_GRAPHICS_H
#include "st7735.h"

extern void ST7735_Line(i16 X1, i16 Y1, i16 X2, i16 Y2, u16 color);
extern void ST7735_8_point(int xc, int yc, int x, int y, u16 color);
extern void ST7735_Circle(int yc,int xc,int r,u16 color);

#endif


