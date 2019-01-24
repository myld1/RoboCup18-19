#include "ch.h"
#include "hal.h"
#include "config.h"
#include "motor.h"
#include "/home/adam/ChibiOS_16.1.9/os/hal/lib/streams/chprintf.h"

int main(void) {
  
    board_init();
    init_pins();
    move_motor(0, 400);
    
    while (true) {
            chThdSleepMilliseconds(5);
			int16_t per[3];
      read_motors_period(per);
      chprintf(&SD1,"%d %d %d \r\n", per[0], per[1], per[2]);
			//chprintf(&SD1,"speed: %d %d %d \r\n", speeds[0], speeds[1], speeds[2]);
      
    }
}
