# Untitled - By: Administrator - pi 12 7 2018

import sensor, image, time, math, ustruct

from pyb import UART

uart = UART(3, 115200, timeout_char=1)  # init with given baudrate

color = [[0,0,0,0,0,0]]
cl = (0,0,0)

GAIN_SCALE = 0.1

#color = [[58, 87, 18, 65, -10, 52]]
color = [(17, 25, 13, 42, 22, 37)]
color = [(25, 63, 16, 52, 35, 52)]
color = [(15, 90, 30, 87, 29, 75)]
color = [(28, 63, 13, 82, 32, 80)]



modrabranka = [(18, 31, -33, -17, -3, 8)]#(23, 30, -24, -15, 2, 30)
zltabranka = [(42, 53, -44, -6, 45, 56)]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.CIF)
sensor.set_auto_gain(False, 10) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_auto_exposure(False, 30000000)
sensor.skip_frames(time = 2000)
clock = time.clock()

def uholkbrane(x, y):
    if x == 177 and y-144 < 0:
        uhol = 64
    elif x == 177:
        uhol = -64
    else:
        uhol = math.atan((y-144)/(x-177))/math.pi*127
    if y-144 < 0 and x-177 > 0:
        uhol = uhol + 127
    elif y-144 > 0 and x-177 > 0:
        uhol = uhol - 127
    return uhol

def vbrane(area):
    if area > 7500:
        return True
    return False

def cal():
    tres = [0, 0, 0, 0, 0, 0]

    tres[0] = image.rgb_to_lab(img.get_pixel(177, 192))[0]
    tres[1] = image.rgb_to_lab(img.get_pixel(177, 192))[0]
    tres[2] = image.rgb_to_lab(img.get_pixel(177, 192))[1]
    tres[3] = image.rgb_to_lab(img.get_pixel(177, 192))[1]
    tres[4] = image.rgb_to_lab(img.get_pixel(177, 192))[2]
    tres[5] = image.rgb_to_lab(img.get_pixel(177, 192))[2]
    nub = tres

    for x in range(140, 215, 4):
        for y in range(150, 192, 4):
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
    img.draw_circle(175,144,60,(0,0,0),1,True)
    #img.draw_circle(175, 144, 165, (255, 255, 255))
    #img.draw_circle(177, 144, 55, (255, 255, 255))
    #img.draw_cross(177, 144)
    #img.draw_circle(177, 185, 53, (0, 0, 0))
    #img.draw_rectangle(140, 151, 75, 42, cl)

    #defaults
    lopta = 127
    zarovnanie = 0

    utok = True
    branka = False

    if (color[0] != [0,0,0,0,0,0]):
        blobs = []
        for blob in img.find_blobs(color, invert = False, pixels_threshold=50, area_threshold=1, merge=True):
            #img.draw_rectangle(blob.rect())
            #img.draw_cross(blob.cx(), blob.cy())
            if pow(3025 < (blob.cx()-177), 2) + pow((blob.cy()-144), 2) < 31329:
                blobs.append(blob)
        areamax = 0
        sucet = [0, 0]
        areasum = 0
        for i in range(len(blobs)):
            if not areamax:
                areamax = blobs[i]
            elif blobs[i].w()*blobs[i].h() > areamax.w()*areamax.h():
                areamax = blobs[i]
        if areamax:
            if areamax.cx() == 177 and areamax.cy()-144 < 0:
                angle = 64
            elif areamax.cx() == 177:
                angle = -64
            else:
                angle = math.atan((areamax.cy()-144)/(areamax.cx()-177))/math.pi*127
            if areamax.cy()-144 < 0 and areamax.cx()-177 > 0:
                angle = angle + 127
            elif areamax.cy()-144 > 0 and areamax.cx()-177 > 0:
                angle = angle - 127
            lopta = angle
            img.draw_cross(areamax.cx(), areamax.cy())
        blobs = []

        # BRANKY - vypocet
        img.draw_circle(175,144,110,(0,0,0),1,True)
        if (branka):
            #ZLTA
            for blob in img.find_blobs(zltabranka, invert = False, pixels_threshold=50, area_threshold=1, merge=True):
                #img.draw_cross(blob.cx(), blob.cy())
                if 4225 < pow((blob.cx()-177), 2) + pow((blob.cy()-144), 2) < 31329:
                    blobs.append(blob)
            areamax = 0
            if (blobs):
                sucet = [0, 0]
                areasum = 0
                for i in range(len(blobs)):
                    if not areamax:
                        areamax = blobs[i]
                    elif blobs[i].w()*blobs[i].h() > areamax.w()*areamax.h():
                        areamax = blobs[i]
                    sucet[0] += blobs[i].cx()*blobs[i].area()
                    sucet[1] += blobs[i].cy()*blobs[i].area()
                    areasum += blobs[i].area()
                cx = sucet[0]//areasum
                cy = sucet[1]//areasum
                img.draw_cross(cx, cy, (255, 255, 0))
                zb = [cx, cy]
                #print(vbrane(areamax.w()*areamax.h()))
                zarovnanie = uholkbrane(cx, cy)
            else:
                zb = [0,0]
        else:
            #MODRA
            for blob in img.find_blobs(modrabranka, invert = False, pixels_threshold=100, area_threshold=1, merge=True):
                #img.draw_cross(blob.cx(), blob.cy())
                if 4225 < pow((blob.cx()-177), 2) + pow((blob.cy()-144), 2) < 31329:
                    blobs.append(blob)
            areamax = 0
            if (blobs):
                sucet = [0, 0]
                areasum = 0
                for i in range(len(blobs)):
                    if not areamax:
                        areamax = blobs[i]
                    elif blobs[i].w()*blobs[i].h() > areamax.w()*areamax.h():
                        areamax = blobs[i]
                    sucet[0] += blobs[i].cx()*blobs[i].area()
                    sucet[1] += blobs[i].cy()*blobs[i].area()
                    areasum += blobs[i].area()
                cx = sucet[0]//areasum
                cy = sucet[1]//areasum
                img.draw_cross(cx, cy, (0, 0, 255))
                mb = [areamax.cx(), areamax.cy()]
                zarovnanie = uholkbrane(areamax.cx(), areamax.cy())
            else:
                mb = [0, 0]
        blobs = []


        if utok:
            if (abs(lopta) < 63.5):
                lopta *= 2
            else:
                lopta *= 3/2


            if (lopta > 127 ):
                lopta -= lopta % 127
                lopta *= -1


            #UART COM
            uart.write(ustruct.pack("<bbb",128,int(lopta),int(zarovnanie)))
            print(str(int(lopta)) + " " + str(int(zarovnanie)))
        else:
            if areamax:
                if vbrane(areamax.w()*areamax.h()):
                    if lopta > 0:
                        uart.write(ustruct.pack("<bbb",128,64,0))
                    else:
                        uart.write(ustruct.pack("<bbb",128,-64,0))
                else:
                    if -96 < zarovnanie < 96:
                        uart.write(ustruct.pack("<bbb",128,0,0))
                    else:
                        uart.write(ustruct.pack("<bbb",128,int(zarovnanie)),0)
    else:
        color[0] = cal()
