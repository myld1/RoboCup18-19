#include "motor.h"
#include "config.h"
#include "includes/chprintf.h"
#include <math.h>

//int8_t main_timer[NUM_OF_MOTORS] = { -ENCODER_OFFSET, -ENCODER_OFFSET, -ENCODER_OFFSET };
virtual_timer_t main_timer[NUM_OF_MOTORS];
int8_t main_visualizer[NUM_OF_MOTORS] = { 0, 0, 0 };

static int8_t MOTOR_0 = 0;
static int8_t MOTOR_1 = 1;
static int8_t MOTOR_2 = 2;

volatile int16_t rotations_per_sec[NUM_OF_MOTORS];
int32_t motor_freqs[NUM_OF_MOTORS];
int16_t motor_actual_speeds[NUM_OF_MOTORS] = { 0, 0, 0 };
int16_t period[3];

void motor_tick(void *i);

void timer_reset(int8_t sender) {
    /*switch(sender) {
        case 0:
            chSysLockFromISR();
            chVTResetI(&main_timer[sender]);
            chVTSetI(&main_timer[sender], MS2ST(15), motor_tick, &MOTOR_0);
            chSysUnlockFromISR();
            break;
        case 1:
            chSysLockFromISR();
            chVTResetI(&main_timer[sender]);
            chVTSetI(&main_timer[sender], MS2ST(15), motor_tick, &MOTOR_1);
            chSysUnlockFromISR();
            break;
        case 2:
            chSysLockFromISR();
            chVTResetI(&main_timer[sender]);
            chVTSetI(&main_timer[sender], MS2ST(15), motor_tick, &MOTOR_2);
            chSysUnlockFromISR();
            break;
    }*/
}

void encoder_pulse_captured(ICUDriver *icup) {
    /*int16_t period_width = 1000000/icuGetPeriodX(icup);
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
        //palClearPad(GPIOA, GPIOA_LED_GREEN);
        //main_visualizer[sender] = 1;
  
        // stats
        rotations_per_sec[sender] = period_width;
        period[sender] = period_calc;
        
        if(period_calc < motor_freqs[sender]) {;
            pwmEnableChannel(&PWMD1, sender, ++motor_actual_speeds[sender]);
        } else {
            pwmEnableChannel(&PWMD1, sender, --motor_actual_speeds[sender]);
        }
    }
    //timer_reset(sender);
    */
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
    } 

    /*switch(motor_number) {
        case 0:
            chVTReset(&main_timer[motor_number]);
            chVTSet(&main_timer[motor_number], MS2ST(15), motor_tick, &MOTOR_0);
            break;
        case 1:
            chVTReset(&main_timer[motor_number]);
            chVTSet(&main_timer[motor_number], MS2ST(15), motor_tick, &MOTOR_1);
            break;
        case 2:
            chVTReset(&main_timer[motor_number]);
            chVTSet(&main_timer[motor_number], MS2ST(15), motor_tick, &MOTOR_2);
            break;
    }*/
   
}

/*void motor_checker(int8_t motor_number) {
    if(main_timer[motor_number] < 0 && motor_actual_speeds[motor_number] != 0) {
    pwmEnableChannel(&PWMD1, motor_number, ++motor_actual_spee4ds[motor_number]);
    } else { return; }
    motor_checker(motor_number);
    chprintf(&SD1, "%d \r\n", motor_actual_speeds[motor_number]);
}*/

THD_WORKING_AREA(waMotorBalanceThread, 128);
THD_FUNCTION(MotorBalanceThread, arg) {

    int8_t i = (*(int8_t*)arg);
    (void)arg;

    //chprintf((BaseSequentialStream*)&SD1, "%d\r\n", i);
    //chprintf((BaseSequentialStream*)&SD1, "%d %5d %5d %5d %5d \r\n",i, main_timer[i], motor_actual_speeds[i], period[i], motor_freqs[i]);
    if (motor_actual_speeds[i] < 2048) {
        pwmEnableChannel(&PWMD1, i, ++motor_actual_speeds[i]);
    } else {
        chVTReset(&main_timer[i]);
        motor_actual_speeds[i] = motor_freqs[i]; // set to default
        pwmEnableChannel(&PWMD1, i, motor_actual_speeds[i]);
        chThdExit((msg_t)NULL);
    }
    timer_reset(i);
    chThdExit((msg_t)NULL);
}


void motor_tick(void *nm) {
    int8_t i = (*(int8_t*)nm);
    main_visualizer[i] = 0;
    palSetPad(GPIOA, GPIOA_LED_GREEN);
    switch(i) {
        case 0:
             chThdCreateStatic(waMotorBalanceThread, sizeof(waMotorBalanceThread), NORMALPRIO, MotorBalanceThread, &MOTOR_0);
            break;
        case 1:
             chThdCreateStatic(waMotorBalanceThread, sizeof(waMotorBalanceThread), NORMALPRIO, MotorBalanceThread, &MOTOR_1);
            break;
        case 2:
             chThdCreateStatic(waMotorBalanceThread, sizeof(waMotorBalanceThread), NORMALPRIO, MotorBalanceThread, &MOTOR_2);
            break;
    }
   
}

void timer_init() {
    for (int8_t i = 0; i < NUM_OF_MOTORS; i++) {
     chVTObjectInit(&main_timer[i]);
    }
}
 
void calculate_speed(double smer, double brana, int8_t percent) {
    brana /= 20;
    chprintf((BaseSequentialStream*)&SD1,"%d : %d \r\n", (int)smer, (int)brana);
    // calculate move angle
    smer /= 127;
    if (percent <= 100 && percent > 0) {
        if(smer != -2) {
            double smer_motor = -1;
            for (int8_t i = 0; i < NUM_OF_MOTORS; i++) {
                smer_motor += MOTOR_CONST;
                double speed = truncl(sin(((double)smer_motor-(double)smer)*(double)PI) * 17 * percent);
                speed += ((speed < 0) ? speed*-1 : speed) < 5 ? -speed : (speed < 0 ? -300 : 300);
                //speed *= i == 2 ? 1.2 : 1;
                move_motor(i,speed+brana);
                chprintf((BaseSequentialStream*)&SD1,"%d ", (int)speed);
            }
            chprintf((BaseSequentialStream*)&SD1,"\r\n");
        } else {
            set_motors_off();
        }
    }
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
            pinA = 9;
            pinB = 8;
            break;
        case 1:
            pinA = 12;
            pinB = 10;
            break;
        case 2:
            pinA = 6;
            pinB = 5;
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