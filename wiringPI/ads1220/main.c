#include "binary.h"
#include "spi.h"
#include "ads1220.h"

i32 raw = 0;
#define	drdy_pin		14

void gpio_callback(uint gpio, uint32_t events) {
    if(gpio==drdy_pin){
        raw = ads1220_read_continuous();
    }
}

int main(){
    ads1220_begin();
    gpio_set_irq_enabled(drdy_pin, GPIO_IRQ_EDGE_FALL, true);
    while(1){

    }
}