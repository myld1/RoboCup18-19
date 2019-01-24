#include "ch.h"
#include "hal.h"
#include "config.h"
#include "motor.h"
#include "includes/chprintf.h"
///home/adam/ChibiOS_16.1.9/os/hal/lib/streams/

int main(void) {

    board_init();
    init_pins();
    move_motor(0, 400);

    while (true) {
        
        for(int8_t i = 0; i < NUM_OF_MOTORS; i++)
        {
          if(main_timer[i] != -1) {
            if(main_timer[i] < 250) {
                main_timer[i]++;   
            } else {
                chprintf(&SD1, "Error: Battery error or bad powwer supply on motor %d /r/n", i);
            }
          }
        }
        
        chThdSleepMilliseconds(4);
        //int16_t per[3];
        //read_motors_period(per);
        //chprintf(&SD1,"%d %d %d \r\n", per[0], per[1], per[2]);      
    }
}
