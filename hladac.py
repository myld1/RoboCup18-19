# Untitled - By: Administrator - pi 12 7 2018

import sensor, image, time

color = [[]]
tres = [0, 0, 0, 0, 0, 0]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.CIF)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)     # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

img = sensor.snapshot()
img.draw_circle(177, 144, 75)
img.draw_rectangle(123, 91, 106, 106)

img = sensor.snapshot()

tres[0] = image.rgb_to_lab(img.get_pixel(177, 144))[0]
tres[1] = image.rgb_to_lab(img.get_pixel(177, 144))[0]
tres[2] = image.rgb_to_lab(img.get_pixel(177, 144))[1]
tres[3] = image.rgb_to_lab(img.get_pixel(177, 144))[1]
tres[4] = image.rgb_to_lab(img.get_pixel(177, 144))[2]
tres[5] = image.rgb_to_lab(img.get_pixel(177, 144))[2]

#for blob in img.find_blobs(color, pixels_threshold=100, area_threshold=100, merge=True):

for x in range(123, 229, 4):
    for y in range(91, 197, 4):
        for i in range(6):
            if i%2:
                if image.rgb_to_lab(img.get_pixel(x, y))[i//2] > tres[i]:
                    tres[i] = image.rgb_to_lab(img.get_pixel(x, y))[i//2]
            else:
                if image.rgb_to_lab(img.get_pixel(x, y))[i//2] < tres[i]:
                    tres[i] = image.rgb_to_lab(img.get_pixel(x, y))[i//2]

color[0] = tres
print(tres)
