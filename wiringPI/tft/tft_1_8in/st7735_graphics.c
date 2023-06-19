#include "st7735_graphics.h"

#define _abs(x) (x>=0?x:-x)

void ST7735_HLine(u16 X1, u16 X2, u16 Y, u16 color) {
    u16 i;
    u08 CH = color >> 8;
    u08 CL = (u08)color;

    CS_L();
    ST7735_AddrSet(X1,Y,X2,Y);
	A0_H();
	for (i = 0; i <= (X2 - X1); i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_VLine(u16 X, u16 Y1, u16 Y2, u16 color) {
    u16 i;
    u08 CH = color >> 8;
    u08 CL = (u08)color;

    CS_L();
    ST7735_AddrSet(X,Y1,X,Y2);
	A0_H();
	for (i = 0; i <= (Y2 - Y1); i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Line(i16 X1, i16 Y1, i16 X2, i16 Y2, u16 color) {
	i16 dX = X2-X1;
	i16 dY = Y2-Y1;
	i16 dXsym = (dX > 0) ? 1 : -1;
	i16 dYsym = (dY > 0) ? 1 : -1;

	if (dX == 0) {
		if (Y2>Y1) ST7735_VLine(X1,Y1,Y2,color); else ST7735_VLine(X1,Y2,Y1,color);
		return;
	}
	if (dY == 0) {
		if (X2>X1) ST7735_HLine(X1,X2,Y1,color); else ST7735_HLine(X2,X1,Y1,color);
		return;
	}

	dX *= dXsym;
	dY *= dYsym;
	i16 dX2 = dX << 1;
	i16 dY2 = dY << 1;
	i16 di;

	if (dX >= dY) {
		di = dY2 - dX;
		while (X1 != X2) {
			ST7735_Pixel(X1,Y1,color);
			X1 += dXsym;
			if (di < 0) {
				di += dY2;
			} else {
				di += dY2 - dX2;
				Y1 += dYsym;
			}
		}
	} else {
		di = dX2 - dY;
		while (Y1 != Y2) {
			ST7735_Pixel(X1,Y1,color);
			Y1 += dYsym;
			if (di < 0) {
				di += dX2;
			} else {
				di += dX2 - dY2;
				X1 += dXsym;
			}
		}
	}
	ST7735_Pixel(X1,Y1,color);
}

extern void ST7735_8_point(int xc, int yc, int x, int y, u16 color)
{
    ST7735_Pixel( y + xc, x + yc, color);
    ST7735_Pixel(-y + xc, x + yc, color);
    ST7735_Pixel(y + xc, -x + yc, color);
    ST7735_Pixel(-y + xc, -x + yc, color);
    
    ST7735_Pixel(x + xc, y + yc, color);
    ST7735_Pixel(-x + xc, y + yc, color);
    ST7735_Pixel(x + xc, -y + yc, color);
    ST7735_Pixel(-x + xc, -y + yc, color);
}

extern void ST7735_Circle(int yc,int xc,int r,u16 color){
    int x = 0; int y = r;
    int f = 1 - r;
    ST7735_8_point(yc, xc, y, x, color);
    while (x < y){
        if (f < 0){
            f += 2*x + 3;
        }
        else{
            y--;
            f += 2*(x-y) + 5;
        }
        x++;
        ST7735_8_point(yc, xc, y, x, color);
    }
}
