#include "ch.h"
#include "hal.h"
#include "config.h"
#include "motor.h"
#include "/home/adam/ChibiOS_16.1.9/os/hal/lib/streams/chprintf.h"

int main(void) {
  
    board_init();
    init_pins();
    
    while (true) {
          /*for(int16_t i = 0; i < 1500; i++) {
            move_motor(0,-1500 + i);
            chThdSleepMilliseconds(5);
            chprintf(&SD1,"speed: %d\r\n",read_motor_speed());
          }*/
            move_motor(0,2048);
            chThdSleepMilliseconds(5);
			int16_t speeds = read_motors_speed();
			chprintf(&SD1,"speed: %d %d %d \r\n", speeds[0], speeds[1], speeds[2]);
    }
}
