import sensor, image, time, ustruct, math
from pyb import UART, LED, Pin

# LED colors
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

#Image related
IMG_W = 320
IMG_H = 240
brightness = 2
default_exposure_us = 7000

#Color detection thresholds/rois:
color_detect_roi = (85, 88, 103, 103)
red_target_th = (10, 100, 15, 61, -22, 127)
green_target_th = (21, 100, -67, -15, -3, 34)
blue_target_th = (8, 80, -86, 22, -87, -9)
far_area_th = 3500
area_th = far_area_th

#UART def:
uart = UART(3, 9600)

clock = time.clock()
def IMG_init():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_auto_gain(True)
    sensor.set_brightness(brightness)
    sensor.set_auto_whitebal(True)
    sensor.set_auto_exposure(False, exposure_us  = default_exposure_us)
    #sensor.set_auto_exposure(True)
    sensor.skip_frames(120)

def send_data(mark0):
    #global uart
    data = ustruct.pack("<bbbbb",
                   0x5A,
                   0x5A,
                   mark0, # 0x00, 高四位是目前过滤器颜色 0x10红 0x20绿色 0x04蓝色
                   0xA6,
                   0xA6)
    uart.write(data);

if __name__ == '__main__':
    IMG_init()

    while(True):
        clock.tick()
        mark0 = 0
        mark1 = 0

        img = sensor.snapshot()

        r_blob = 0; g_blob = 0; b_blob = 0;color = 0;
        blobs_r = img.find_blobs([red_target_th], roi = color_detect_roi, merge = False, area_threshold = area_th)
        blobs_g = img.find_blobs([green_target_th],roi = color_detect_roi, merge = False, area_threshold = area_th)
        blobs_b = img.find_blobs([blue_target_th],roi = color_detect_roi, merge = False, area_threshold = area_th)
        if blobs_r:
            r_blob = max(blobs_r, key = lambda x: x.area())
        if blobs_g:
            g_blob = max(blobs_g, key = lambda x: x.area())
        if blobs_b:
            b_blob = max(blobs_b, key = lambda x: x.area())
        colors = [r_blob, g_blob, b_blob]
        dist = (IMG_W/2)**2+(IMG_H/2)**2
        for i in range(0, 3):
            if colors[i] != 0:
                if (colors[i].cx() - int(IMG_W/2))**2+(colors[i].cy() - int(IMG_H/2))**2 < dist:
                    dist = (colors[i].cx() - IMG_W/2)**2+(colors[i].cy() - IMG_H/2)**2
                    color = i+1
        if color == 1:
            red_led.on()
            blue_led.off()
            green_led.off()
            img.draw_circle(5, 5, 5, color = RED)
        elif color == 2:
            red_led.off()
            blue_led.off()
            green_led.on()
            img.draw_circle(5, 5, 5, color = GREEN)
        elif color == 3:
            red_led.off()
            blue_led.on()
            green_led.off()
            img.draw_circle(5, 5, 5, color = BLUE)
        else:
            red_led.on()
            blue_led.on()
            green_led.on()
        mark0 = mark0|(color << 4)
        mark0 = mark0|0x08

        send_data(mark0)
