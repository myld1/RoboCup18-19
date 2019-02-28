#ifndef CONFIG_H_
#define CONFIG_H_

#include "ch.h"
#include "hal.h"
#include <icu_lld.h>
#include "includes/chprintf.h"

void init_pins(void);
void board_init(void);
void configure_icu_notifications(bool state);

#endif
