# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time
from pyb import UART
uart = UART(3,38400, timeout_char = 1000)
color = (42, 62, 80, 44, 67, 23)


sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(30)     # Wait for settings take effect.
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()                # Create a clock object to track the FPS.


while(True):
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs([color],pixels_treshold=100,area_treshold=100,merge=True):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(),blob.cy())
        uart.writechar(blob.cx())
        uart.writechar(blob.cy())
   # print(clock.fps())
