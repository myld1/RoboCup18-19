#include "motor.h"
#include "config.h"
#include <math.h>
#include "/home/adam/ChibiOS_16.1.9/os/hal/lib/streams/chprintf.h"

thread_t *motor_save_thread;

volatile int16_t rotations_per_sec[NUM_OF_MOTORS];
int32_t motor_speeds[NUM_OF_MOTORS];
int16_t motor_actual_speeds[NUM_OF_MOTORS];

void encoder_pulse_captured(ICUDriver *icup) {
    int16_t period_width = icuGetPeriodX(icup);
    if (icup == &ICUD2) {
        rotations_per_sec[0] = period_width;
        chprintf(&SD2, "%d \r\n", (3270000 - 1352 * motor_speeds[0]) / 1000);
        if(period_width < (3270000 - 1352 * motor_speeds[0]) / 1000) {
            // −1,352813853 3270,562770563
            pwmEnableChannel(&PWMD1, 0, ++motor_actual_speeds[0]);
        } else {
            pwmEnableChannel(&PWMD1, 0, --motor_actual_speeds[0]);             
        }
    } else if (icup == &ICUD5) {
        rotations_per_sec[1] = 1000 / (period_width * 24);
    } else if (icup == &ICUD3) {
        rotations_per_sec[2] = 1000 / (period_width * 24);
    }
}

 // TODO
 // speed between 0 - 100
 // encoder movement
 
void move_motor(int8_t motor_number, int16_t speed) {
    configure_icu_notifications(false);
    if (motor_speeds[motor_number] == 0) {
        if (speed < 0) {
            set_motor_state(0, motor_number);
        } else {
            set_motor_state(1, motor_number);
        }
        pwmEnableChannel(&PWMD1, motor_number, 2048);
        motor_actual_speeds[motor_number] = 2048;
    }
    motor_speeds[motor_number] = speed;
    configure_icu_notifications(true);   
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
	motor_speeds[motor_number] = 0;
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
        case 2:
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