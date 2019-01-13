#ifndef CONFIG_H_
#define CONFIG_H_

#include "ch.h"
#include "hal.h"
#include <icu_lld.h>
#include "/home/adam/ChibiOS_16.1.9/os/hal/lib/streams/chprintf.h"

void init_pins(void);
void board_init(void);
void configure_icu_notifications(bool state);

#endif
