import cv2
import threading
import time
import numpy as np
import RPi.GPIO as GPIO
from RpiMotorLib import RpiMotorLib

GPIO.setmode(GPIO.BCM)

color_ball = [67, 142, 120]# Color in BGR colorspace
color_paddle = [84,54,22]
n=0
center_x = center_y = center_xp = center_xy = 0
rs = 4
ls = 27

GPIO.setup(rs, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(ls, GPIO.IN, pull_up_down=GPIO.PUD_UP)

cap = cv2.VideoCapture(0)
cap.set(3, 640)  
cap.set(4, 480)

def get_limits(color):
    c = np.uint8([[color]])  # BGR values
    hsvC = cv2.cvtColor(c, cv2.COLOR_BGR2HSV)
    
    hue = hsvC[0][0][0]  # Get the hue value

    # Handle red hue wrap-around
    if hue >= 165:  # Upper limit for divided red hue
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([180, 255, 255], dtype=np.uint8)
    elif hue <= 15:  # Lower limit for divided red hue
        lowerLimit = np.array([0, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)
    else:
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)

    return lowerLimit, upperLimit

def tracking():
    global cap,img,center_x, center_y,n, center_xp, center_xy
    while True:
        ret, frame = cap.read() #Get frames

        hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) #Convert into HSV

        lowerLimit_ball, upperLimit_ball = get_limits(color_ball) #Get the range of the color that need to identify
        lowerLimit_paddle, upperLimit_paddle = get_limits(color_paddle)

        mask_ball = cv2.inRange(hsvImage, lowerLimit_ball, upperLimit_ball) #Create a mask (If the color is identified - white, other locations black)
        mask_paddle = cv2.inRange(hsvImage, lowerLimit_paddle, upperLimit_paddle)

        contours_ball, gg = cv2.findContours(mask_ball, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) #Find those white signs
        contours_paddle, gg1 = cv2.findContours(mask_paddle, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) 
        cv2.imshow('c', mask_ball)
        for contour in contours_ball:
            area = cv2.contourArea(contour) #Find the area of each contour
            if area > 100:  # Adjust the area threshold as needed
                x, y, w, h = cv2.boundingRect(contour) #Get the location as a rectangle
                #cv2.drawContours(frame, contour, -1, (0,255,0), 1)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 5) 
                center_x = x + w // 2
                center_y = y + h // 2
                print(f"ball: ({center_x}, {center_y})")
                
        for contour in contours_paddle:
            areap = cv2.contourArea(contour) #Find the area of each contour
            if areap > 100:  # Adjust the area threshold as needed
                xp, yp, wp, hp = cv2.boundingRect(contour) #Get the location as a rectangle
                #cv2.drawContours(frame, contour, -1, (0,255,0), 1)
                cv2.rectangle(frame, (xp, yp), (xp + wp, yp + hp), (255, 0, 0), 5) 
                center_xp = xp + wp // 2
                center_yp = yp + hp // 2
                print(f"paddle: ({center_xp}, {center_yp})")

        cv2.imshow('frame', frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            n=100
            cap.release()
            cv2.destroyAllWindows()
            break

def following():
    global center_xp, center_x, n
    while True:
        if center_xp > center_x:
            while center_xp > center_x:
                step.motor_go(True, "Full", 200, 0.0004, False, 0)
                print(f"position - {center_xp}, target - {center_x}")
                if n == 100:
                    break
        if center_xp < center_x:
            while center_xp < center_x:
                step.motor_go(False, "Full", 200, 0.0004, False, 0)
                print(f"position - {center_xp}, target - {center_x}")
                if n == 100:
                    break
        if n == 100:
            break

d = 24
s = 23

step = RpiMotorLib.A4988Nema(d,s,(21,21,21), "DRV8825")


test1 = threading.Thread(target=tracking, args=())
test2 = threading.Thread(target=following, args=())

test1.start()
test2.start()


