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
    init_sensor_thread();
    //move_motor(2, 400);

    while (true) {

        //motor_tick();
        chThdSleepMilliseconds(10);
        //chprintf(&SD1, "dhfkja \r\n");
    }
}
