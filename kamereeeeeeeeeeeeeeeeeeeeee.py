# Untitled - By: Administrator - pi 12 7 2018

import sensor, image, time

from pyb import UART

uart = UART(3, 115200, timeout_char=1)  # init with given baudrate

color = [[0,0,0,0,0,0]]
cl = (0,0,0)

#color = [[58, 87, 18, 65, -10, 52]]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.CIF)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

def cal():
    tres = [0, 0, 0, 0, 0, 0]

    tres[0] = image.rgb_to_lab(img.get_pixel(177, 144))[0]
    tres[1] = image.rgb_to_lab(img.get_pixel(177, 144))[0]
    tres[2] = image.rgb_to_lab(img.get_pixel(177, 144))[1]
    tres[3] = image.rgb_to_lab(img.get_pixel(177, 144))[1]
    tres[4] = image.rgb_to_lab(img.get_pixel(177, 144))[2]
    tres[5] = image.rgb_to_lab(img.get_pixel(177, 144))[2]
    nub = tres

    for x in range(123, 229, 4):
        for y in range(91, 197, 4):
            for i in range(6):
                if i%2:
                    if image.rgb_to_lab(img.get_pixel(x, y))[i//2] > tres[i]:
                        tres[i] = image.rgb_to_lab(img.get_pixel(x, y))[i//2]
                else:
                    if image.rgb_to_lab(img.get_pixel(x, y))[i//2] < tres[i]:
                        tres[i] = image.rgb_to_lab(img.get_pixel(x, y))[i//2]
    for i in range(6):
        tres[i] = (tres[i] + 3*nub[i]) // 4

    print(tres)
    return tres

while(True):
    clock.tick()
    img = sensor.snapshot()
   # img.draw_circle(177, 144, 75, (0, 0, 0))
    #img.draw_rectangle(123, 91, 106, 106, cl)

    if (color[0] != [0,0,0,0,0,0]):
        x = []
        for blob in img.find_blobs(color, pixels_threshold=100, area_threshold=100, merge=False):
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            x.append(blob.cx())
        for i in range(len(x)):
            x[i] = (176 - x[i]) / 528
            #print(x[i])
    else:
        color[0] = cal()


