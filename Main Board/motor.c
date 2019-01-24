#include "motor.h"
#include "config.h"
#include <math.h>
#include "/home/adam/ChibiOS_16.1.9/os/hal/lib/streams/chprintf.h"

thread_t *motor_save_thread;



volatile int16_t rotations_per_sec[NUM_OF_MOTORS];
int32_t motor_freqs[NUM_OF_MOTORS];
int16_t motor_actual_speeds[NUM_OF_MOTORS] = { 0, 0, 0 };
int16_t period[3];

bool i_occured[NUM_OF_MOTORS] = { false, false, false };

void encoder_pulse_captured(ICUDriver *icup) {
    int16_t period_width = 1000000/icuGetPeriodX(icup);
    int16_t period_calc = (1848*period_width-281984)/1667;
    if (icup == &ICUD2) {
   

    } else if (icup == &ICUD5) {
        rotations_per_sec[1] = 1000 / (period_width * 24);
    } else if (icup == &ICUD3) {
        rotations_per_sec[2] = 1000 / (period_width * 24);
    }
         i_occured[0] = true;
        rotations_per_sec[0] = period_width;
      
        period[0] = motor_freqs[0];
        // ==
        period[1] = period_calc;

        period[2] = motor_actual_speeds[0]; 
       
        if(period_calc < motor_freqs[0]) {
            // −1,352813853 3270,562770563
            //motor_actual_speeds[0] += (motor_freqs[0] - period_calc)/256 //+ 1;
            pwmEnableChannel(&PWMD1, 0, ++motor_actual_speeds[0]);
        } else {
            //motor_actual_speeds[0] -= (period_calc - motor_freqs[0])/256 //+ 1;
            pwmEnableChannel(&PWMD1, 0, --motor_actual_speeds[0]);
        }
}

 // TODO
 // speed between 0 - 100
 // encoder movement
 
void move_motor(int8_t motor_number, int16_t speed) {
    if (speed < 0) {
        set_motor_state(0, motor_number);
    } else {
        set_motor_state(1, motor_number);
    }

    motor_freqs[motor_number] = abs(speed); 

    if (motor_actual_speeds[motor_number] == 0) {
        pwmEnableChannel(&PWMD1, motor_number, abs(speed));
        motor_actual_speeds[motor_number] = abs(speed);
        motor_checker(motor_number);
    } 
}

void motor_checker(int8_t motor_number) {
    if(!i_occured[motor_number] && motor_actual_speeds[motor_number] != 0) {
    pwmEnableChannel(&PWMD1, motor_number, ++motor_actual_speeds[motor_number]);
    } else { return; }
    motor_checker(motor_number);
}
 
void calculate_speed(float alpha) {
    if(alpha != 0) {
    float target_motor = ceil(alpha / (2 / NUM_OF_MOTORS));
    
    } else {
        set_motors_off();
    }
}

void move_motors(float alpha) {
    calculate_speed(alpha);
}

void set_motor_off(int8_t motor_number) {
    configure_icu_notifications(false);
	motor_freqs[motor_number] = 0;
    configure_icu_notifications(true);
    set_motor_state(MOTOR_BREAK, motor_number);
}

void set_motors_off() {
    for(int8_t i = 0; i < NUM_OF_MOTORS; i++) {
        set_motor_state(MOTOR_BREAK, i);
    }
}

void read_motors_speed(int16_t *buff) {
    configure_icu_notifications(false);
    for(int i = 0; i < NUM_OF_MOTORS; i++) {
        buff[i] = rotations_per_sec[i];
    }
    configure_icu_notifications(true);
}

void read_motors_period(int16_t *buff) {
    //configure_icu_notifications(false);
    for(int i = 0; i < 3; i++) {
        buff[i] = period[i];
    }
    //configure_icu_notifications(true);
}

void set_motor_state(int8_t dir, int8_t num) {
    
    int8_t pinA;
    int8_t pinB;
    
    switch(num) {
        case 0:
            pinA = 12;
            pinB = 10;
            break;
        case 1:
            pinA = 9;
            pinB = 8;
            break;
            pinA = 0;
            pinB = 0;
            break;
    }
    
    switch(dir) {
        case 0:
            palClearPad(GPIOC, pinA);
            palClearPad(GPIOC, pinB);
            break;
        case 1:
            palSetPad(GPIOC, pinA);
            palClearPad(GPIOC, pinB);
            break;
        case 2:
            palSetPad(GPIOC, pinB);
            palClearPad(GPIOC, pinA);
            break;
        case 3:
            palSetPad(GPIOC, pinA);
            palSetPad(GPIOC, pinB);
            break;
    }
}