import sensor, image, time, ustruct, math
from pyb import UART, LED, Pin

pin1 = Pin('P4', Pin.OUT_PP)
pin2 = Pin('P5', Pin.OUT_PP)

WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)

# middle_roi = ((int)((320-middle_wide)/2), 210, middle_wide, 20)

IMG_W = 320
IMG_H = 240
default_exposure_us = 7000

target_cx = int(IMG_W/2)
target_cy = int(IMG_H/2)
red_target_th = (10, 100, 15, 61, -22, 127)
green_target_th = (30, 100, -51, -7, -3, 30)
blue_target_th = (8, 80, -86, 22, -87, -9)
far_area_th = 3500
area_th = far_area_th

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

clock = time.clock()
#uart = UART(3, 9600)

def IMG_init():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_auto_gain(True)
    sensor.set_brightness(2)
    sensor.set_auto_whitebal(True)
    sensor.set_auto_exposure(False, exposure_us  = default_exposure_us)
    #sensor.set_auto_exposure(True)
    sensor.skip_frames(120)
    #red_led.on()
    #green_led.on()
    #blue_led.on()

def draw_keypoints(img, kpts):
    if kpts:
        print(kpts)
        img.draw_keypoints(kpts)
        img = sensor.snapshot()
        time.sleep_ms(1000)

def send_data(mark0, cx, cy, mark1):
    #global uart
    data = ustruct.pack("<bbbhhbbb",
                   0x5A,
                   0x5A,
                   mark0, # 0x00, 高四位是目前过滤器颜色 0x10红 0x20绿色 0x04蓝色
                   cx,
                   cy,
                   mark1,
                   0xA6,
                   0xA6)
    #uart.write(data);

#def

if __name__ == '__main__':
    IMG_init()
    flag_mode = 4 # 0：待机 1：红色 2：绿色 3：蓝色 4：颜色测试

    while(True):
        clock.tick()

        cx = int(IMG_W/2)
        cy = int(IMG_H/2)
        mark0 = 0
        mark1 = 0

        img = sensor.snapshot()

        blobs = []

        # 模式处理：指令（0x01 红 0x02绿 0x03蓝）
        #if uart.any():
            #cmd_byte_array = uart.read()
            #if cmd_byte_array[-1] == 0x00:
                #flag_mode = 0
            #elif cmd_byte_array[-1]&0x01:
                #flag_mode = 1
            #elif cmd_byte_array[-1]&0x02:
                #flag_mode = 2
            #elif cmd_byte_array[-1]&0x03:
                #flag_mode = 3
            #elif cmd_byte_array[-1]&0x04:
                #flag_mode = 4
            #else:
                #flag_mode = 0

        if flag_mode == 0:
            red_led.off()
            green_led.off()
            blue_led.off()
            mark0 = mark0|0x00
        elif flag_mode == 1:
            red_led.on()
            green_led.off()
            blue_led.off()
            mark0 = mark0|0x01
            blobs = img.find_blobs([red_target_th], merge = False, area_threshold = area_th)
        elif flag_mode == 2:
            red_led.off()
            green_led.on()
            blue_led.off()
            mark0 = mark0|0x02
            blobs = img.find_blobs([green_target_th], merge = False, area_threshold = area_th)
        elif flag_mode == 3:
            red_led.off()
            green_led.off()
            blue_led.on()
            blobs = img.find_blobs([blue_target_th], merge = False, area_threshold = area_th)
            mark0 = mark0|0x04
        elif flag_mode == 4:
            red_led.on()
            green_led.on()
            blue_led.on()

            blobs_r = img.find_blobs([red_target_th], roi = (85, 88, 103, 103), merge = False, area_threshold = area_th)
            blobs_g = img.find_blobs([green_target_th],roi = (85, 88, 103, 103), merge = False, area_threshold = area_th)
            blobs_b = img.find_blobs([blue_target_th],roi = (85, 88, 103, 103), merge = False, area_threshold = area_th)
            r_blob = []; g_blob = []; b_blob = [];
            r_blob = 0; g_blob = 0; b_blob = 0;
            if blobs_r:
                r_blob = max(blobs_r, key = lambda x: x.area())
            if blobs_g:
                g_blob = max(blobs_g, key = lambda x: x.area())
            if blobs_b:
                b_blob = max(blobs_b, key = lambda x: x.area())
            colors = [r_blob, g_blob, b_blob]
            color = 0
            dist = (IMG_W/2)**2+(IMG_H/2)**2
            for i in range(0, 3):
                if colors[i] != 0:
                    if (colors[i].cx() - int(IMG_W/2))**2+(colors[i].cy() - int(IMG_H/2))**2 < dist:
                        dist = (colors[i].cx() - IMG_W/2)**2+(colors[i].cy() - IMG_H/2)**2
                        color = i+1
            if color&0x01:
                pin1.high()
            else:
                pin1.low()
            if color&0x02:
                pin2.high()
            else:
                pin2.low()
            print(color);
            mark0 = mark0|0x08

        if blobs:
            target_blob = max(blobs, key = lambda x: x.area())
            cx = target_blob.cx()
            cy = target_blob.cy()

        # 图像调试内容：
        #print(cx, cy)
        blobs_r = []; blobs_g = []; blobs_b = []
        if flag_mode == 5:
            blobs_r = img.find_blobs([red_target_th], merge = True, area_threshold = area_th)
            blobs_g = img.find_blobs([green_target_th], merge = True, area_threshold = area_th)
            blobs_b = img.find_blobs([blue_target_th], merge = True, area_threshold = area_th)
            if blobs_r:
                for blobb in blobs_r:
                    img.draw_cross(blobb.cx(), blobb.cy(), color = RED)
                    img.draw_circle(blobb.cx(), blobb.cy(), int(blobb.w()/2), color = RED)
            if blobs_g:
                for blobb in blobs_g:
                    img.draw_cross(blobb.cx(), blobb.cy(), color = GREEN)
                    img.draw_circle(blobb.cx(), blobb.cy(), int(blobb.w()/2), color = GREEN)
            if blobs_b:
                for blobb in blobs_b:
                    img.draw_cross(blobb.cx(), blobb.cy(), color = BLUE)
                    img.draw_circle(blobb.cx(), blobb.cy(), int(blobb.w()/2), color = BLUE)
        else:
            if blobs:
                for blobb in blobs:
                    img.draw_cross(blobb.cx(), blobb.cy())
                    img.draw_circle(blobb.cx(), blobb.cy(), int(blobb.w()/2))

        send_data(mark0, cx, cy, mark1)
