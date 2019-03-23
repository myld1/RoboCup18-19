#ifndef ANALOG_H_
#define ANALOG_H_

#include "ch.h"
#include "hal.h"
#include "includes/chprintf.h"

#define NUM_OF_SENSORS 16
#define SENSOR_DEBUG false

void init_moving_thread(void);
void init_sensor_thread(void);
bool lines_check(void);

#endif