#include "motor.h"
#include "config.h"
#include <math.h>
#include "/home/adam/ChibiOS_16.1.9/os/hal/lib/streams/chprintf.h"

thread_t *motor_save_thread;

volatile int16_t rotations_per_sec[NUM_OF_MOTORS];
int8_t motor_speed_calculation[NUM_OF_MOTORS];
int8_t motor_speed_temp[NUM_OF_MOTORS];


void encoder_pulse_captured(ICUDriver *icup) {
    int16_t period_width = icuGetPeriodX(icup);
	if ()
    if (icup == &ICUD2) {
        rotations_per_sec[0] = 1000 / (period_width * 24);
    } else if (icup == &ICUD5) {
        rotations_per_sec[1] = 1000 / (period_width * 24);
    } else if (icup == &ICUD3) {
        rotations_per_sec[2] = 1000 / (period_width * 24);
    }
}

// COM THEARD
THD_WORKING_AREA(waMotorThread, 128);
THD_FUNCTION(MotorThread, arg) {
    (void)arg;
    
    thread_t *main;
    msg_t information;

    while(1) {
        main = chMsgWait();
        information = chMsgGet(main);
        if (information == UPDATE) {
			motor_speed_calculation = motor_speed_temp;
		}
    }
}


 // TODO
 // speed between 0 - 100
 // encoder movement
 
void move_motor(int8_t motor_number, int16_t speed) {
	motor_speed_temp[motor_number] = speed;
    chMsgSend(motor_save_thread, UPDATE);
}
 
void calculate_speed(float alpha) {
    if(alpha != 0) {
    float target_motor = ceil(alpha / (2 / NUM_OF_MOTORS));
    
    } else {
        set_motors_off();
    }
}

void move_motors(float angle) {
    calculate_speed(angle);
	chMsgSend(motor_save_thread, UPDATE);
}

void set_motor_off(int8_t motor_number) {
    set_motor_state(MOTOR_BREAK, motor_number);
}

void set_motors_off() {
    for(int8_t i = 0; i < NUM_OF_MOTORS; i++) {
        set_motor_state(MOTOR_BREAK, i);
    }
}

int16_t read_motors_speed() {
    configure_icu_notifications(false;
	rot = rotations_per_sec;
    configure_icu_notifications(true);
    return rot;
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
