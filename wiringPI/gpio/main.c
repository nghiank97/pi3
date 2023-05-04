#include <stdio.h>
#include <wiringPi.h>

int status = 0;
#define PIN_LED 26

int main (int argc, char **argv)
{
    wiringPiSetupGpio();
    pinMode(PIN_LED, OUTPUT);

    while (1){
        if (status == 0) {
            digitalWrite(PIN_LED, HIGH);
            status = 1;
	    printf("ON \r\n");
        }
        else {
            digitalWrite(PIN_LED, LOW);
            status = 0;
	    printf("OFF \r\n");
        }
        delay(1000);
    }
    return 0;
}
