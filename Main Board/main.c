#include "ch.h"
#include "hal.h"
#include "config.h"
#include "motor.h"
#include "includes/chprintf.h"
///home/adam/ChibiOS_16.1.9/os/hal/lib/streams/

int main(void) {

    board_init();
    init_pins();
    move_motor(0, 200);
    move_motor(1, 400);
    move_motor(2, 600);

    while (true) {
        motor_tick();
        chThdSleepMilliseconds(10);
        //int16_t per[3];
        //read_motors_period(per);
        chprintf(&SD1,"d \r\n");      
    }
}
