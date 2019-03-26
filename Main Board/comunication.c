#include "comunication.h"
#include "config.h"
#include "includes/chprintf.h"

double get_camera_output() {
    int8_t output = sdGet(&SD1);
    //chprintf((BaseSequentialStream*)&SD1,"%d \r\n", output);
    return output;
}