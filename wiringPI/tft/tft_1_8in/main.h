#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "binary.h"
#include "font5x7.h"
#include "garmin-digits.h"
#include "main.h"
#include "st7735_graphics.h"
#include "st7735_text.h"
#include "st7735.h"

extern void setup(void);
extern void loop(void);

#endif