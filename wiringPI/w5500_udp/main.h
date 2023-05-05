#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "binary.h"
#include "main.h"


extern void setup(void);
extern void loop(void);

#endif