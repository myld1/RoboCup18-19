#include "analog.h"
#include "motor.h"
#include "comunication.h"
#include "config.h"
#include "includes/chprintf.h"


void mx_set(uint8_t num) {
    if (num & 0b1) {
        palClearPad(GPIOA,5);
    } else {
        palSetPad(GPIOA,5);
    }

    if (num & 0b10) {
        palClearPad(GPIOC,7);
    } else {
        palSetPad(GPIOC,7);
    }

    if (num & 0b100) {
        palClearPad(GPIOB,6);
    } else {
        palSetPad(GPIOB,6);
    }
}

void mx_specify(bool a, bool b, bool c) {
    if (a){
        palSetPad(GPIOA,5);
    } else {
        palClearPad(GPIOA,5);
    }
    if (b) {
        palSetPad(GPIOC,7);
    } else {
        palClearPad(GPIOC,7);
    }
    if (c) {
        palSetPad(GPIOB,6);
    } else {
        palClearPad(GPIOB,6);
    }
}

static const ADCConversionGroup line_sensors_cfg1  = {
    FALSE,
    1,
    NULL,
    NULL,
    0,
    ADC_CR2_SWSTART,
    0,
    ADC_SMPR2_SMP_AN6(ADC_SAMPLE_3),
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN6)
};

static const ADCConversionGroup line_sensors_cfg2  = {
    FALSE,
    1,
    NULL,
    NULL,
    0,
    ADC_CR2_SWSTART,
    0,
    ADC_SMPR2_SMP_AN7(ADC_SAMPLE_3),
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN7)
};

thread_t *sensor_thread;
thread_t *moving_thread;
int16_t sensors[NUM_OF_SENSORS];
int8_t sensor_state[NUM_OF_SENSORS]; 

THD_WORKING_AREA(waSensorThread, 512);
THD_FUNCTION(SensorThread, arg) {

    (void)arg;

    adcsample_t sensor_value1;
    adcsample_t sensor_value2;

    while(1) {
        if (get_camera_output() == -128) {
            for (uint8_t i = 0; i < NUM_OF_SENSORS/2; i++) {
                mx_set(i);
                adcConvert(&ADCD1, &line_sensors_cfg1, &sensor_value1, 1);
                adcConvert(&ADCD1, &line_sensors_cfg2, &sensor_value2, 1);
                sensor_state[i] = sensor_value1 < 500 ? 1 : 0;
                sensor_state[i+8] = sensor_value2 < 500 ? 1 : 0;
            }
            
            for(i = 0; i < 16; i++){
        
                x -= input[i]*sinus(i);
                y += input[i]*sinus(i+4);
            }


            int8_t out = get_camera_output();
            double output;
            if (!x) {
                if (y > 0) {
                    output = 0;
                } else if (y < 0) {
                    output = 1;
                } else {
                    output = out;
                }
            } else {
                output = atan(y / x) / PI;
                if (x > 0) {
                    output -= 1;
                }  
            }
    
            chMsgSend(moving_thread, output);
            // output
            if (SENSOR_DEBUG) {
                for(int8_t i = 0; i < NUM_OF_SENSORS; i++)
                {
                    int16_t output = sensors[i];
                    if (output < 800) { output=1; }
                    chprintf((BaseSequentialStream*)&SD1, "%5d ", output);
                }
                chprintf((BaseSequentialStream*)&SD1, "\n\r");
            }
        }
        chThdSleepMicroseconds(100);
    }

}

 THD_WORKING_AREA(waMoveThread, 128);
    THD_FUNCTION(MoveThread, arg) {
    
        (void)arg;

        while(1){
            sensor_thread = chMsgWait();
            double line = (double)chMsgGet(sensor_thread);
            chMsgRelease(sensor_thread, MSG_OK);
            // PROCESS
            if (abs(line) <= 128) { 
                calculate_speed(line,get_camera_output(),40);
            }
        }
    }

void init_sensor_thread() {

    for (int8_t i = 0; i < NUM_OF_SENSORS; i++) {
		sensors[i] = 0;
    }

    sensor_thread = chThdCreateStatic(waSensorThread, sizeof(waSensorThread), NORMALPRIO, SensorThread, NULL);
}

void init_moving_thread() {
    moving_thread = chThdCreateStatic(waMoveThread, sizeof(waMoveThread), NORMALPRIO, MoveThread, NULL);
}

int sinus(int input){

    int iai;
    input %= 16;
    iai = 1;
    if(input > 7){
        
        input -= 8;
        iai = -1;
        
    }
    switch(input){
        
        case 0:  
        iai *= 0;
        break;
        
        case 1:
        case 7:
        iai *= 382;
        break;
        
        case 2:
        case 6:
        iai *= 707;
        break;
        
        case 3:
        case 5:
        iai *= 923;
        break;
        
        case 4:
        iai *= 1000;
        break;
        
    }
    return iai;
    
}

int main(){
    
    int i;
    double x, y;
    int mag;
    int input[16];
    
    /*for(i = 0; i < 16; i++){
        
        input[i] = 0;
        
    }
    
    input[12] = 1;
    input[0] = 1;*/
    
    // TREBA NASTAVIT DO input[] HODNOTY 0/1 PODLA SENZOROV
    
    
    if(mag == 0){
        output =
    }else{
        
        x /= mag;
        y /= mag;
        
    }
    
    printf("%f \r\n", x);
    printf("%f \r\n", y);
    
} 