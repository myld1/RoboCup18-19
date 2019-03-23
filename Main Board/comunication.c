#include "comunication.h"
#include "config.h"
#include "includes/chprintf.h"

float get_camera_output() {
    float output = sdGet(&SD1)/10000;
    return output;
}