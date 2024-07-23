# DualPlay
# Code for raspberry pi

import serial
import time
import os
import cv2
import numpy as np
import pygame

pygame.mixer.init()


# Setup Camera
cap = cv2.VideoCapture(0)
cap.set(3, 600)
cap.set(4, 640)

color_ball = [27, 176, 138]# Color in BGR colorspace
color_paddle = [0,0,255] 
#color_paddle = [23,0,147]
#color_paddle = [0,0,166] 

# Variables to get the mid point of the ball and the paddle
# center_x = center_y = center_xp = center_xy = 0
# But here we use coners of identified colors (ball and paddle) to get the location

# Define serial connections
arduino1 = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
arduino2 = serial.Serial('/dev/ttyACM1', 115200, timeout=1)

# Define variables to identify the correct ports
port = 0
# port = 1 means arduino1 is the arduino mega (human play)
# port = 2 means arduino2 is the arduino mega (human play)

# reset serial buffer
arduino1.reset_input_buffer()
arduino2.reset_input_buffer()

# Send a message to start the program
time.sleep(2)
arduino1.write(b'port')
arduino2.write(b'port')

# loop to get the feedback from the arduino and identify the correct port 
while True :
    if arduino1.in_waiting > 0:
        com = arduino1.readline().decode('utf-8').rstrip()
        arduino1.reset_input_buffer()
        port = 1 
        break

    if arduino2.in_waiting > 0:
        com = arduino2.readline().decode('utf-8').rstrip()
        arduino2.reset_input_buffer()
        port = 2
        break 

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


# Function to send a message to arduino mega
def sendMega(msg):
    global port
    if port==1:
        arduino1.write(f"{msg}\n".encode())
    else:
        arduino2.write(f"{msg}\n".encode())
        
    time.sleep(0.01)

# Function to send a message to arduino Uno
def sendUno(msg):
    global port
    if port==1:
        arduino2.write(f"{msg}\n".encode())
    else:
        arduino1.write(f"{msg}\n".encode())
    
    time.sleep(0.01)

# Function to get data from arduino Mega
def getMega():
    com = None
    if port==1:
        while True :
            if arduino1.in_waiting > 0:
                com = arduino1.readline().decode('utf-8').rstrip()
                arduino1.reset_input_buffer() 
                break
    else:
        while True:
            if arduino2.in_waiting > 0:
                com = arduino2.readline().decode('utf-8').rstrip()
                arduino2.reset_input_buffer()
                break
    return com

# Function to get data from arduino Mega without waiting
def _getMega():
    com = None
    if port==1:
        if arduino1.in_waiting > 0:
            com = arduino1.readline().decode('utf-8').rstrip()
            arduino1.reset_input_buffer() 
    else:
        if arduino2.in_waiting > 0:
            com = arduino2.readline().decode('utf-8').rstrip()
            arduino2.reset_input_buffer()
            
    return com

def tracking():
    global cap
    
    arduino1.reset_input_buffer()
    arduino2.reset_input_buffer()
    
    x = y = w = h = xp = yp = wp = hp = 0
    while True:
        ret, frame = cap.read() #Get frames

        hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) #Convert into HSV

        lowerLimit_ball, upperLimit_ball = get_limits(color_ball) #Get the range of the color that need to identify
        lowerLimit_paddle, upperLimit_paddle = get_limits(color_paddle)

        mask_ball = cv2.inRange(hsvImage, lowerLimit_ball, upperLimit_ball) #Create a mask (If the color is identified - white, other locations black)
        mask_paddle = cv2.inRange(hsvImage, lowerLimit_paddle, upperLimit_paddle)

        contours_ball, gg = cv2.findContours(mask_ball, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) #Find those white signs
        contours_paddle, gg1 = cv2.findContours(mask_paddle, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) 
        #cv2.imshow('c', mask_ball)
        
        for contour in contours_ball:
            area = cv2.contourArea(contour) #Find the area of each contour
            if area > 100:  # Adjust the area threshold as needed
                x, y, w, h = cv2.boundingRect(contour) #Get the location as a rectangle
                #cv2.drawContours(frame, contour, -1, (0,255,0), 1)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 5) 
                #center_x = x + w // 2
                #center_y = y + h // 2
                print(f"ball: ({y}, {yp})")
                
        for contour in contours_paddle:
            areap = cv2.contourArea(contour) #Find the area of each contour
            if areap > 500:  # Adjust the area threshold as needed
                xp, yp, wp, hp = cv2.boundingRect(contour) #Get the location as a rectangle
                #cv2.drawContours(frame, contour, -1, (0,255,0), 1)
                cv2.rectangle(frame, (xp, yp), (xp + wp, yp + hp), (255, 0, 0), 5) 
                #center_xp = xp + wp // 2
                #center_yp = yp + hp // 2
                print(f"paddle: ({xp}, {xp+wp})")
                
        if x < xp+33 and (y-yp-hp)>=-40:
                print(0)
                sendUno('0\n')
                
        elif x+w > xp+wp-33 and (y-yp-hp)>=-40:
                print(1)
                sendUno('1\n')
                
        elif (x>= xp+33 ) and ((x+w)<=(xp+wp-33)):
                print(-1)
                sendUno('-1\n')
           
            
        if 30>=(y-yp-hp)>=-20 and ((x+w) >= xp) and ((x)<=(xp+wp)):
            sendUno("2\n")
            print(10)
            
               
        cv2.imshow('frame', frame)
        
        if cv2.waitKey(1) and _getMega()=="6":
            sendUno('10')
            cv2.destroyAllWindows()
            break
        
            
# main loop
while True:

    # reset serial buffer
    arduino1.reset_input_buffer()
    arduino2.reset_input_buffer()
    
    # Send a message to start the program
    time.sleep(1)
    sendMega('start')

    # Get initial command to start the game
    pygame.mixer.music.load("1.mp3")
    pygame.mixer.music.play()
    while True:
        com = getMega()
        if com=='0' or len(com)==1:
            print(com)
            pygame.mixer.music.stop()
            break
    
    # Shutdown raspberry pi if the com = 0
    if(com=='0'):
        arduino1.close()
        arduino2.close()
        time.sleep(1)
        os.system("sudo shutdown now")
        exit

    else:
        # get the game mode
        gm = com
    
    while True:
        com = getMega()
        if com == "left":
            sendUno('-2')
            break
        elif com == "right":
            sendUno('-3')
            break
    
    arduino1.reset_input_buffer()
    arduino2.reset_input_buffer()
            
    # game mode = 1 means it is human vs human
    if gm == '1':
    
        # wait until for the signal from arduino mega
        while True:
            com = getMega()
            if com =="100":
                break
        
        while True:
            # run until ir detect the ball
            while True:
                sendUno('5')
                signal1 = _getMega()
                if signal1 == '6':
                    sendUno('10\n')
                    break
                    
            while True:
                com = getMega()
                if com == "left":
                    sendUno('-2')
                    break
                elif com == "right":
                    sendUno('-3')
                    break
                
            while True:
                signal2 = getMega()
                if signal2 is not None:
                    break
            
            if signal2 == "7":
                sendUno('5')
                continue
            if signal2 == "8":
                sendUno('10\n')
                break
            
    elif gm == '2':
        # wait until for the signal from arduino mega
        while True:
            com = getMega()
            if com =="100":
                break
        
        while True:
            # Send the message to uno 
            sendUno('6')
            
            arduino1.reset_input_buffer()
            arduino2.reset_input_buffer()
            
            # run until ir detect the ball
            tracking()
            
            while True:
                com = getMega()
                if com == "left":
                    sendUno('-2')
                    break
                elif com == "right":
                    sendUno('-3')
                    break
            
            # wait until for the signal from arduino Mega
            while True:
                signal2 = getMega()
                if signal2 is not None:
                    break
            
            if signal2 == "7":
                continue
            elif signal2 == "8":
                sendUno('10\n')
                break
                                                                                                                                                                          
