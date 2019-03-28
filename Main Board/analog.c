#include "analog.h"
#include "motor.h"
#include "comunication.h"
#include "config.h"
#include "includes/chprintf.h"

int16_t sinus(int8_t input);

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
int16_t sensor_state[NUM_OF_SENSORS];

    int16_t sensor_x[NUM_OF_SENSORS] = {0, -38, -71, -92, -100, -92, -71, -38, 0, 38, 71, 92, 100, 92, 71, 38};
    int16_t sensor_y[NUM_OF_SENSORS] = {100, 92, 71, 38, 0, -38, -71, -92, -100 -92, -71, -38, 0, 38, 71, 92};

int16_t limit[NUM_OF_SENSORS] = {1400,550,500,1000,600,550,550,500,500,750,500,500,550,500,1000};


THD_WORKING_AREA(waSensorThread, 512);
THD_FUNCTION(SensorThread, arg) {

    (void)arg;

    adcsample_t sensor_value1;
    adcsample_t sensor_value2;

    while(1) {
        if (get_camera_output() == -128) {

            for (int8_t i = 0; i < NUM_OF_SENSORS/2; i++) {
                mx_set(i);
                chThdSleepMicroseconds(100);
                adcConvert(&ADCD1, &line_sensors_cfg1, &sensor_value1, 1);
                adcConvert(&ADCD1, &line_sensors_cfg2, &sensor_value2, 1);
                sensor_state[i] = sensor_value1 < limit[i]+100 ? 1 : 0;
                sensor_state[i+8] = sensor_value2 < limit[i+8]+300 ? 1 : 0;
            }
            
            int16_t x=0;
            int16_t y=0;

            for(int8_t i = 0; i < 16; i++){
        
                x += sensor_state[i]*sensor_x[i];
                y += sensor_state[i]*sensor_y[i];
            }


            double out = get_camera_output();
            double output;
            if (!x && !y) {
                out /= 127;
                output = out;
                //chprintf((BaseSequentialStream*)&SD1, "     out %f     ", out);
            } else {
                output = atan2(x, y) / PI; 
                //chprintf((BaseSequentialStream*)&SD1, "%5f ", output);
            }

            chMsgSend(moving_thread, (msg_t)(output*10000));
            // output
            if (SENSOR_DEBUG) {
                for(int8_t i = 0; i < NUM_OF_SENSORS; i++)
                {
                    int16_t output = sensor_state[i];
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
            double line = (double)chMsgGet(sensor_thread) / 10000;
            chMsgRelease(sensor_thread, MSG_OK);
            chprintf((BaseSequentialStream*)&SD1,"%f \r\n", line);
            // PROCESS
            //if (line <= 128 && line >= -128) { 
                calculate_speed(line,get_camera_output(),35);
           // }
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
