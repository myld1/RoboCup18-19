#include "motor.h"
#include "config.h"
#include "includes/chprintf.h"

//int8_t main_timer[NUM_OF_MOTORS] = { -ENCODER_OFFSET, -ENCODER_OFFSET, -ENCODER_OFFSET };
virtual_timer_t main_timer[NUM_OF_MOTORS];
virtual_timer_t asd;
int8_t index = -1;

volatile int16_t rotations_per_sec[NUM_OF_MOTORS];
int32_t motor_freqs[NUM_OF_MOTORS];
int16_t motor_actual_speeds[NUM_OF_MOTORS] = { 0, 0, 0 };
int16_t period[3];

void motor_tick();

void encoder_pulse_captured(ICUDriver *icup) {
    int16_t period_width = 1000000/icuGetPeriodX(icup);
    int16_t period_calc = (1848*period_width-281984)/1667;

    //ICUD sender
    int8_t sender = -1;
    if (icup == &ICUD2) {
        sender = 0;
    } else if (icup == &ICUD5) {
        sender = 1;
    } else if (icup == &ICUD3) {
        sender = 2;
    }   
    if (sender >= 0) {
        index = sender;
        palClearPad(GPIOA, GPIOA_LED_GREEN);
        chVTSet(&main_timer[sender], MS2ST(2000), motor_tick, NULL);
        //chprintf(&SD1, "%d %5d %5d %5d %5d \r\n",sender, main_timer[sender], motor_actual_speeds[sender], period[sender], motor_freqs[sender]);
        
        // stats
        rotations_per_sec[sender] = period_width;
        period[sender] = period_calc;
        
        if(period_calc < motor_freqs[sender]) {
            // −1,352813853 3270,562770563
            //motor_actual_speeds[0] += (motor_freqs[0] - period_calc)/256 //+ 1;
            pwmEnableChannel(&PWMD1, sender, ++motor_actual_speeds[sender]);
        } else {
            //motor_actual_speeds[0] -= (period_calc - motor_freqs[0])/256 //+ 1;
            pwmEnableChannel(&PWMD1, sender, --motor_actual_speeds[sender]);
        }
    }
}

void move_motor(int8_t motor_number, int16_t speed) {
    if (speed < 0) {
        set_motor_state(2, motor_number);
        speed *= -1;
    } else if (speed > 0) {
        set_motor_state(1, motor_number);
    } else {
        set_motor_state(0, motor_number);
    }

    motor_freqs[motor_number] = speed; 
    if (motor_actual_speeds[motor_number] == 0) {
        pwmEnableChannel(&PWMD1, motor_number, speed);
        motor_actual_speeds[motor_number] = speed;
        //motor_checker(motor_number); 
    } 
    index = motor_number;
    
    //chVTReset(&main_timer[motor_number]);
    chVTSet(&main_timer[motor_number], MS2ST(2000), motor_tick, NULL);
}

/*void motor_checker(int8_t motor_number) {
    if(main_timer[motor_number] < 0 && motor_actual_speeds[motor_number] != 0) {
    pwmEnableChannel(&PWMD1, motor_number, ++motor_actual_speeds[motor_number]);
    } else { return; }
    motor_checker(motor_number);
    chprintf(&SD1, "%d \r\n", motor_actual_speeds[motor_number]);
}*/

void motor_tick() {
    int8_t i = index;
    index = -1;
    palSetPad(GPIOA, GPIOA_LED_GREEN);
    //chprintf(&SD1, "%d %5d %5d %5d %5d \r\n",i, main_timer[i], motor_actual_speeds[i], period[i], motor_freqs[i]);
    if (motor_actual_speeds[i] < 2048) {
        pwmEnableChannel(&PWMD1, i, ++motor_actual_speeds[i]);
    } else {
        chVTReset(&main_timer[i]);
        motor_actual_speeds[i] = motor_freqs[i]; // set to default
        //chprintf(&SD1, "_Error: Motor malfunction or bad power supply on motor %d \r \n", i);
    }
}

void timer_init() {
    for (int8_t i = 0; i < NUM_OF_MOTORS; i++) {
     chVTObjectInit(&main_timer[i]);
    }
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
        case 2:
            pinA = 5;
            pinB = 6;
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
    }
}