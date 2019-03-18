#include "config.h"
#include "motor.h"



static PWMConfig pwmcfg = {
    48000000,
    2048,
    NULL,
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    0,
    0
};

static const ICUConfig icucfgA = {
    ICU_INPUT_ACTIVE_HIGH,
    1000000,
    encoder_pulse_captured,
    NULL,
    NULL,
    ICU_CHANNEL_1,
    0   
};

static const ICUConfig icucfgB = {
    ICU_INPUT_ACTIVE_HIGH,
    1000000,
    NULL,
    encoder_pulse_captured,
    NULL,
    ICU_CHANNEL_2,
    0   
};

static const ICUConfig icucfgC = {
    ICU_INPUT_ACTIVE_HIGH,
    1000000,
    NULL,
    encoder_pulse_captured,
    NULL,
    ICU_CHANNEL_1,
    0   
};

static const SerialConfig usart_cfg = {
	115200,
	0,
	0,
	0
};

void init_pins() {
    // Serial
    palSetPadMode(GPIOA,15,PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOB,3,PAL_MODE_ALTERNATE(7));
    // Motor 1
    palSetPadMode(GPIOC,10,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC,12,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA,8,PAL_MODE_ALTERNATE(1));
    // Motor 2
    palSetPadMode(GPIOC,8,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC,9,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA,9,PAL_MODE_ALTERNATE(1));
    // Motor 3
    palSetPadMode(GPIOC,5,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC,6,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA,10,PAL_MODE_ALTERNATE(1));
    // Input capture 
    palSetPadMode(GPIOA,0,PAL_MODE_ALTERNATE(1));
    palSetPadMode(GPIOA,1,PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOB,4,PAL_MODE_ALTERNATE(2));
    // ADC
    palSetPadMode(GPIOA,6,PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOA,7,PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOA,5,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,6,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC,7,PAL_MODE_OUTPUT_PUSHPULL);
    // Usta
    // PC0

    // Kicker Solenoid
}

void board_init() {
    // System
    halInit();
    chSysInit();
    
    // Serial
    sdInit();
    sdStart(&SD1, &usart_cfg);
    
    // PWM
    pwmInit();
    pwmStart(&PWMD1,&pwmcfg);

    // ICU
    icuInit();
    
    icuStart(&ICUD2, &icucfgA);
    icuStart(&ICUD5, &icucfgB);
    icuStart(&ICUD3, &icucfgC);
    
    icuStartCapture(&ICUD2);
    icuStartCapture(&ICUD5);
    icuStartCapture(&ICUD3);

    //ADC
    palSetGroupMode(GPIOA, PAL_PORT_BIT(1) | PAL_PORT_BIT(2),
                    0, PAL_MODE_INPUT_ANALOG);
    palSetGroupMode(GPIOB, PAL_PORT_BIT(1) | PAL_PORT_BIT(2),
                    0, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, NULL);
    adcSTM32EnableTSVREFE();
    
    configure_icu_notifications(true);
}

void configure_icu_notifications(bool state) {
    if(state) {
        icuEnableNotifications(&ICUD2);
        icuEnableNotifications(&ICUD5);
        icuEnableNotifications(&ICUD3);
	} else {          
        icuDisableNotifications(&ICUD2);
        icuDisableNotifications(&ICUD5);
        icuDisableNotifications(&ICUD3);
    }
}