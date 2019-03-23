#include "ch.h"
#include "hal.h"
#include "config.h"
#include "motor.h"
#include "analog.h"
#include "includes/chprintf.h"
///home/adam/ChibiOS_16.1.9/os/hal/lib/streams/

int main(void) {

    board_init();
    init_pins();
    chprintf(&SD1, "Loaded! \r\n");
    //init_sensor_thread();
    timer_init();
    palClearPad(GPIOA, GPIOA_LED_GREEN);
    move_motor(0, 600);
    move_motor(1, 600);
    move_motor(2, 600);

    while (true) {
        chThdSleepMilliseconds(10);
    }   
}
