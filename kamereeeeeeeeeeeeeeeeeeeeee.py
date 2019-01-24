# Untitled - By: Administrator - pi 12 7 2018

import sensor, image, time

color = [(36, 67, -7, 81, 29, 72)]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.CIF)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()
    x = []
    for blob in img.find_blobs(color, pixels_threshold=100, area_threshold=100, merge=True):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        x.append(blob.cx())
    for i in range(len(x)):
        x[i] = (176 - x[i]) / 528
        print(x[i])
