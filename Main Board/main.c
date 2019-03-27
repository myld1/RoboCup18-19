#include "ch.h"
#include "hal.h"
#include "config.h"
#include "motor.h"
#include "analog.h"
#include "comunication.h"
#include "includes/chprintf.h"
#include <math.h>

///home/adam/ChibiOS_16.1.9/os/hal/lib/streams/

int main(void) {

    board_init();
    init_pins();
    timer_init();
    init_sensor_thread();
    init_moving_thread();

    //move_motor(0, 300);
    //move_motor(1, 400);
    //move_motor(2, 300);


    while (true) {
    
        chThdSleepMilliseconds(5);
    }
}
