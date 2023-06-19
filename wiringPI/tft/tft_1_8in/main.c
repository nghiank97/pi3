#include <stdio.h>
#include "main.h"

extern void setup(){
	ST7735_Init();
	ST7735_Clear(0x0000);
	ST7735_PutStr5x7(10, 10, "nghia", RGB565(236, 252, 3));
}

extern void loop(){
  printf("hello \n");
}

int main(){
  setup();
  while(1){
    loop();
  }
}